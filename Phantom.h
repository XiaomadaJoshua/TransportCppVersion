/*
 * Phantom.h
 *
 *  Created on: Sep 29, 2014
 *      Author: S158879
 */

#ifndef PHANTOM_H_
#define PHANTOM_H_
#include "Voxel.h"
#include "Position.h"
#include "Direction.h"
#include "Macros.h"

class MSPR;

class Phantom {
public:
	Phantom();
	Phantom(const Vector3 & voxSize_, int width_, int hight_, int depth_, const Vector3 & iso);
	void initPhantom(Vector3 & voxSize_, int width_, int hight_, int depth_, const Vector3 & iso, const MSPR & mspr, const DensCorrection & densCorrection);
	virtual ~Phantom();

	void findVoxel(const Position &, int *) const;
	void score(float energy, const Vector3 & position);
	void score2(float energy, const Vector3 & position);
	Vector3 size() const;
	Position iso() const;
	float giveDensity(const Position &) const;
	float giveEDensity(const Position &) const;
	int giveMatID(const Position &) const;
	float step2VoxBoundary(const Position &, const Direction &, int &) const;
	float radiationLength(const Position &) const;
	inline bool ifInside(const Position &);
	Voxel & giveVoxel(const Position &) const;

	void output(const char *) const;
	void outputBinary(const char *) const;
	void energy2Dose();
	void averageLET();

	static float zero;
private:
	Vector3 voxSize;
	int width, hight, depth;
	Position isoCenter;
	Voxel *** voxel3dArray;
};

inline bool Phantom::ifInside(const Position & position){
	int index[3];
	findVoxel(position, index);
	if(index[0]<0 || index[1]<0 || index[2]<0 || index[0] >= width || index[1] >= hight || index[2] >= depth)
		return false;
	else
		return true;
}

inline void Phantom::findVoxel(const Position & position, int * index) const{

	*(index) = static_cast<int>(floor(position.X()/voxSize.X() + float(width)/2.0f));
	*(index+1) = static_cast<int>(floor(position.Y()/voxSize.Y() + float(hight)/2.0f));
	*(index+2) = static_cast<int>(floor(position.Z()/voxSize.Z() + float(depth)/2.0f));
}

inline void Phantom::score(float energy, const Vector3 & position){
	if(!ifInside(position))
		return;
	int index[3];
	findVoxel(position, index);
	voxel3dArray[index[0]][index[1]][index[2]].score(energy);
}

inline void Phantom::score2(float energy, const Vector3 & position){
	if(!ifInside(position))
		return;
	int index[3];
	findVoxel(position, index);
	voxel3dArray[index[0]][index[1]][index[2]].score2(energy);
}

inline Vector3 Phantom::size() const{
	return Vector3(voxSize.X()*width, voxSize.Y()*hight, voxSize.Z()*depth);
}

inline Position Phantom::iso() const{
	return isoCenter;
}

#endif /* PHANTOM_H_ */

