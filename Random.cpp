/*
 * Random.cpp
 *
 *  Created on: Oct 6, 2014
 *      Author: S158879
 */

#include "Random.h"

Random::Random():dimensionality(0) {
	// TODO Auto-generated constructor stub

}

Random::Random(unsigned long dimensionality_):dimensionality(dimensionality_){

}

Random::~Random() {
	// TODO Auto-generated destructor stub
}

float Random::inverseCumulativeNormal(float u){
	static float a[4]={2.50662823884, -18.61500062529, 41.39119773534, -25.44106049637};
	static float b[4]={-8.47351093090, 23.08336743743, -21.06224101826, 3.13082909833};
	static float c[9]={0.3374754822726147, 0.9761690190917186, 0.1607979714918209, 0.0276438810333863, 0.0038405729373609, 0.0003951896511919,
			0.0000321767881768, 0.0000002888167364, 0.0000003960315187};
	float x=u-0.5;
	float r;
	if (std::abs(x)<0.42) // Beasley-Springer
	{
		float y=x*x;
		r=x*(((a[3]*y+a[2])*y+a[1])*y+a[0])/((((b[3]*y+b[2])*y+b[1])*y+b[0])*y+1.0);
	}
	else // Moro
	{
		r=u;
		if (x>0.0)
			r=1.0-u;
		r=log(-log(r));
		r=c[0]+r*(c[1]+r*(c[2]+r*(c[3]+r*(c[4]+r*(c[5]+
				r*(c[6]+r*(c[7]+r*c[8])))))));
		if (x<0.0)
			r=-r;
	}
	return r;
}

void Random::getGaussians(float * variates){
	getUniforms(variates);
	for(unsigned int i=0; i<dimensionality; i++){
		float x = *(variates+i);
		*(variates+i) = inverseCumulativeNormal(x);
	}
}

void Random::getExps(float * variates, float p){
	getUniforms(variates);
	for(unsigned int i=0; i<dimensionality; i++){
		*(variates+i) = -log(1-*(variates+i))/p;
	}
}
