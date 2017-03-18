/*
 * F2XE.h
 *
 *  Created on: Mar 1, 2017
 *      Author: matt
 */

#ifndef F2XE_H_
#define F2XE_H_

#include <cinttypes>
#include <cstring>
#include <cstdbool>
#include <memory>
#include <x86intrin.h>
#include "utils.h"
#include "F2X.h"

extern __m128i clmul_mask;

template<unsigned int N>
class F2XE {
private:
	static const unsigned int bitsInEntry = sizeof(ValType) * BITS_IN_BYTE;
	static const unsigned int len = CIEL(N, sizeof(ValType));
	static std::shared_ptr<F2XE<N>> irred;
	ValType val[CIEL(N,sizeof(ValType))];
	bool getCoefficient(DegType i) const;
public:
	F2XE();
	F2XE(const F2XE<N>& a);
	~F2XE(){};
	F2XE<N>& sqr();
	F2XE<N> operator/(const F2XE<N> &a) const;
	F2XE<N>& operator/= (const F2XE<N> &a);
	F2XE<N>& operator+=(const F2XE<N>& a);
	F2XE<N> operator+(const F2XE<N>& a) const;
	F2XE<N>& operator*=(const F2XE<N>& a);
	F2XE<N> operator*(const F2XE<N>& a) const;
	F2XE<N>& operator=(const F2XE<N>& a);
	F2XE<N> operator~() const; //<- Calculates the inverse of an element
	bool isZero() const;
	bool isUnit() const;
	void setZero();
	void setUnit();
	F2X toStdRepr() const;


	// Sets the irreducible used by the elements of the field of this size to be
	// x^N + irred.
	static void setIrred(const std::shared_ptr<F2XE<N>>& irred);


};

template<unsigned int N>
std::shared_ptr<F2XE<N>>  F2XE<N>::irred = NULL;

template<unsigned int N>
void F2XE<N>::setIrred(const std::shared_ptr<F2XE<N>>& irred)
{
	F2XE<N>::irred = irred;
}


template<unsigned int N>
F2XE<N>::F2XE(){}

template<unsigned int N>
F2XE<N>::F2XE(const F2XE<N>& a)
{
	memcpy(this->val, a.val, sizeof(a.val));
}

template<unsigned int N>
F2XE<N> F2XE<N>::operator+(const F2XE<N>& a) const
{
	F2XE<N> r(a);
	return r+=(*this);
}

template<unsigned int N>
F2XE<N>& F2XE<N>::operator+=(const F2XE<N>& a)
{
	for (int i = 0  ; i < len ; ++i)
		{
			this->val[i] ^= a.val[i];
		}
	return this;
}




template<unsigned int N>
F2XE<N>& F2XE<N>::operator*=(const F2XE<N>& a)
{
	return *this;
}

template<unsigned int N>
F2XE<N> F2XE<N>::operator *(const F2XE<N>& a) const
{
	return a;
}

template<>
F2XE<64>& F2XE<64>::operator*=(const F2XE<64>& a);

template<>
F2XE<64> F2XE<64>::operator* (const F2XE<64>& a) const;

template<unsigned int N>
F2XE<N>& F2XE<N>::operator=(const F2XE<N>& a)
{
	memcpy(this->val, a.val, sizeof(this->val));
	return *this;
}

template <unsigned int N>
bool F2XE<N>::isZero() const
{
	for (int i = 0 ; i < this->len ; ++i)
	{
		if (0 != this->val[i])
		{
			return false;
		}
	}
	return true;

}

template <unsigned int N>
bool F2XE<N>::isUnit() const
{
	if (1 != this->val[0])
	{
		return false;
	}
	for (int i =  1; i < this->len ; ++i)
	{
		if (this->val[i] != 0)
		{
			return false;
		}
	}
	return true;
}

template <unsigned int N>
void F2XE<N>::setZero()
{
	memset(this->val, 0, sizeof(this->val));
}

template <unsigned int N>
void F2XE<N>::setUnit()
{
	this->setZero();
	this->val[0] = 1;
}

template <unsigned int N>
F2X F2XE<N>::toStdRepr() const
{
	F2X ans(N - 1);
	GF2 a(true);
	for (unsigned int i = 0 ; i < N - 1 ; ++i)
	{
		if (this->getCoefficient(i))
		{
			ans.setCoefficient(i, a);
		}
	}
	return ans;
}

template <unsigned int N>
bool F2XE<N>::getCoefficient(DegType i) const
{
	auto cellIdx = i / this->bitsInEntry;
	if (cellIdx > this->len)
	{
		return false;
	}
	auto cell = this->val[cellIdx];
	// If i-th bit in the representation is on - return true, else - false.
	return cell & (1 << (i % this->bitsInEntry)) == 0 ? false : true;
}

template<unsigned int N>
F2XE<N>& F2XE<N>::sqr()
{
	F2XE a(*this);
	(*this)*=a;
	return *this;
}

template<unsigned int N>
F2XE<N> F2XE<N>::operator~() const
{
	F2XE<N> t;
	t.setZero();
	F2XE<N> r(*this->irred);
	F2XE<N> newr(*this);
	F2XE<N> newt;
	newt.setUnit();

	while (!newr.isZero())
	{
		auto q = r.toStdRepr() / newr.toStdRepr();
		auto tmp = r;
		r = newr;
		newr = tmp - q * newr;
		tmp = t;
		t = newt;
		newt = tmp - q * newt;
	}
	return t;
}

template<unsigned int N>
F2XE<N> F2XE<N>::operator/(const F2XE<N> &a) const
{
	F2XE t(*this);
	t/=a;
	return t;
}

template<unsigned int N>
F2XE<N>& F2XE<N>::operator/= (const F2XE<N> &a)
{
	F2XE n(~a);
	(*this)*=n;
}


#endif /* F2XE_H_ */
