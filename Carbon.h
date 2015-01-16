/*
 * Carbon.h
 *
 *  Created on: Nov 24, 2014
 *      Author: S158879
 */

#ifndef CARBON_H_
#define CARBON_H_
#include "ParticleStatus.h"
#include "Macros.h"
#include "SecondaryParticles.h"
#include <math.h>

class Carbon : public ParticleStatus {
public:
	Carbon();
	virtual ~Carbon();
	Carbon(Position, Direction, float);
	Carbon(const Carbon & c);
	Carbon(const ParticleStatus * c);

	virtual void hardEvent(Random * ranGenerator, Voxel & vox, SecondaryParticles * secondary, const MacroCrossSection * macroSigma, float & Te);
private:
	enum eventType{ION,imaginary} event;

	float energyInOneStep(const Voxel & vox, const RSPW * massStpPowRatio, const MSPR * resStpPowWater, float stepLength) const;
	float getMaxStep(const Voxel & vox, const RSPW * resStpWater, const MSPR * massStpPowRatio) const;
	void sampleEvent(Random * ranGenerator, const Voxel & vox, const MacroCrossSection * macroSigma);
};

#endif /* CARBON_H_ */
