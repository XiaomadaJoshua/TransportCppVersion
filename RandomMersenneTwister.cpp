/*
 * RandomMersenneTwister.cpp
 *
 *  Created on: Oct 6, 2014
 *      Author: S158879
 */

#include "RandomMersenneTwister.h"

RandomMersenneTwister::RandomMersenneTwister() {
	// TODO Auto-generated constructor stub

}

RandomMersenneTwister::~RandomMersenneTwister() {
	// TODO Auto-generated destructor stub
}

void RandomMersenneTwister::getUniforms(float * variates){
	for(unsigned int i=0; i<dimensionality; i++)
		*(variates+i) = innerGenerator.getARandNumber();
}

void RandomMersenneTwister::reset(){
	innerGenerator.reset();
}

void RandomMersenneTwister::resetDimensionality(unsigned long newDimensionality){
	Random::resetDimensionality(newDimensionality);
}







