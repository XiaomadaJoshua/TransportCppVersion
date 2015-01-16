/*
 * Direction.cpp
 *
 *  Created on: Sep 18, 2014
 *      Author: S158879
 */

#include "Direction.h"
#include "Macros.h"

Direction::Direction(const float & x_, const float & y_, const float & z_): Vector3(x_, y_, z_){
	float norm = x*x + y*y + z*z;
	if(norm > 1.0+zero || norm < 1.0-zero){
		x = x/sqrt(norm);
		y = y/sqrt(norm);
		z = z/sqrt(norm);
	}
}

Direction::Direction(const Vector3 & vec):Vector3(vec){
	float norm = x*x + y*y + z*z;
	if(norm > 1.0+zero || norm < 1.0-zero){
		x = x/sqrt(norm);
		y = y/sqrt(norm);
		z = z/sqrt(norm);
	}
}

Direction::~Direction() {
	// TODO Auto-generated destructor stub
}

void Direction::transform(float theta, float phi){
	// if original direction is along z-axis
	float temp = 1.0-zero;
	if(z*z >= temp){
		if(z > 0){
			x = sin(theta)*cos(phi);
			y = sin(theta)*sin(phi);
			z = cos(theta);
		}
		else{
			x = -sin(theta)*cos(phi);
			y = -sin(theta)*sin(phi);
			z = -cos(theta);
		}
	}
	else{
		float u, v, w;
		 u= x*cos(theta) + sin(theta)*(x*z*cos(phi) - y*sin(phi))/sqrt(1.0 - z*z);
		 v= y*cos(theta) + sin(theta)*(y*z*cos(phi) + x*sin(phi))/sqrt(1.0 - z*z);
		 w = z*cos(theta) - sqrt(1 - z*z)*sin(theta)*cos(phi);
		 x = u;
		 y = v;
		 z = w;
	}
	// if norm does not equal to 1
	float norm = x*x + y*y + z*z;
		if(norm > 1.0+zero || norm < 1.0-zero){
			x = x/sqrt(norm);
			y = y/sqrt(norm);
			z = z/sqrt(norm);
		}
}


Vector3 Direction::operator*(float coef) const{
	float x_ = x*coef;
	float y_ = y*coef;
	float z_ = z*coef;
	return Vector3(x_, y_, z_);
}















