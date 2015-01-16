/*
 * Phantom.cpp
 *
 *  Created on: Sep 29, 2014
 *      Author: S158879
 */

#include "Phantom.h"
#include <stdlib.h>
#include <fstream>
#include "MSPR.h"
#include "Macros.h"
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <string>

using namespace std;


float Phantom::zero = ZERO;

Phantom::Phantom():voxSize(0, 0, 0), width(0), hight(0), depth(0), isoCenter(0, 0, 0), voxel3dArray(NULL) {
	// TODO Auto-generated constructor stub

}

Phantom::Phantom(const Vector3 & voxSize_, int width_, int hight_, int depth_, const Vector3 & iso): voxSize(voxSize_), width(width_), hight(hight_), depth(depth_), isoCenter(iso){
	voxel3dArray = new Voxel ** [width];
	for(int i=0; i<width; i++){
		voxel3dArray[i] = new Voxel * [hight];
		for(int j=0; j<hight; j++)
			voxel3dArray[i][j] = new Voxel[depth];
	}
}

Phantom::~Phantom() {
	// TODO Auto-generated destructor stub
	for(int i=0; i<width; i++){
		for(int j=0; j<hight; j++)
			delete [] voxel3dArray[i][j];
		delete [] voxel3dArray[i];
	}
	delete [] voxel3dArray;
}

void Phantom::initPhantom(Vector3 & voxSize_, int width_, int hight_, int depth_, const Vector3 & iso, const MSPR & mspr, const DensCorrection & densCorrection){
	 voxSize = voxSize_;
	 width = width_;
	 hight = hight_;
	 depth = depth_;
	 isoCenter = iso;

	 voxel3dArray = new Voxel ** [width];
	 for(int i=0; i<width; i++){
	 	voxel3dArray[i] = new Voxel * [hight];
	 	for(int j=0; j<hight; j++){
	 		voxel3dArray[i][j] = new Voxel[depth];
	 		for(int k=0; k<depth; k++)
	 			voxel3dArray[i][j][k].initVoxel(0.0f, mspr, densCorrection);
	 	}
	 }
}

float Phantom::giveDensity(const Position & position) const{
	int index[3];
	findVoxel(position, index);
	return voxel3dArray[index[0]][index[1]][index[2]].giveDensity();
}

float Phantom::giveEDensity(const Position & position) const{
	int index[3];
	findVoxel(position, index);
	return voxel3dArray[index[0]][index[1]][index[2]].giveEDensity();
}

int Phantom::giveMatID(const Position & position) const{
	int index[3];
	findVoxel(position, index);
	return voxel3dArray[index[0]][index[1]][index[2]].giveMatID();
}

float Phantom::step2VoxBoundary(const Position & pos, const Direction & dir, int & cb) const{
	float stepX, stepY, stepZ;
	int currentVoxel[3];
	findVoxel(pos, currentVoxel);
	if(dir.X() < zero && -1.0f*dir.X() > -1.0f*zero)
		stepX = INF;
	else if(dir.X() > 0)
		stepX = (ceil(pos.X()/voxSize.X())*voxSize.X() - pos.X())/dir.X();
	else
		stepX = (floor(pos.X()/voxSize.X())*voxSize.X() - pos.X())/dir.X();

	if(dir.Y() < zero && -1.0f*dir.Y() > -1.0f*zero)
		stepY = INF;
	else if(dir.Y() > 0)
		stepY = (ceil(pos.Y()/voxSize.Y())*voxSize.Y() - pos.Y())/dir.Y();
	else
		stepY = (floor(pos.Y()/voxSize.Y())*voxSize.Y() - pos.Y())/dir.Y();

	if(dir.Z() < zero && -1.0f*dir.Z() > -1.0f*zero)
		stepZ = INF;
	else if(dir.Z() > 0)
		stepZ = (ceil(pos.Z()/voxSize.Z())*voxSize.Z() - pos.Z())/dir.Z();
	else
		stepZ = (floor(pos.Z()/voxSize.Z())*voxSize.Z() - pos.Z())/dir.Z();

	float step;
	if(stepX < stepY){
		step = stepX;
		if(step < stepZ)
			cb = 1;
		else{
			step = stepZ;
			cb = 3;
		}
	}
	else{
		step = stepY;
		if(step < stepZ)
			cb = 2;
		else{
			step = stepZ;
			cb = 3;
		}
	}

	return step < zero ? zero : step;
}

float Phantom::radiationLength(const Position & position) const{
	int index[3];
	findVoxel(position, index);
	return voxel3dArray[index[0]][index[1]][index[2]].radiationLength();
}

