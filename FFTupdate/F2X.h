/*
 * F2X.h
 *
 *  Created on: Mar 3, 2017
 *      Author: matt
 */

#ifndef F2X_H_
#define F2X_H_
#include <vector>
#include <memory>
#include <cstdbool>
#include "utils.h"
#include "GF2.h"

typedef unsigned long long ValType;
typedef unsigned long long DegType;

class F2X
{
private:
	const static unsigned int bitsInEntry = BITS_IN_BYTE * sizeof(ValType);

	std::shared_ptr<ValType> val;
	unsigned int len;
	void resize(unsigned int len);
	void fit();
	DegType getFitLen() const;
	ValType& operator[](size_t index);
	const ValType& operator [](size_t index) const;
	void flipCoefficient(DegType d);
	void setCoefficient(DegType d, const GF2& a);
	GF2 getCoefficient(DegType d) const;
public:
	F2X();
	F2X(unsigned int i);
	~F2X(){};
	F2X(const F2X& a);
	F2X& operator= (const F2X& a);

	// Mathematical operators
	void flipCoefficient(DegType d);
	void setCoefficient(DegType d, const GF2& a);
	GF2 getCoefficient(DegType d) const;
	DegType deg() const;
	F2X operator+ (const F2X& a) const;
	F2X& operator+= (const F2X& a);
	F2X operator* (const F2X& a) const;
	F2X& operator*= (const F2X& a);
	F2X operator/ (const F2X& a) const;
	F2X& operator/= (const F2X& a);
	bool operator== (const F2X& a) const;
	bool operator!= (const F2X& a) const;

	F2X operator<< (unsigned int a) const;
	const F2X& operator<<= (unsigned int a);
	F2X operator>> (unsigned int a) const;
	const F2X& operator>>= (unsigned int a);
};



#endif /* F2X_H_ */
