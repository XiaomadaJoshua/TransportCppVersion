/*
 * Carbon.cpp
 *
 *  Created on: Nov 24, 2014
 *      Author: S158879
 */

#include "Carbon.h"


Carbon::Carbon():event(imaginary){
	// TODO Auto-generated constructor stub
	ParticleStatus::m = MC;
	ParticleStatus::c = CC;
}

Carbon::~Carbon() {
	// TODO Auto-generated destructor stub
}

Carbon::Carbon(Position p, Direction d, float e):ParticleStatus(p, d, e, MC, CC), event(imaginary){

}

Carbon::Carbon(const ParticleStatus * c):ParticleStatus(c->currentPos(), c->currentDir(), c->currentEnergy(), MC, CC), event(imaginary){

}

Carbon::Carbon(const Carbon & c):ParticleStatus(c.pos, c.dir, c.energy, MC, CC), event(imaginary){

}




float Carbon::energyInOneStep(const Voxel & vox, const RSPW * resStpPowWater, const MSPR * massStpPowRatio, float stepLength) const{
	//calculate equivalent step in water
	float stepInWater;
//	float density = vox.giveDensity();
//	float mspr = massStpPowRatio->lookMSPR(vox.giveMatID(), energy);
//	stepInWater = mspr*stepLength*density/WATERDENSITY;
	stepInWater = stepLength;

	//calculate energy transfer
	float rspw = resStpPowWater->lookLw(energy);
	float b = resStpPowWater->lookB(energy);

	float de1 = stepInWater*rspw;
	float temp = energy/m;
	float eps = de1/energy;
	return de1*(1.0f + eps/(1.0f+temp)/(2.0f+temp) + eps*eps*(2.0f+2.0f*temp+temp*temp)/(1.0f+temp)/(1.0f+temp)/(2.0f+temp)/(2.0f+temp)
			- b*eps*(0.5f+2.0f*eps/3.0f/(1.0f+temp)/(2.0f+temp) + (1.0f-b)*eps/6.0f) );

}

float Carbon::getMaxStep(const Voxel & vox, const RSPW * resStpWater, const MSPR * massStpPowRatio) const{
	float stepInWater = MAXENERGYRATIO*energy/resStpWater->lookLw(energy - 0.5f*MAXENERGYRATIO*energy);
//	return stepInWater*WATERDENSITY/vox.giveDensity()/massStpPowRatio->lookMSPR(vox.giveMatID(), energy - 0.5f*MAXENERGYRATIO*energy);
	return stepInWater;
}
void Carbon::hardEvent(Random * ranGenerator, Voxel & vox, SecondaryParticles * secondary, const MacroCrossSection * macroSigma, float & Te){
	if(!ifHard)
		return;

	sampleEvent(ranGenerator, vox, macroSigma);
	if(event == imaginary)
		return;
	else if(event == ION)
		Te = ionization(ranGenerator, vox);
	else
		cout << "hard event bug" << endl;
}

void Carbon::sampleEvent(Random * ranGenerator, const Voxel & vox, const MacroCrossSection * macroSigma){
	float sigIon = macroSigma->lookIon(energy);
	sigIon *= vox.giveEDensity();

	float rand;
	ranGenerator->resetDimensionality(1);
	ranGenerator->getUniforms(&rand);
	rand *=maxSigma;

	if(rand < sigIon)
		event = ION;
	else
		event = imaginary;
}

