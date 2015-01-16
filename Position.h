/*
 * Position.h
 *
 *  Created on: Sep 17, 2014
 *      Author: s158879
 */

#ifndef POSITION_H_
#define POSITION_H_

#include "Vector3.h"

class Position : public Vector3 {
public:
	Position(const float & x_ = 0, const float & y_ = 0, const float & z_ = 0);
	Position(const Vector3 &);
	virtual ~Position();
	void move(const Vector3 &);
	void move(const Vector3 &, int crossBound);
	bool ifInsideBox(const Position & start, const Position & end);
};

inline bool Position::ifInsideBox(const Position & start, const Position & end){
	if(x<start.x || y<start.y || z<start.z || x>end.x || y>end.y || z>end.z)
		return false;
	return true;
}

#endif /* POSITION_H_ */
