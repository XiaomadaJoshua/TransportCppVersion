/*
 * DensCorrection.h
 *
 *  Created on: Sep 30, 2014
 *      Author: S158879
 */

#ifndef DENSCORRECTION_H_
#define DENSCORRECTION_H_
#include "Data.h"

class DensCorrection : public Data {
public:
	DensCorrection();
	virtual ~DensCorrection();
	virtual bool setData(const char * filename);
	float operator [] (int i) const;
	int size() const;
private:
	float * densCorrectionFactor;
	int nFactor;
};

inline float DensCorrection::operator [] (int i) const{
	return densCorrectionFactor[i];
}

inline int DensCorrection::size() const{
	return nFactor;
}

#endif /* DENSCORRECTION_H_ */
