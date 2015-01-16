/*
 * MCEngine.h
 *
 *  Created on: Nov 24, 2014
 *      Author: S158879
 */

#ifndef MCEngine_H_
#define MCEngine_H_
/*
 * MCEngine.h
 *
 *  Created on: Oct 16, 2014
 *      Author: S158879
 */

#include "MacroCrossSection.h"
#include "MSPR.h"
#include "RSPW.h"
#include "DensCorrection.h"
#include "Beam.h"
#include "Phantom.h"
#include "SecondaryParticles.h"
#include <stdlib.h>
#include "Random.h"
#include "Proton.h"
#include "AntiThetic.h"
#include "RandomMersenneTwister.h"
#include <string>
#include "Macros.h"
#include <fstream>

using namespace std;


class Position;
class Random;


template <typename T>
class MCEngine {
public:
	MCEngine(const char *);
	virtual ~MCEngine();
	void simulate(float minEnergy);
	void output();
private:
	Random * ranGenerator;
	Beam * beam;
	Phantom * phantom;
	MacroCrossSection * macroSigma;
	MSPR * mspr;
	RSPW * rspw;
	DensCorrection * densCorrection;
	SecondaryParticles * second;
	string outDir;
	unsigned int nBatch;
};




template <typename T>
MCEngine<T>::MCEngine (const char * file):nBatch(1){
	// TODO Auto-generated constructor stub
	// initialize from file
	ifstream ifs(file, fstream::in);
	char buff[300];
	string directory;

	// initialize physics data
	ifs.getline(buff, 300);
	getline(ifs, directory);
	macroSigma = new MacroCrossSection();
	macroSigma->setData(directory.c_str());

	ifs.getline(buff, 300);
	getline(ifs, directory);
	mspr = new MSPR();
	mspr->setData(directory.c_str());

	ifs.getline(buff, 300);
	getline(ifs, directory);
	rspw = new RSPW();
	rspw->setData(directory.c_str());

	ifs.getline(buff,300);
	getline(ifs, directory);
	densCorrection = new DensCorrection();
	densCorrection->setData(directory.c_str());

	// initialize a random number generator
	ranGenerator = new RandomMersenneTwister();

	// initialize beam
	ifs.getline(buff, 300);
	beam = new Beam();
	float bWidth, energy;
	unsigned long nPaths;
	Position bSource, bIso;
	ifs >> bWidth >> ws >> energy >> ws >> nPaths >> ws;
	float x, y, z;
	ifs >> x >> y >> z >> ws;
	bSource = Vector3(x, y, z);
	ifs >> x >> y >> z >> ws;
	bIso = Vector3(x, y, z);
	beam->initBeam<T> (bWidth, bSource, bIso, nPaths, ranGenerator, energy);

	//number of batches
	ifs.getline(buff, 300);
	ifs >> nBatch >> ws;

	//initialize phantom
	ifs.getline(buff, 300);
	phantom = new Phantom();
	ifs >> x >> y >> z >> ws;
	Vector3 voxSize(x, y, z);
	ifs >> x >> y >> z >> ws;
	Position pIso(x, y, z);
	int width, hight, depth;
	ifs >> width >> hight >> depth >> ws;
	phantom->initPhantom(voxSize, width, hight, depth, pIso, *mspr, *densCorrection);

	//initialize secondary particles container
	ifs.getline(buff, 300);
	ifs >> nPaths >> ws;
	second = new SecondaryParticles(nPaths);

	// output directory
	ifs.getline(buff, 300);
	getline(ifs, outDir);
}

template <typename T>
MCEngine<T>::~MCEngine() {
	// TODO Auto-generated destructor stub
	delete ranGenerator;
	delete beam;
	delete phantom;
	delete macroSigma;
	delete mspr;
	delete rspw;
	delete densCorrection;
	delete second;
}

template <typename T>
void MCEngine<T>::simulate(float minEnergy){
	ParticleStatus * thisOne(NULL);
	unsigned long debugi = 0;
	unsigned long debugj = 0;
	unsigned long debugk = 0;
	float energyTransfer;
	float stepLength;
	float Te;

	for(unsigned int i = 0; i < nBatch; i++){
		while(!second->isEmpty() || !beam->isEmpty()){
			while(!second->isEmpty()){
				second->pop(&thisOne);
				cout << "propagating second " << ++debugj << endl;
				while(thisOne->currentEnergy() > minEnergy){
					if(!phantom->ifInside(thisOne->currentPos()))
						break;

					++debugk;
					Voxel & thisVox = phantom->giveVoxel(thisOne->currentPos());
					energyTransfer = 0.0f;
					Te = 0.0f;
					thisOne->propagate(ranGenerator, phantom, thisVox, macroSigma, rspw, mspr, energyTransfer, stepLength);
					thisOne->hardEvent(ranGenerator, thisVox, second, macroSigma, Te);
					thisVox.score2(energyTransfer);
					thisVox.scoreLET2((energyTransfer + Te)*(energyTransfer + Te)/stepLength);
				}
				if(phantom->ifInside(thisOne->currentPos())){
					if(thisOne->currentEnergy() == 0)
						continue;
					Voxel & thisVox = phantom->giveVoxel(thisOne->currentPos());
					thisVox.score2(thisOne->currentEnergy());
					float stepInWater = thisOne->currentEnergy()/rspw->lookLw(thisOne->currentEnergy());
					float ratio = mspr->lookMSPR(thisVox.giveMatID(), thisOne->currentEnergy());
					stepLength = stepInWater*WATERDENSITY/ratio/thisVox.giveDensity();
					thisVox.scoreLET2(thisOne->currentEnergy()*thisOne->currentEnergy()/stepLength);
				}
				delete thisOne;
			}
			if(!beam->isEmpty()){
				beam->popOne(&thisOne);
				cout << "propagating beam " << ++debugi << endl;

				while(thisOne->currentEnergy() > minEnergy){
					if(!phantom->ifInside(thisOne->currentPos()))
						break;

					++debugk;
					Voxel & thisVox = phantom->giveVoxel(thisOne->currentPos());
					energyTransfer = 0.0f;
					Te = 0.0f;
					thisOne->propagate(ranGenerator, phantom, thisVox, macroSigma, rspw, mspr, energyTransfer, stepLength);
					thisOne->hardEvent(ranGenerator, thisVox, second, macroSigma, Te);
					thisVox.score(energyTransfer);
					thisVox.scoreLET((energyTransfer + Te)*(energyTransfer + Te)/stepLength);
				}

				if(phantom->ifInside(thisOne->currentPos())){
					if(thisOne->currentEnergy() == 0)
						continue;
					Voxel & thisVox = phantom->giveVoxel(thisOne->currentPos());
					thisVox.score(thisOne->currentEnergy());
					float stepInWater = thisOne->currentEnergy()/rspw->lookLw(thisOne->currentEnergy());
					float ratio = mspr->lookMSPR(thisVox.giveMatID(), thisOne->currentEnergy());
					stepLength = stepInWater*WATERDENSITY/ratio/thisVox.giveDensity();
					thisVox.scoreLET(thisOne->currentEnergy()*thisOne->currentEnergy()/stepLength);
				}
				delete thisOne;
			}
		}
		beam->initBeam <T> (*beam, ranGenerator);
	}
	cout << "total steps " << debugk << endl;
}


template <typename T>
void MCEngine<T>::output(){
	phantom->averageLET();
	phantom->energy2Dose();
	phantom->output(outDir.c_str());
	phantom->outputBinary(outDir.c_str());
}







#endif /* MCEngine_H_ */