void Phantom::output(const char * dir) const{
	string file(dir);
	string file2(dir);
	string fileHeavy(dir);
	string fileElectron(dir);
	string fileTotal(dir);
	string fileLET(dir);
	string fileLET2(dir);
	file.append("primary.dat");
	file2.append("secondary.dat");
	fileHeavy.append("heavy.dat");
	fileElectron.append("electron.dat");
	fileTotal.append("total.dat");
	fileLET.append("LET.dat");
	fileLET2.append("LET2.dat");
	ofstream ofs(file.c_str(), ios::out|ios::trunc);
	ofstream ofs2(file2.c_str(), ios::out|ios::trunc);
	ofstream ofsHeavy(fileHeavy.c_str(), ios::out|ios::trunc);
	ofstream ofsElectron(fileElectron.c_str(), ios::out|ios::trunc);
	ofstream ofsTotal(fileTotal.c_str(), ios::out|ios::trunc);
	ofstream ofsLET(fileLET.c_str(), ios::out|ios::trunc);
	ofstream ofsLET2(fileLET2.c_str(), ios::out|ios::trunc);
	float e1, e2, eHeavy, eElectron, total;
	float l1, l2;
	for(int k = 0; k < depth; k++){
		for(int j = 0; j < hight; j++){
			for(int i = 0; i < width; i++){
				voxel3dArray[i][j][k].giveEnergy(e1, e2, eHeavy, eElectron, total);
				voxel3dArray[i][j][k].giveLET(l1, l2);
				ofs << e1 << '\t';
				ofs2 << e2 << '\t';
				ofsHeavy << eHeavy << '\t';
				ofsElectron << eElectron <<'\t';
				ofsTotal << total << '\t';
				ofsLET << l1 << '\t';
				ofsLET2 << l2 << '\t';
			}
			ofs << '\n';
			ofs2 << '\n';
			ofsHeavy << '\n';
			ofsElectron << '\n';
			ofsTotal << '\n';
			ofsLET << '\n';
			ofsLET2 << '\n';
		}
		ofs << '\n';
		ofs2 << '\n';
		ofsHeavy << '\n';
		ofsElectron << '\n';
		ofsTotal << '\n';
		ofsLET << '\n';
		ofsLET2 << '\n';
	}

	ofs.close();
	ofs2.close();
	ofsHeavy.close();
	ofsElectron.close();
	ofsTotal.close();
	ofsLET.close();
	ofsLET2.close();
}

void Phantom::outputBinary(const char * dir) const{
	string file(dir);
	string file2(dir);
	string fileHeavy(dir);
	string fileElectron(dir);
	string fileTotal(dir);
	string fileLET(dir);
	string fileLET2(dir);
	file.append("primary.bin");
	file2.append("secondary.bin");
	fileHeavy.append("heavy.bin");
	fileElectron.append("electron.bin");
	fileTotal.append("total.bin");
	fileLET.append("LET.bin");
	fileLET2.append("LET2.bin");

	float e1, e2, eHeavy, eElectron, total;
	float l1, l2;
	float * data;
	float * data2;
	float * dataHeavy;
	float * dataElectron;
	float * dataTotal;
	float * dataLET;
	float * dataLET2;
	data = new float[width*hight*depth];
	data2 = new float[width*hight*depth];
	dataHeavy = new float[width*hight*depth];
	dataElectron = new float[width*hight*depth];
	dataTotal = new float[width*hight*depth];
	dataLET = new float[width*hight*depth];
	dataLET2 = new float[width*hight*depth];

	for(int k = 0; k < depth; k++){
		for(int j = 0; j < hight; j++){
			for(int i = 0; i < width; i++){
				voxel3dArray[i][j][k].giveEnergy(e1, e2, eHeavy, eElectron, total);
				voxel3dArray[i][j][k].giveLET(l1, l2);
				data[i + j*width + k*(width*hight)] = e1;
				data2[i + j*width + k*(width*hight)] = e2;
				dataHeavy[i + j*width + k*(width*hight)] = eHeavy;
				dataElectron[i + j*width + k*(width*hight)] = eElectron;
				dataTotal[i + j*width + k*(width*hight)] = total;
				dataLET[i + j*width + k*(width*hight)] = l1;
				dataLET2[i + j*width + k*(width*hight)] = l2;
			}
		}
	}
	FILE * pFile;
	pFile = fopen(file.c_str(), "wb");
	fwrite(data, sizeof(float), width*hight*depth, pFile);
	fclose(pFile);

	pFile = fopen(file2.c_str(), "wb");
	fwrite(data2, sizeof(float), width*hight*depth, pFile);
	fclose(pFile);

	pFile = fopen(fileHeavy.c_str(), "wb");
	fwrite(dataHeavy, sizeof(float), width*hight*depth, pFile);
	fclose(pFile);

	pFile = fopen(fileElectron.c_str(), "wb");
	fwrite(dataElectron, sizeof(float), width*hight*depth, pFile);
	fclose(pFile);

	pFile = fopen(fileTotal.c_str(), "wb");
	fwrite(dataTotal, sizeof(float), width*hight*depth, pFile);
	fclose(pFile);

	pFile = fopen(fileLET.c_str(), "wb");
	fwrite(dataLET, sizeof(float), width*hight*depth, pFile);
	fclose(pFile);

	pFile = fopen(fileLET2.c_str(), "wb");
	fwrite(dataLET2, sizeof(float), width*hight*depth, pFile);
	fclose(pFile);

	delete [] data;
	delete [] data2;
	delete [] dataHeavy;
	delete [] dataTotal;
	delete [] dataLET;
	delete [] dataLET2;
}

void Phantom::energy2Dose(){
	for(int i = 0; i < width; i++)
		for(int j = 0; j < hight; j++)
			for(int k = 0; k < depth; k++)
				voxel3dArray[i][j][k].energy2Dose(voxSize);
}

void Phantom::averageLET(){
	for(int i = 0; i < width; i++)
		for(int j = 0; j < hight; j++)
			for(int k = 0; k < depth; k++)
				voxel3dArray[i][j][k].averageLET();
}

Voxel & Phantom::giveVoxel(const Position & pos) const{
	int index[3];
	findVoxel(pos, index);
	return voxel3dArray[index[0]][index[1]][index[2]];
}

