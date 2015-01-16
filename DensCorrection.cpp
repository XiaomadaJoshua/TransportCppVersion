/*
 * DensCorrection.cpp
 *
 *  Created on: Sep 30, 2014
 *      Author: S158879
 */

#include "DensCorrection.h"

using namespace std;

DensCorrection::DensCorrection(): densCorrectionFactor(NULL), nFactor(0) {
	// TODO Auto-generated constructor stub

}

DensCorrection::~DensCorrection() {
	// TODO Auto-generated destructor stub
	delete [] densCorrectionFactor;
}

bool DensCorrection::setData(const char * filename){
	ifstream ifs;
	ifs.open(filename, ios::in);
	if(!ifs.is_open())
		return false;
	char temp[256];
	ifs.getline(temp, 256);
	ifs >> nFactor >> ws;
	ifs.getline(temp, 256);
	densCorrectionFactor = new float [nFactor];
	for(int i=0; i<nFactor; i++){
		ifs >> densCorrectionFactor[i];
		ifs.ignore(3);
	}

	if(!ifs.eof())
		return false;
	ifs.close();
	return true;
}

