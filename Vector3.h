/*
 * Vector3.h
 *
 *  Created on: Sep 17, 2014
 *      Author: s158879
 */

#ifndef VECTOR3_H_
#define VECTOR3_H_
#include <math.h>
#include "Macros.h"

class Vector3 {
public:
	Vector3(const float & x_ = 0, const float & y_ = 0, const float & z_ = 0);
	Vector3(const Vector3 &);
	virtual ~Vector3();
	virtual Vector3 operator+(const Vector3 &);
	virtual Vector3 operator-(const Vector3 &);
	virtual Vector3 operator*(const Vector3 &);
	virtual Vector3 & operator=(const Vector3 &);
	virtual Vector3 & operator+=(const Vector3 &);
	virtual float X() const {return x;}
	virtual float Y() const {return y;}
	virtual float Z() const {return z;}
	virtual float getLength() const;
//	virtual bool operator<(Vector3 &);
//	virtual bool operator >(const Vector3 &)const;

protected:
	float x, y, z;
	static float zero;
};

inline Vector3 Vector3::operator+(const Vector3 & addition){
	Vector3 sum;
	sum.x = x + addition.x;
	sum.y = y + addition.y;
	sum.z = z + addition.z;
	return sum;
}

inline Vector3 & Vector3::operator +=(const Vector3 & value){
	x += value.x;
	y += value.y;
	z += value.z;
	return *this;
}

inline Vector3 Vector3::operator-(const Vector3 & addition){
	Vector3 dif;
	dif.x = x - addition.x;
	dif.y = y - addition.y;
	dif.z = z - addition.z;
	return dif;
}

inline Vector3 Vector3::operator*(const Vector3 & addition){
	Vector3 pro;
	pro.x = x * addition.x;
	pro.y = y * addition.y;
	pro.z = z * addition.z;
	return pro;
}

inline Vector3 & Vector3::operator =(const Vector3 & value){
	x = value.x;
	y = value.y;
	z = value.z;
	return *this;
}


#endif /* VECTOR3_H_ */
