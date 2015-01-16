/*
 * Proton.h
 *
 *  Created on: Oct 9, 2014
 *      Author: S158879
 */

#ifndef PROTON_H_
#define PROTON_H_
#include "ParticleStatus.h"
#include "Macros.h"
#include "SecondaryParticles.h"
#include <math.h>

class Proton : public ParticleStatus {
public:
	Proton();
	Proton(Position, Direction, float);
	Proton(const Proton & p);
	Proton(const ParticleStatus * p);
	virtual ~Proton(){}

	void hardEvent(Random * ranGenerator, Voxel & vox, SecondaryParticles * secondary, const MacroCrossSection * macroSigma, float & Te);

private:
	enum eventType{ION, PPE, POE, POI, imaginary} event;

	void sampleEvent(Random * ranGenerator, const Voxel & vox, const MacroCrossSection * macroSigma);
	void PPElastic(Random * ranGenerator, SecondaryParticles * secondary);
	void POElastic(Random * ranGenerator, Voxel & vox);
	void POInelastic(Random * ranGenerator, SecondaryParticles * secondary, Voxel & vox);
};

#endif /* PROTON_H_ */
