/*
 * MacroCrossSection.cpp
 *
 *  Created on: Oct 1, 2014
 *      Author: S158879
 */

#include "MacroCrossSection.h"
#include <stdlib.h>
#include <iostream>
using namespace std;

MacroCrossSection::MacroCrossSection():
minElectronEnergy(0), minProtonEnergy(0), maxProtonEnergy(0), energyInterval(0), nData(0), macroSigma(NULL){
	// TODO Auto-generated constructor stub

}

MacroCrossSection::~MacroCrossSection() {
	// TODO Auto-generated destructor stub
	delete [] macroSigma;
}

bool MacroCrossSection::setData(const char * filename){
	ifstream ifs;
	ifs.open(filename, ios::in);
	if(!ifs.is_open())
		return false;
	char temp[256];
	ifs.getline(temp, 256);
	ifs.getline(temp, 256);
	ifs >> minElectronEnergy >> minProtonEnergy >> maxProtonEnergy >> energyInterval >> nData >> ws;
	macroSigma = new MCS[nData];
	ifs.getline(temp, 256);
	for(unsigned int i=0; i<nData; i++)
		ifs >> macroSigma[i].energy >> macroSigma[i].sigIon >> macroSigma[i].sigPPE >> macroSigma[i].sigPOE >> macroSigma[i].sigPOI >> ws;

	if(!ifs.eof())
		return false;
	ifs.close();
	return true;
}

float MacroCrossSection::lookIon(float e) const{
	if(e < minProtonEnergy)
		return macroSigma[0].sigIon;
	if(e > maxProtonEnergy)
		return macroSigma[nData-1].sigIon;
	int floor((e-minProtonEnergy)/energyInterval), ceiling(1 + (e-minProtonEnergy)/energyInterval);
	float sig1 = macroSigma[floor].sigIon;
	float sig2 = macroSigma[ceiling].sigIon;
	float e1 = macroSigma[floor].energy;
	float e2 = macroSigma[ceiling].energy;
	return sig1 + (e - e1)*(sig2 - sig1)/(e2 - e1);
}

float MacroCrossSection::lookPE(float e) const{
	if(e < minProtonEnergy)
		return macroSigma[0].sigPPE;
	if(e > maxProtonEnergy)
		return macroSigma[nData-1].sigPPE;
	int floor((e-minProtonEnergy)/energyInterval), ceiling(1 + (e-minProtonEnergy)/energyInterval);
	float sig1 = macroSigma[floor].sigPPE;
	float sig2 = macroSigma[ceiling].sigPPE;
	float e1 = macroSigma[floor].energy;
	float e2 = macroSigma[ceiling].energy;
	return sig1 + (e - e1)*(sig2 - sig1)/(e2 - e1);
}

float MacroCrossSection::lookOE(float e) const{
	if(e < minProtonEnergy)
		return macroSigma[0].sigPOE;
	if(e > maxProtonEnergy)
		return macroSigma[nData-1].sigPOE;
	int floor((e-minProtonEnergy)/energyInterval), ceiling(1 + (e-minProtonEnergy)/energyInterval);
	float sig1 = macroSigma[floor].sigPOE;
	float sig2 = macroSigma[ceiling].sigPOE;
	float e1 = macroSigma[floor].energy;
	float e2 = macroSigma[ceiling].energy;
	return sig1 + (e - e1)*(sig2 - sig1)/(e2 - e1);
}

float MacroCrossSection::lookOI(float e) const{
	if(e < minProtonEnergy)
		return macroSigma[0].sigPOI;
	if(e > maxProtonEnergy)
		return macroSigma[nData-1].sigPOI;
	int floor((e-minProtonEnergy)/energyInterval), ceiling(1 + (e-minProtonEnergy)/energyInterval);
	float sig1 = macroSigma[floor].sigPOI;
	float sig2 = macroSigma[ceiling].sigPOI;
	float e1 = macroSigma[floor].energy;
	float e2 = macroSigma[ceiling].energy;
	return sig1 + (e - e1)*(sig2 - sig1)/(e2 - e1);
}


