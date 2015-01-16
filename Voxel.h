/*
 * Voxel.h
 *
 *  Created on: Sep 23, 2014
 *      Author: S158879
 */

#ifndef Voxel_H_
#define Voxel_H_
#include "Vector3.h"
#include "MSPR.h"
#include "macros.h"
#include "DensCorrection.h"
#include <iostream>
#include <math.h>


class Voxel {
public:
	Voxel();
	Voxel(float huValue_);
	virtual ~Voxel();

	float giveDensity() const;
	float giveEDensity() const;
	int giveMatID() const;
	float giveEnergy() const;
	void initVoxel(float hu, const MSPR &, const DensCorrection & densCorrection);
	void score(float energy_);
	float radiationLength() const;
	void energy2Dose(const Vector3 & voxSize);
	void score2(float e);
	void scoreHeavy(float e);
	void scoreElectron(float e);
	float giveEnergy(float &, float &, float &, float &, float &) const;
	void scoreLET(float let);
	void scoreLET2(float let);
	void averageLET();
	float giveLET(float & l1, float & l2) const;
private:
	float huValue;
	float density, edensity;
	float ct2den() const;
	float ct2eden() const;
	void setMaterial(const MSPR &);
	float energy;
	int materialId;
	float energy2, energyHeavy;
	float LET, LET2;
	float energyE;
	float totalDose;
};

inline void Voxel::score(float energy_){
	energy += energy_;
}

inline float Voxel::giveEnergy() const{
	return energy;
}

inline void Voxel::score2(float e){
	energy2 += e;
}

inline void Voxel::scoreHeavy(float e){
	energyHeavy += e;
}

inline void Voxel::scoreElectron(float e){
	energyE += e;
}


inline float Voxel::giveEnergy(float & e1, float & e2, float & eHeavy, float & eElectron, float & total) const{
	e1 = energy;
	e2 = energy2;
	eHeavy = energyHeavy;
	eElectron = energyE;
	total = totalDose;

	return 0;
}

inline void Voxel::scoreLET(float let){
	LET += let;
}

inline void Voxel::scoreLET2(float let){
	LET2 += let;
}

inline float Voxel::giveLET(float & l1, float & l2) const{
	l1 = LET;
	l2 = LET2;
	return 0;
}





#endif /* Voxel_H_ */
