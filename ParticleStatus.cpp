/*
 * ParticleStatus.cpp
 *
 *  Created on: Sep 19, 2014
 *      Author: S158879
 */

#include "ParticleStatus.h"
#include "Macros.h"

ParticleStatus::ParticleStatus():energy(0.0), ifHard(false), maxSigma(0.0f), m(0.0f), c(0.0f){}
ParticleStatus::ParticleStatus(Position pos_, Direction dir_, float energy_, float m_, float c_):pos(pos_), dir(dir_),
		energy(energy_), ifHard(false), maxSigma(0.0f), m(m_), c(c_){
}
ParticleStatus::~ParticleStatus() {
	// TODO Auto-generated destructor stub
}

void ParticleStatus::update(Vector3 step, float theta, float phi, float energyTran){
	pos.move(step);
	dir.transform(theta, phi);
	energy -= energyTran;
}

void ParticleStatus::update(Vector3 step, float theta, float phi, float energyTran, int crossBound){
	pos.move(step, crossBound);
	dir.transform(theta, phi);
	energy -= energyTran;
}

void ParticleStatus::setStatus(Position position_, Direction direction_, float energy_){
	pos = position_;
	dir = direction_;
	energy = energy_;
}

float ParticleStatus::energyInOneStep(const Voxel & vox, const RSPW * resStpPowWater, const MSPR * massStpPowRatio, float stepLength) const{
	//calculate equivalent step in water
	float stepInWater;
	float density = vox.giveDensity();
	float mspr = massStpPowRatio->lookMSPR(vox.giveMatID(), energy);
	stepInWater = mspr*stepLength*density/WATERDENSITY;

	//calculate energy transfer
	float rspw = resStpPowWater->lookLw(energy);
	float b = resStpPowWater->lookB(energy);

	float de1 = stepInWater*rspw;
	float temp = energy/m;
	float eps = de1/energy;
	return de1*(1.0f + eps/(1.0f+temp)/(2.0f+temp) + eps*eps*(2.0f+2.0f*temp+temp*temp)/(1.0f+temp)/(1.0f+temp)/(2.0f+temp)/(2.0f+temp)
			- b*eps*(0.5f+2.0f*eps/3.0f/(1.0f+temp)/(2.0f+temp) + (1.0f-b)*eps/6.0f) );
}

float ParticleStatus::getMaxStep(const Voxel & vox, const RSPW * resStpWater, const MSPR * massStpPowRatio) const{
	float stepInWater = MAXENERGYRATIO*energy/resStpWater->lookLw(energy - 0.5f*MAXENERGYRATIO*energy);
	return stepInWater*WATERDENSITY/vox.giveDensity()/massStpPowRatio->lookMSPR(vox.giveMatID(), energy - 0.5f*MAXENERGYRATIO*energy);
}

float ParticleStatus::totalLinearSigma(const Voxel & vox, const MacroCrossSection * macroSigma, float e) const{
	float sigIon = macroSigma->lookIon(e);
	float sigPPE = macroSigma->lookPE(e);
	float sigPOE = macroSigma->lookOE(e);
	float sigPOI = macroSigma->lookOI(e);
	float density = vox.giveDensity();
	float edensity = vox.giveEDensity();
	return sigIon*edensity + (sigPPE + sigPOE + sigPOI)*density;
}

float ParticleStatus::energyStraggling(Random * ranGenerator, const Voxel & vox, float stepLength) const{
	//energyStraggling
	float edensity = vox.giveEDensity();
	float variance = TWOPIRE2MENEW*edensity*stepLength*c*c*MIN(MINELECTRONENERGY, maxDeltaElectronEnergy())*(1.0/(beta()*beta()) - 0.5f);
	float StdGauss;
	ranGenerator->resetDimensionality(1);
	ranGenerator->getGaussians(&StdGauss);
	return StdGauss*sqrt(variance);
}

void ParticleStatus::propagate(Random * ranGenerator, Phantom * phantom, Voxel & vox, const MacroCrossSection * macroSigma, const RSPW * rspw, const MSPR * mspr,
		float & energyTransfer, float & stepLength){
	// rescale maxStep to let energy transferred in one step < MAXENERGYRATIO
	float thisMaxStep = MAXSTEP;
	int crossBound = 0;

	float step2bound = phantom->step2VoxBoundary(pos, dir, crossBound);
	energyTransfer = energyInOneStep(vox , rspw, mspr, thisMaxStep);
	if(energyTransfer > MAXENERGYRATIO*energy)
		thisMaxStep = getMaxStep(vox, rspw, mspr);

	// get linear macro cross section to sample a step
	energyTransfer = energyInOneStep(vox, rspw, mspr, thisMaxStep);
	float sigma1 = totalLinearSigma(vox, macroSigma, energy);
	float sigma2 = totalLinearSigma(vox, macroSigma, energy-energyTransfer);
	float sigma = sigma1 > sigma2 ? sigma1 : sigma2;

	// sample one step
	ranGenerator->resetDimensionality(1);
	float rand;
	ranGenerator->getUniforms(&rand);
	float sampledStep = -log(rand)/sigma;
	stepLength = sampledStep < thisMaxStep ? sampledStep : thisMaxStep;
	if(stepLength >= step2bound){
		ifHard = false;
		stepLength = step2bound;
	}
	else{
		ifHard = true;
		crossBound = 0;
	}


	// get energy transferred in this sampled step
	energyTransfer = energyInOneStep(vox, rspw, mspr, stepLength);
	energyTransfer += energyStraggling(ranGenerator, vox, stepLength);
	energyTransfer = energyTransfer > 0 ? energyTransfer : -energyTransfer;
	energyTransfer = energyTransfer < energy ? energyTransfer : energy;

	// deflection
	float variance = ES*ES*c*c*stepLength/(beta()*beta()*momentumSquare()*vox.radiationLength());
	float StdGauss;
	ranGenerator->resetDimensionality(1);
	ranGenerator->getGaussians(&StdGauss);
	float theta = StdGauss*sqrt(variance);
	ranGenerator->getUniforms(&rand);
	float phi = 2.0f*PI*rand;

	//score energy, update particle status
	Vector3 step = dir*stepLength;
	maxSigma = sigma;
	update(step, theta, phi, energyTransfer, crossBound);
}

float ParticleStatus::ionization(Random * ranGenerator, Voxel & vox){
	float rand[2];
	ranGenerator->resetDimensionality(2);

	float E = energy + m;
	float Te;
	while(true){
		ranGenerator->getUniforms(rand);
		Te = MINELECTRONENERGY*maxDeltaElectronEnergy()
				/((1-rand[0])*maxDeltaElectronEnergy()+rand[0]*MINELECTRONENERGY);
		if(rand[1] < 1-beta()*beta()*Te/maxDeltaElectronEnergy()+Te*Te/(2*E*E))
			break;
	}

	update(Vector3(0.0f,0.0f,0.0f), 0.0f, 0.0f, Te);
	vox.scoreElectron(Te);
	return Te;
}


