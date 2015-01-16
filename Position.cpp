/*
 * Position.cpp
 *
 *  Created on: Sep 17, 2014
 *      Author: s158879
 */

#include "Position.h"



Position::Position(const float & x_, const float & y_, const float & z_): Vector3(x_, y_, z_){
}

Position::Position(const Vector3 & vec):Vector3(vec){
}

Position::~Position() {
	// TODO Auto-generated destructor stub
}

void Position::move(const Vector3 & step){
	this->operator +=(step);
}

void Position::move(const Vector3 & value, int crossBound){
	switch(crossBound){
	case(1):
		if(value.X() < zero && -1.0f*value.X() > -1.0f*zero)
			if(value.X() >= 0)
				x += zero;
			else
				x -= zero;
		else
			x += value.X();
		y += value.Y();
		z += value.Z();
		break;

	case(2):
		if(value.Y() < zero && -1.0f*value.Y() > -1.0f*zero)
			if(value.Y() >= 0)
				y += zero;
			else
				y -= zero;
		else
			y += value.Y();
		x += value.X();
		z += value.Z();
		break;

	case(3):
		if(value.Z() < zero && -1.0f*value.Z() > -1.0f*zero)
			if(value.Z() >= 0)
				z += zero;
			else
				z -= zero;
		else
			z += value.Z();
		y += value.Y();
		x += value.X();
		break;

	case(0):
		x += value.X();
		y += value.Y();
		z += value.Z();
		break;
	}
}
