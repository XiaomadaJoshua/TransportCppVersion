/*
 * MersenneTwister.h
 *
 *  Created on: Oct 6, 2014
 *      Author: S158879
 */

#ifndef MERSENNETWISTER_H_
#define MERSENNETWISTER_H_
#include <math.h>
#include <stdlib.h>     // srand, rand
#include <time.h>

class MersenneTwister {
public:
	MersenneTwister();
	virtual ~MersenneTwister();

	float getARandNumber();
	void reset();
private:
	int iseed[2];
};

#endif /* MERSENNETWISTER_H_ */
