/*
 * Data.h
 *
 *  Created on: Sep 30, 2014
 *      Author: S158879
 */

#ifndef DATA_H_
#define DATA_H_
#include <fstream>

class Data {
public:
	Data();
	virtual ~Data();
	virtual bool setData(const char*) = 0;
};

#endif /* DATA_H_ */
