/*
 * Random.h
 *
 *  Created on: Oct 6, 2014
 *      Author: S158879
 */

#ifndef RANDOM_H_
#define RANDOM_H_
#include <cmath>

class Random {
public:
	Random();
	virtual ~Random();
	Random(unsigned long dimensionality_);
	inline unsigned long getDimensionality() const;

	virtual void getUniforms(float * variates)=0;
	virtual void getGaussians(float * variates);
	virtual void getExps(float * variates, float p);
	virtual void reset()=0;
	inline virtual void resetDimensionality(unsigned long newDimensionality);
protected:
	unsigned long dimensionality;
	float inverseCumulativeNormal(float u);
};

inline unsigned long Random::getDimensionality() const{
	return dimensionality;
}
inline void Random::resetDimensionality(unsigned long newDimensionality){
	dimensionality = newDimensionality;
}
#endif /* RANDOM_H_ */
