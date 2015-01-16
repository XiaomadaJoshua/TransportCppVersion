/*
 * main.cpp
 *
 *  Created on: Sep 18, 2014
 *      Author: S158879
 */

#include "Vector3.h"
#include "Position.h"
#include "Direction.h"
#include "Voxel.h"
#include "Phantom.h"
#include "DensCorrection.h"
#include "MacroCrossSection.h"
#include "RSPW.h"
#include "MSPR.h"
#include "RandomMersenneTwister.h"
#include "AntiThetic.h"
#include "Beam.h"
#include "Proton.h"
#include <iostream>
#include "MCEngine.h"
#include "Carbon.h"



int main(int argc, char *argv[]){
	string input = argv[1];
	if(input.find("proton") != string::npos){
		MCEngine<Proton> mc(input.c_str());
		mc.simulate(MINPROTONENERGY);
		mc.output();
	}
	else if(input.find("carbon") != string::npos){
		MCEngine<Carbon> mc(input.c_str());
		mc.simulate(MINCARBONENERGY);
		mc.output();
	}

	return 0;
}
