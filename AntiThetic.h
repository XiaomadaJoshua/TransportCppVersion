/*
 * AntiThetic.h
 *
 *  Created on: Oct 6, 2014
 *      Author: S158879
 */

#ifndef ANTITHETIC_H_
#define ANTITHETIC_H_
#include "Random.h"

class AntiThetic : public Random {
public:
	AntiThetic();
	AntiThetic(Random * innerGenerator_);
	virtual ~AntiThetic();

	virtual void getUniforms(float * variates);
	virtual void resetDimensionality(unsigned long newDimensionality);
	virtual void reset();
private:
	Random * innerGenerator;
	bool oddEven;
	float * nextVariates;
};

#endif /* ANTITHETIC_H_ */
