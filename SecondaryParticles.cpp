/*
 * SecondaryParticles.cpp
 *
 *  Created on: Oct 13, 2014
 *      Author: S158879
 */

#include "SecondaryParticles.h"
#include <stdlib.h>

SecondaryParticles::SecondaryParticles(long buff_):buff(buff_), current(0), flag(false) {
	// TODO Auto-generated constructor stub
	particles = new ParticleStatus * [buff];
}

SecondaryParticles::~SecondaryParticles() {
	// TODO Auto-generated destructor stub
	if(current != 0)
		for(unsigned long i = 0; i < current; i++)
			delete particles[i];
	delete [] particles;
}

bool SecondaryParticles::isFull() const{
	return flag;
}

void SecondaryParticles::add(ParticleStatus * newGuy){
	if(isFull()){
		std::cout << "secondary particle overflow" << std::endl;
		exit(EXIT_FAILURE);
	}
	particles[current] = newGuy;
	newGuy = NULL;
	current++;
	if(current == buff)
		flag = true;
}

bool SecondaryParticles::pop(ParticleStatus ** thisGuy){
	if(current == 0)
		return false;
	current--;
	*thisGuy = particles[current];
	particles[current] = NULL;
	return true;
}

bool SecondaryParticles::isEmpty() const{
	if(current == 0)
		return true;
	return false;
}

