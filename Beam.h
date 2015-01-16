/*
 * Beam.h
 *
 *  Created on: Oct 2, 2014
 *      Author: S158879
 */

#ifndef BEAM_H_
#define BEAM_H_
#include "ParticleStatus.h"
#include "Vector3.h"
#include "Random.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

class Beam {
public:
	Beam();
	virtual ~Beam();
	template <typename T> void initBeam(float width_, Position & sourceCenter_, Position & isoCenter, unsigned long nParticles_, Random * randGenerator, float energy_);
	template <typename T> void initBeam(Beam & beam, Random * randGenerator);
	bool popOne(ParticleStatus **);
	bool isEmpty() const;

	void output() const;

private:
	float width;
	unsigned long nParticles;
	Position sourceCenter, isoCenter;
	Direction beamAngle;
	ParticleStatus ** particles;
	unsigned long current;
	float meanEnergy;
};

template <typename T> void Beam::initBeam(float width_, Position & sourceCenter_, Position & isoCenter_, unsigned long nParticles_, Random * randGenerator, float energy_){
//	if(current != 0){
//		std::cout << "beam already initialized" << std::endl;
//		exit(EXIT_FAILURE);
//	}
	width = width_;
	nParticles = nParticles_;
	current = nParticles_;
	randGenerator->resetDimensionality(nParticles);
	meanEnergy = energy_;
	particles = new ParticleStatus * [nParticles];
	sourceCenter = sourceCenter_;
	isoCenter = isoCenter_;
	float sad = (isoCenter - sourceCenter).getLength();
	beamAngle = isoCenter - sourceCenter;

	float * particlePositionX = new float[nParticles];
	float * particlePositionZ = new float[nParticles];
	randGenerator->getUniforms(particlePositionX);
	randGenerator->getUniforms(particlePositionZ);
	for(unsigned long i=0; i<nParticles; i++){
		particlePositionX[i] = particlePositionX[i]*width - width/2;
		particlePositionZ[i] = particlePositionZ[i]*width - width/2;
		Position particlePosition(particlePositionX[i], -sad, particlePositionZ[i]);
		particles[i] = new T;
		particles[i]->setStatus(particlePosition, beamAngle, energy_);
	}
	delete [] particlePositionX;
	delete [] particlePositionZ;
}

template <typename T> void Beam::initBeam(Beam & beam, Random * randGenerator){
//	if(current != 0){
//		std::cout << "beam already initialized" << std::endl;
//		exit(EXIT_FAILURE);
//	}
	width = beam.width;
	nParticles = beam.nParticles;
	current = nParticles;
	randGenerator->resetDimensionality(nParticles);
	meanEnergy = beam.meanEnergy;
	particles = new ParticleStatus * [nParticles];
	sourceCenter = beam.sourceCenter;
	isoCenter = beam.isoCenter;
	float sad = (isoCenter - sourceCenter).getLength();
	beamAngle = isoCenter - sourceCenter;

	float * particlePositionX = new float[nParticles];
	float * particlePositionZ = new float[nParticles];
	randGenerator->getUniforms(particlePositionX);
	randGenerator->getUniforms(particlePositionZ);
	for(unsigned long i=0; i<nParticles; i++){
		particlePositionX[i] = particlePositionX[i]*width - width/2;
		particlePositionZ[i] = particlePositionZ[i]*width - width/2;
		Position particlePosition(particlePositionX[i], -sad, particlePositionZ[i]);
		particles[i] = new T;
		particles[i]->setStatus(particlePosition, beamAngle, meanEnergy);
	}
	delete [] particlePositionX;
	delete [] particlePositionZ;
}




#endif /* BEAM_H_ */
