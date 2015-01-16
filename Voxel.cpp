/*
 * Voxel.cpp
 *
 *  Created on: Sep 23, 2014
 *      Author: S158879
 */

#include "Voxel.h"
#include <stdlib.h>
#include "Macros.h"

Voxel::Voxel():huValue(0.0f), density(0.0f), edensity(0.0f), energy(0.0f), materialId(-1),
		energy2(0.0f), energyHeavy(0.0f), LET(0.0f), LET2(0.0f), energyE(0.0f), totalDose(0.0f){
	// TODO Auto-generated constructor stub
}

Voxel::Voxel(float ct):huValue(ct), energy(0.0f), materialId(-1), energy2(0.0f), energyHeavy(0.0f),
		LET(0.0f), LET2(0.0f), energyE(0.0f), totalDose(0.0f){
	density = ct2den();
	edensity = ct2eden();
}

Voxel::~Voxel() {
	// TODO Auto-generated destructor stub
}

float Voxel::ct2den() const{
//	convert HU to dens, in g/cm^3

float temp = 0.0f;
//	MGH calibration curve
	if(huValue >= -1000 && huValue < -88)
		temp = 0.00121f + 0.001029700665188f*(1000.0f + huValue);
	else if(huValue >= -88 && huValue < 15)
	    temp = 1.018f + 0.000893f*huValue;
	else if(huValue >= 15 && huValue < 23)
	    temp = 1.03f;
	else if(huValue >= 23 && huValue < 101)
	    temp = 1.003f + 0.001169f*huValue;
	else if(huValue >= 101 && huValue < 2001)
	    temp = 1.017f + 0.000592f*huValue;
	else if(huValue >= 2001 && huValue < 2995)
	    temp = 2.201f + 0.0005f*(-2000.0f + huValue);
	else
		temp = 4.54f;
	return temp;
}

float Voxel::ct2eden() const{
	//	convert HU to electron dens, in unit of ne_w=3.34e23/cm^3
    //  MGH CT convertion curve
	int Nele = 13;
	float Z[13] = {1.0f, 6.0f, 7.0f, 8.0f, 12.0f, 15.0f, 16.0f, 17.0f, 18.0f, 20.0f, 11.0f, 19.0f, 22.0f};
	float A[13] = {1.00794, 12.0107, 14.0067, 15.9994, 24.305, 30.9737, 32.065, 35.453, 39.948, 40.078, 22.9898, 39.0983, 47.867};

	float ZperA[13];
	for(int iele = 0; iele < Nele; iele++){
        ZperA[iele] = Z[iele]/A[iele];
	}

	float composition[25][13] =
	{{0.0  , 0.0  , 0.755, 0.232, 0.0  , 0.0  , 0.0  , 0.0  , 0.013, 0.0  , 0.0  , 0.0  , 0.0},
			{0.103, 0.105, 0.031, 0.749, 0.0  , 0.002, 0.003, 0.003, 0.0  , 0.0  , 0.002, 0.002, 0.0},
			{0.116, 0.681, 0.002, 0.198, 0.0  , 0.0  , 0.001, 0.001, 0.0  , 0.0  , 0.001, 0.0  , 0.0},
	        {0.113, 0.567, 0.009, 0.308, 0.0  , 0.0  , 0.001, 0.001, 0.0  , 0.0  , 0.001, 0.0  , 0.0},
	        {0.110, 0.458, 0.015, 0.411, 0.0  , 0.001, 0.002, 0.002, 0.0  , 0.0  , 0.001, 0.0  , 0.0},
	        {0.108, 0.356, 0.022, 0.509, 0.0  , 0.001, 0.002, 0.002, 0.0  , 0.0  , 0.0  , 0.0  , 0.0},
	        {0.106, 0.284, 0.026, 0.578, 0.0  , 0.001, 0.002, 0.002, 0.0  , 0.0  , 0.0  , 0.001, 0.0},
	        {0.103, 0.134, 0.030, 0.723, 0.0  , 0.002, 0.002, 0.002, 0.0  , 0.0  , 0.002, 0.002, 0.0},
	        {0.094, 0.207, 0.062, 0.622, 0.0  , 0.0  , 0.006, 0.003, 0.0  , 0.0  , 0.006, 0.0  , 0.0},
	        {0.095, 0.455, 0.025, 0.355, 0.0  , 0.021, 0.001, 0.001, 0.0  , 0.045, 0.001, 0.001, 0.0},
	        {0.089, 0.423, 0.027, 0.363, 0.0  , 0.030, 0.001, 0.001, 0.0  , 0.064, 0.001, 0.001, 0.0},
	        {0.082, 0.391, 0.029, 0.372, 0.0  , 0.039, 0.001, 0.001, 0.0  , 0.083, 0.001, 0.001, 0.0},
	        {0.076, 0.361, 0.030, 0.380, 0.001, 0.047, 0.002, 0.001, 0.0  , 0.101, 0.001, 0.0  , 0.0},
	        {0.071, 0.335, 0.032, 0.387, 0.001, 0.054, 0.002, 0.0  , 0.0  , 0.117, 0.001, 0.0  , 0.0},
	        {0.066, 0.310, 0.033, 0.394, 0.001, 0.061, 0.002, 0.0  , 0.0  , 0.132, 0.001, 0.0  , 0.0},
	        {0.061, 0.287, 0.035, 0.400, 0.001, 0.067, 0.002, 0.0  , 0.0  , 0.146, 0.001, 0.0  , 0.0},
	        {0.056, 0.265, 0.036, 0.405, 0.002, 0.073, 0.003, 0.0  , 0.0  , 0.159, 0.001, 0.0  , 0.0},
	        {0.052, 0.246, 0.037, 0.411, 0.002, 0.078, 0.003, 0.0  , 0.0  , 0.170, 0.001, 0.0  , 0.0},
	        {0.049, 0.227, 0.038, 0.416, 0.002, 0.083, 0.003, 0.0  , 0.0  , 0.181, 0.001, 0.0  , 0.0},
	        {0.045, 0.210, 0.039, 0.420, 0.002, 0.088, 0.003, 0.0  , 0.0  , 0.192, 0.001, 0.0  , 0.0},
	        {0.042, 0.194, 0.040, 0.425, 0.002, 0.092, 0.003, 0.0  , 0.0  , 0.201, 0.001, 0.0  , 0.0},
	        {0.039, 0.179, 0.041, 0.429, 0.002, 0.096, 0.003, 0.0  , 0.0  , 0.210, 0.001, 0.0  , 0.0},
	        {0.036, 0.165, 0.042, 0.432, 0.002, 0.100, 0.003, 0.0  , 0.0  , 0.219, 0.001, 0.0  , 0.0},
	        {0.034, 0.155, 0.042, 0.435, 0.002, 0.103, 0.003, 0.0  , 0.0  , 0.225, 0.001, 0.0  , 0.0},
	        {0.0  , 0.0  , 0.0  , 0.0  , 0.0  , 0.0  , 0.0  , 0.0  , 0.0  , 0.0  , 0.0  , 0.0  , 1.0}};

	float edensity = 0;
	for(int iele = 0; iele < Nele; iele++){
        edensity += composition[materialId][iele]*ZperA[iele];
	}
	edensity *= 6.02214129e23*density/3.342774e23;
	return edensity;
}

