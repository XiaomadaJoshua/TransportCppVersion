/*
 * Vector3.cpp
 *
 *  Created on: Sep 17, 2014
 *      Author: s158879
 */

#include "Vector3.h"

float Vector3::zero = ZERO;

Vector3::~Vector3() {
	// TODO Auto-generated destructor stub
}

Vector3::Vector3(const float & x_, const float & y_, const float & z_):
	x(x_), y(y_), z(z_){

}

Vector3::Vector3(const Vector3 & vec):x(vec.x), y(vec.y), z(vec.z){
}

float Vector3::getLength() const{
	return sqrt(x*x + y*y + z*z);
}
