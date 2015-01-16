/*
 * MSPR.cpp
 *
 *  Created on: Oct 2, 2014
 *      Author: S158879
 */

#include "MSPR.h"

MSPR::MSPR():nMaterial(0), nEnergy(0), startingHU(NULL), minEnergy(0), energyInterval(0), fs(NULL){
	// TODO Auto-generated constructor stub

}

MSPR::~MSPR() {
	// TODO Auto-generated destructor stub
	delete [] startingHU;
	delete [] fs;
}

bool MSPR::setData(const char * filename){
	ifstream ifs;
	ifs.open(filename, ios::in);
	if(!ifs.is_open())
		return false;
	char temp[256];
	ifs.getline(temp, 256);
	ifs.getline(temp, 256);
	ifs >> nMaterial  >> ws;
	fs = new float * [nMaterial];
	startingHU = new float[nMaterial];
	for(unsigned int j=0; j<nMaterial; j++){
		ifs.getline(temp, 256);
		int matID = j;
		float nominalDens, energy;
		ifs >> matID >> startingHU[matID] >> nominalDens >> nEnergy >> minEnergy >> energyInterval >> ws;
		ifs.getline(temp, 256);
		fs[j] = new float[nEnergy];
		for(unsigned int i=0; i<nEnergy; i++)
			ifs >> energy >> fs[j][i] >> ws;
	}
	if(!ifs.eof())
		return false;
	ifs.close();
	return true;
}

const float * MSPR::lookStartingHU() const{
	return startingHU;
}

unsigned int MSPR::lookNMaterial() const{
	return nMaterial;
}

float MSPR::lookMSPR(int matID, float energy) const{
	if(energy < minEnergy)
		return fs[matID][0];
	else if(energy > minEnergy + nEnergy*energyInterval)
		return fs[matID][nEnergy - 1];
	int index = static_cast<int>((energy - minEnergy)/energyInterval);
	float e1 = minEnergy + index*energyInterval;
	float e2 = e1 + energyInterval;
	float mspr1 = fs[matID][index];
	float mspr2 = fs[matID][index+1];
	return mspr1 + (energy - e1)*(mspr2 - mspr1)/(e2 - e1);
}

