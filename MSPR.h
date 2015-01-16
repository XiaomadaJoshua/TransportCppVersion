/*
 * MSPR.h
 *
 *  Created on: Oct 2, 2014
 *      Author: S158879
 */

#ifndef MSPR_H_
#define MSPR_H_
#include "Data.h"
#include <fstream>
#include "macros.h"
using namespace std;

class MSPR : public Data {
public:
	MSPR();
	virtual ~MSPR();
	virtual bool setData(const char *);

	const float * lookStartingHU() const;
	unsigned int lookNMaterial() const;
	float lookMSPR(int matID, float energy) const;
private:
	unsigned int nMaterial, nEnergy;
	float * startingHU;
	float minEnergy, energyInterval;
	float ** fs;
};

#endif /* MSPR_H_ */
