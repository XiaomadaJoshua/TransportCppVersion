/*
 * Macros.h
 *
 *  Created on: Oct 8, 2014
 *      Author: S158879
 */

#ifndef MACROS_H_
#define MACROS_H_

#define INF 1.0e20
#define WATERDENSITY 0.9997 // g/cm^3
#define MP 938.272046	//proton mass, in MeV
#define CP 1.00000 //protom charge
#define ME 0.510998928  //electron mass, in MeV
#define MO 14903.3460795634 //oxygen mass in MeV
#define MINELECTRONENERGY 0.1 // MeV
#define TWOPIRE2MENEW 0.08515495201157892 //2pi*r_e^2*m_e*n_{ew}, where r_e in cm, m_e in eV, n_ew = 3.34e23/cm^3
#define ES 13.6	// Es parameter for multiple scatter angle, in MeV
#define XW 36.0863	//radiation length of water, in cm
#define PI 3.1415926535897932384626433
#define SECONDPARTICLEVOLUME 10000
#define EMINPOI 3.0	//minimun energy used in p-o inelastic event, in MeV
#define EBIND 7.0	//initial binding energy used in p-o inelastic, in MeV
#define MAXSTEP 0.2 //in cm
#define MAXENERGYRATIO 0.25 //Max energy decay ratio of initial energy in a step
#define MINPROTONENERGY 0.5 //Min proton energy to transport
#define ZERO 1e-6
#define MC 11177.928732 //carbon mass in MeV
#define CC 6.0000 //carbon charge
#define MINCARBONENERGY 5.0 //Min carbon energy to transport in MeV

#define MIN(a,b) (a > b ? b : a)
#define MIN3(a,b,c) (a > b ? b : a) > c ? c : (a > b ? b : a);




#endif /* MACROS_H_ */
