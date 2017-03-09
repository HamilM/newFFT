/*
 * F2X.h
 *
 *  Created on: Mar 3, 2017
 *      Author: matt
 */

#ifndef F2X_H_
#define F2X_H_
#include <vector>
#include "utils.h"

typedef unsigned long long ValType;
typedef unsigned long long DegType;

class F2X
{
private:
	std::vector<ValType> val;
	unsigned int len;

public:
	F2X();
	~F2X(){};
	F2X(const F2X& a);
	F2X& operator= (const F2X& a);

	// Mathematical operators
	DegType deg() const;
	F2X operator+ (const F2X& a) const;
	F2X& operator+= (const F2X& a);
	F2X operator* (const F2X& a) const;
	F2X& operator*= (const F2X& a);
	F2X operator/ (const F2X& a) const;
	F2X& operator/= (const F2X& a);

};

DegType F2X::deg() const
{
	std::vector<int > a;



}

#endif /* F2X_H_ */
