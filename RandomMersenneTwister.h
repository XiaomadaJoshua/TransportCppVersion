/*
 * RandomMersenneTwister.h
 *
 *  Created on: Oct 6, 2014
 *      Author: S158879
 */

#ifndef RANDOMMERSENNETWISTER_H_
#define RANDOMMERSENNETWISTER_H_
#include "Random.h"
#include "MersenneTwister.h"

class RandomMersenneTwister : public Random {
public:
	RandomMersenneTwister();
	virtual ~RandomMersenneTwister();

	virtual void reset();
	virtual void resetDimensionality(unsigned long newDimensionality);
	virtual void getUniforms(float * variates);
private:
	MersenneTwister innerGenerator;
};

#endif /* RANDOMMERSENNETWISTER_H_ */
