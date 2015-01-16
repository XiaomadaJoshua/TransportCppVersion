/*
 * Direction.h
 *
 *  Created on: Sep 18, 2014
 *      Author: S158879
 */

#ifndef DIRECTION_H_
#define DIRECTION_H_

#include "Vector3.h"
#include <iostream>
#include <math.h>

class Direction: public Vector3 {
public:
	Direction(const float & x_ = 0, const float & y_ = 1, const float & z_ = 0);
	Direction(const Vector3 &);
	virtual ~Direction();
	void transform(float, float);
	Vector3 operator*(float) const;
	void printZero();
};

inline void Direction::printZero(){
	std::cout << zero << std::endl;
}

#endif /* DIRECTION_H_ */
