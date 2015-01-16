/*
 * ParticleStatus.h
 *
 *  Created on: Sep 19, 2014
 *      Author: S158879
 */

#ifndef PARTICLESTATUS_H_
#define PARTICLESTATUS_H_

#include "Position.h"
#include "Direction.h"
#include "Random.h"
#include "Phantom.h"
#include "MacroCrossSection.h"
#include "RSPW.h"
#include "MSPR.h"

class SecondaryParticles;

class ParticleStatus {
public:
	ParticleStatus();
	ParticleStatus(Position, Direction, float energy_ = 0.0f, float m_ = 0.0f, float c_ = 0.0f);
	virtual ~ParticleStatus();

	void setStatus(Position pos_, Direction dir_, float e_);
	const Position & currentPos() const;
	const Direction & currentDir() const;
	float currentEnergy() const;
	Position lastPos() const;
	float lastEnergy() const;
	void update(Vector3 step, float theta, float phi, float energyTran, int crossBound);
	void update(Vector3 step, float theta, float phi, float energyTran);


	virtual float mass() const;
	virtual float charge() const;
	virtual float gamma() const;
	virtual float beta() const;
	virtual float maxDeltaElectronEnergy() const;
	virtual float maxOxygenEnergy() const;
	virtual float momentumSquare() const;

	virtual void propagate(Random * ranGenerator, Phantom * phantom, Voxel & vox, const MacroCrossSection * macroSigma, const RSPW * resStpPowWater, const MSPR * massStpPowRatio,
			float & energyTransfer, float & stepLength);
	virtual void hardEvent(Random * ranGenerator, Voxel & vox, SecondaryParticles * secondary, const MacroCrossSection * macroSigma, float & Te) = 0;
protected:
	Position pos;
	Direction dir;
	float energy;
	bool ifHard;
	float maxSigma;
	float m, c;

	virtual float energyInOneStep(const Voxel & vox, const RSPW * resStpPowWater, const MSPR * massStpPowRatio, float stepLength) const;
	virtual float energyStraggling(Random * ranGenerator, const Voxel & vox, float stepLength) const;
	virtual float getMaxStep(const Voxel & vox, const RSPW * resStpWater, const MSPR * massStpPowRatio) const;
	virtual float totalLinearSigma(const Voxel & vox, const MacroCrossSection * macroSigma, float e) const;
	virtual float ionization(Random * ranGenerator, Voxel & vox);
};

inline const Position & ParticleStatus::currentPos() const{
	return pos;
}
inline const Direction & ParticleStatus::currentDir() const{
	return dir;
}
inline float ParticleStatus::currentEnergy() const{
	return energy;
}
inline float ParticleStatus::mass() const{
	return m;
}
inline float ParticleStatus::charge() const{
	return c;
}
inline float ParticleStatus::gamma() const{
	return (energy + m)/m;
}
inline float ParticleStatus::beta() const{
	return sqrt(1 - m*m/((energy + m)*(energy + m)));
}

inline float ParticleStatus::maxDeltaElectronEnergy() const{
	return (2*ME*beta()*beta()*gamma()*gamma())/(1 + 2*gamma()*ME/m + ME*ME/(m*m));
}

inline float ParticleStatus::maxOxygenEnergy() const{
	return (2*MO*beta()*beta()*gamma()*gamma())/(1 + 2*gamma()*MO/m + MO*MO/(m*m));
}

inline float ParticleStatus::momentumSquare() const{
	return energy*energy + 2*energy*m;
}

#endif /* PARTICLESTATUS_H_ */
