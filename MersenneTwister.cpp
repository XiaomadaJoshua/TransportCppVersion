/*
 * MersenneTwister.cpp
 *
 *  Created on: Oct 6, 2014
 *      Author: S158879
 */

#include "MersenneTwister.h"
#include <fstream>

MersenneTwister::MersenneTwister() {
	// TODO Auto-generated constructor stub
	srand((unsigned int)time(NULL));
	iseed[0] = rand();
	iseed[1] = rand();
//	iseed[0] = 1;
//	iseed[1] = 2;
}

MersenneTwister::~MersenneTwister() {
	// TODO Auto-generated destructor stub
}

float MersenneTwister::getARandNumber(){
	int I1 = iseed[0] / 53668;
	iseed[0] = 40014 * (iseed[0] - I1 * 53668) - I1 * 12211;
	if (iseed[0] < 0) iseed[0] = iseed[0] + 2147483563;

	int I2 = iseed[1] / 52774;
	iseed[1] = 40692 * (iseed[1] - I2 * 52774) - I2 * 3791;
	if (iseed[1] < 0) iseed[1] = iseed[1] + 2147483399;

	int IZ = iseed[0] - iseed[1];
	if (IZ < 1) IZ = IZ + 2147483562;

	return float(IZ*4.656612873077392578125e-10);
}

void MersenneTwister::reset(){
	srand((unsigned int)time(NULL));
	iseed[0] = rand();
	iseed[1] = rand();
}



