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
#include "utils.h"

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
	void setCoefficient(DegType d);
	DegType getCoefficient(DegType d);
public:
	F2X();
	F2X(unsigned int i);
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



#endif /* F2X_H_ */
