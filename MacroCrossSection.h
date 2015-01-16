/*
 * MacroCrossSection.h
 *
 *  Created on: Oct 1, 2014
 *      Author: S158879
 */

#ifndef MACROCROSSSECTION_H_
#define MACROCROSSSECTION_H_
#include "Data.h"
using namespace std;

class MacroCrossSection : public Data{
public:
	MacroCrossSection();
	virtual ~MacroCrossSection();
	virtual bool setData(const char *);

	float lookIon(float e) const;
	float lookPE(float e) const;
	float lookOE(float e) const;
	float lookOI(float e) const;
private:
	float minElectronEnergy, minProtonEnergy, maxProtonEnergy, energyInterval;
	unsigned int nData;
	struct MCS{
		float energy, sigIon, sigPPE, sigPOE, sigPOI;
	};
	MCS * macroSigma;
};

#endif /* MACROCROSSSECTION_H_ */
