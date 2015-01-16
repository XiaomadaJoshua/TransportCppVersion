/*
 * AntiThetic.cpp
 *
 *  Created on: Oct 6, 2014
 *      Author: S158879
 */

#include "AntiThetic.h"
#include <stddef.h>

AntiThetic::AntiThetic():innerGenerator(NULL), oddEven(true), nextVariates(NULL) {
	// TODO Auto-generated constructor stub

}

AntiThetic::AntiThetic(Random * innerGenerator_):Random(innerGenerator_->getDimensionality()), innerGenerator(innerGenerator_), oddEven(true){
	nextVariates = new float[dimensionality];
}

AntiThetic::~AntiThetic() {
	// TODO Auto-generated destructor stub
	delete[] nextVariates;
	innerGenerator = NULL;
}

void AntiThetic::getUniforms(float * variates){
	if(oddEven==true){
		innerGenerator->getUniforms(variates);
		for(unsigned int i=0; i<dimensionality; i++)
			*(nextVariates+i) = 1.0 - *(variates+i);
		oddEven = false;
	}
	else{
		for(unsigned int i=0; i<dimensionality; i++)
			*(variates+i) = *(nextVariates+i);
		oddEven = true;
	}
}

void AntiThetic::resetDimensionality(unsigned long newDimensionality){
	Random::resetDimensionality(newDimensionality);
	innerGenerator->resetDimensionality(newDimensionality);
	float * previous = nextVariates;
	nextVariates = new float[newDimensionality];
	delete [] previous;
	oddEven = true;
}

void AntiThetic::reset(){
	innerGenerator->reset();
	oddEven = true;
}







