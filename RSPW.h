/*
 * RSPW.h
 *
 *  Created on: Oct 1, 2014
 *      Author: S158879
 */

#ifndef RSPW_H_
#define RSPW_H_
#include "Data.h"
using namespace std;

class RSPW : public Data {
public:
	RSPW();
	virtual ~RSPW();
	virtual bool setData(const char *);

	float lookLw(float e) const;
	float lookB(float e) const;
private:
	float minElectronEnergy, minProtonEnergy, maxProtonEnergy, energyInterval;
	unsigned int nData;
	struct rspw{
		float energy, Lw, b;
	};
	rspw * data;
};

#endif /* RSPW_H_ */
