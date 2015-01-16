/*
 * SecondaryParticles.h
 *
 *  Created on: Oct 13, 2014
 *      Author: S158879
 */

#ifndef SECONDARYPARTICLES_H_
#define SECONDARYPARTICLES_H_
#include "ParticleStatus.h"
#include <iostream>

class SecondaryParticles {
public:
	SecondaryParticles(long buff_);
	virtual ~SecondaryParticles();
	void add(ParticleStatus * newGuy);
	bool pop(ParticleStatus ** thisGuy);
	bool isFull() const;
	bool isEmpty() const;
private:
	ParticleStatus ** particles;
	unsigned long buff;
	unsigned long current;
	bool flag;
};

#endif /* SECONDARYPARTICLES_H_ */
