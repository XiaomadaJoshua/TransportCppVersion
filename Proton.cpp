/*
 * Proton.cpp
 *
 *  Created on: Oct 9, 2014
 *      Author: S158879
 */

#include "Proton.h"
#include "Macros.h"


Proton::Proton():event(imaginary){
	// TODO Auto-generated constructor stub
	ParticleStatus::m = MP;
	ParticleStatus::c = CP;
}

Proton::Proton(const ParticleStatus * p):ParticleStatus(p->currentPos(), p->currentDir(), p->currentEnergy(), MP, CP), event(imaginary){

}
Proton::Proton(const Proton & p):ParticleStatus(p.pos, p.dir, p.energy, MP, CP), event(imaginary){

}
Proton::Proton(Position p, Direction d, float e):ParticleStatus(p, d, e, MP, CP), event(imaginary){

}


void Proton::hardEvent(Random * ranGenerator, Voxel & vox, SecondaryParticles * secondary, const MacroCrossSection * macroSigma, float & Te){
	if(!ifHard)
		return;

	sampleEvent(ranGenerator, vox, macroSigma);
	if(event == imaginary)
		return;
	else if(event == ION){
		Te = ionization(ranGenerator, vox);
		return;
	}
	else if(event == PPE){
		PPElastic(ranGenerator, secondary);
		return;
	}
	else if(event == POE){
		POElastic(ranGenerator, vox);
		return;
	}
	else if(event == POI){
		POInelastic(ranGenerator, secondary, vox);
		return;
	}
}

void Proton::sampleEvent(Random * ranGenerator, const Voxel & vox, const MacroCrossSection * macroSigma){
	float sigIon = macroSigma->lookIon(energy);
	float sigPPE = macroSigma->lookPE(energy);
	float sigPOE = macroSigma->lookOE(energy);
	float sigPOI = macroSigma->lookOI(energy);
	sigIon *= vox.giveEDensity();
	sigPPE *= vox.giveDensity();
	sigPOE *= vox.giveDensity();
	sigPOI *= vox.giveDensity();

	float rand;
	ranGenerator->resetDimensionality(1);
	ranGenerator->getUniforms(&rand);
	rand *=maxSigma;
	if(rand < sigIon)
		event = ION;
	else if(rand < sigIon + sigPPE)
		event = PPE;
	else if(rand < sigIon + sigPPE + sigPOE)
		event = POE;
	else if(rand < sigIon + sigPPE + sigPOE + sigPOI)
		event = POI;
	else
		event = imaginary;
}


void Proton::PPElastic(Random * ranGenerator, SecondaryParticles * secondary){
	float rand[2];
	ranGenerator->resetDimensionality(2);
	ranGenerator->getUniforms(rand);

	// sample energy transferred
	float energyTransfer = 0.5*energy*rand[0];

	// sample incident proton deflection and update incident proton
	float temp1 = energy*(energy + 2.0*m);
	float temp2 = (energy - energyTransfer)*(energy - energyTransfer + 2.0*m);
	float costhe = (temp1 + temp2 - energyTransfer*(energyTransfer + 2.0*m))/2.0/sqrt(temp1*temp2);
	float theta = acos(costhe);
	float phi = 2*PI*rand[1];
	update(Vector3(0,0,0), theta, phi, energyTransfer);

	// compute angular deflection of recoil proton and store in secondary particle container
	temp1 = (sqrt(temp1) - sqrt(temp2)*(costhe))/sqrt(energyTransfer*(energyTransfer + 2.0f*m));
	Proton * p = new Proton(pos, dir, energyTransfer);
	p->update(Vector3(0,0,0), acos(temp1), phi+PI, 0);
	secondary->add(p);
}

void Proton::POElastic(Random * ranGenerator, Voxel & vox){
	// sample energy transferred to oxygen
	float meanEnergy = 0.65f*exp(-0.0013f*energy) - 0.71f*exp(-0.0177f*energy);
	meanEnergy = meanEnergy < 1.0f ? 1.0f : meanEnergy;
	ranGenerator->resetDimensionality(1);
	float energyTransfer;
	do{
	ranGenerator->getExps(&energyTransfer, 1.0f/meanEnergy);
	}while(energyTransfer > maxOxygenEnergy());

	// calculate theta, sample phi
	float temp1 = energy*(energy + 2.0f*m);
	float temp2 = (energy- energyTransfer)*(energy - energyTransfer + 2.0f*m);
	float costhe = (temp1 + temp2 - energyTransfer*(energyTransfer + 2.0f*MO))/ 2.0f /sqrt(temp1*temp2 );
	float theta = acos(costhe);
	float phi;
	ranGenerator->getUniforms(&phi);
	phi = phi*2.0f*PI;

	update(Vector3(0.0f,0.0f,0.0f), theta, phi, energyTransfer);
	vox.scoreHeavy(energyTransfer);
}

void Proton::POInelastic(Random * ranGenerator, SecondaryParticles * secondary, Voxel & vox){
	ranGenerator->resetDimensionality(4);
	float rand[4];

	float bindEnergy = EBIND;
	float minEnergy = EMINPOI;
	float remainEnergy = energy;
	float energyDeposit = 0.0f;

	// simulate POI
	while(true){
		if(remainEnergy - bindEnergy < minEnergy){
			energyDeposit += remainEnergy;
			break;
		}

		energyDeposit += bindEnergy;
		remainEnergy -= bindEnergy;

		ranGenerator->getUniforms(rand);
		float energy2SecondParticle = rand[0]*(remainEnergy - minEnergy) + minEnergy;
		remainEnergy -= energy2SecondParticle;

		if(rand[3] < 0.650f){ //proton
			Proton * p = new Proton(pos, dir, energy2SecondParticle);
			float costhe = 1.0f - 2.0f*energy2SecondParticle*rand[1]/energy;
//			float costhe = (2.0f - 2.0f*energy2SecondParticle/remainEnergy)*rand[1] + 2.0f*energy2SecondParticle/remainEnergy - 1.0f;

			if(costhe > 1 || costhe < -1)
				cout << "nan from POI\n";

			float theta = acos(costhe);
			float phi = 2.0f*PI*rand[2];
			p->update(Vector3(0.0f,0.0f,0.0f), theta, phi, 0.0f);
			secondary->add(p);
		}

		else if(rand[3] < 0.67450f)//short range energy
			energyDeposit += energy2SecondParticle;

		bindEnergy *= 0.85f;
	}
	vox.scoreHeavy(energyDeposit);
	update(Vector3(0.0f,0.0f,0.0f), 0.0f, 0.0f, energy);
}