void Voxel::initVoxel(float hu, const MSPR & mspr, const DensCorrection & densCorrection){
	huValue = hu;
	energy = 0.0f;
	energy2 = 0.0f;
	energyHeavy = 0.0f;
	LET = 0.0f;
	LET2 = 0.0f;
	energyE = 0.0f;
	density = ct2den();
	int ind = huValue + 1000;
	ind = ind > densCorrection.size() - 1 ? densCorrection.size() - 1 : ind;
	density *= densCorrection[ind];
	setMaterial(mspr);
	edensity = ct2eden();
}

float Voxel::radiationLength() const
//	calculate the radiation length ratio \rho_wX_w/(\rhoX_0(\rho)
{
	float ratio;
	if(density >= 0.9)
	{
		ratio = 1.19f + 0.44f*log(density-0.44f);
	}
	else if (density >= 0.26)
	{
		ratio = 1.0446f -0.2180f*density;
	}
	else
	{
		ratio = 0.9857f + 0.0085f*density;
	}
	return WATERDENSITY*XW/(ratio*density);
}

float Voxel::giveDensity() const{
	return density;
}

float Voxel::giveEDensity() const{
	return edensity;
}

int Voxel::giveMatID() const{
	return materialId;
}

void Voxel::setMaterial(const MSPR & mspr){
	if(huValue<mspr.lookStartingHU()[0]){
		std::cout << "invalid HU" << std::endl;
		exit (EXIT_FAILURE);
	}
	for(unsigned int i=0; i<mspr.lookNMaterial()-1; i++)
		if(huValue >= mspr.lookStartingHU()[i] && huValue < mspr.lookStartingHU()[i+1]){
			materialId = i;
			return;
		}
	materialId = mspr.lookNMaterial() - 1;
}

void Voxel::energy2Dose(const Vector3 & voxSize){
	float mass = density*voxSize.X()*voxSize.Y()*voxSize.Z();
	energy = energy/mass;
	energy2 = energy2/mass;
	energyE = energyE/mass;
	energyHeavy = energyHeavy/mass;
	totalDose = energy + energy2 + energyE + energyHeavy;
}

void Voxel::averageLET(){
	if(LET > 0){
		if((energy + energy2 + energyE) == 0)
			cout << "let" << endl;
		else
			LET = LET/(energy + energy2 + energyE)/density;
	}

	if(LET2 > 0){
		if((energy + energy2 + energyE) == 0)
			cout << "let2" << endl;
		else
			LET2 = LET2/(energy + energy2 + energyE)/density;
	}
	if(LET > 1000 || LET2 > 1000)
		cout << "oo" << endl;
}

