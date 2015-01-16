/*
 * Beam.cpp
 *
 *  Created on: Oct 2, 2014
 *      Author: S158879
 */

#include "Beam.h"
#include <fstream>

Beam::Beam(): width(0.0f), nParticles(0), particles(NULL), current(0), meanEnergy(0.0f){
	// TODO Auto-generated constructor stub

}

Beam::~Beam() {
	// TODO Auto-generated destructor stub
	if(current != 0){
		for(unsigned long i = 0; i < current; i++)
			delete particles[i];
	}

	delete [] particles;
}

bool Beam::popOne(ParticleStatus ** thisOne){
	if(current == 0)
		return false;
	current--;
	*thisOne = particles[current];
	particles[current] = NULL;
	return true;
}

bool Beam::isEmpty() const{
	if(current == 0)
		return true;
	return false;
}

void Beam::output() const{
	std::ofstream ofs("beamX", std::fstream::out|std::fstream::trunc);
	for(unsigned long i = 0; i < nParticles; i++){
		ofs <<	particles[i]->currentPos().X() << '\t';
	}
	ofs.close();
}

