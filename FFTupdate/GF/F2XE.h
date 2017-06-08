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
#include <exception>
#include <GF2.h>
#include "utils.h"
#include "F2X.h"

extern __m128i clmul_mask;

class F2XEException : public std::exception {};
class PolynomialDegreeTooHigh : public F2XEException {};

template <unsigned int N>
class F2XE {
private:
	static const unsigned int bitsInEntry = sizeof(ValType) * BITS_IN_BYTE;
	static const unsigned int len = CIEL(N, 8*sizeof(ValType));
    static F2XE<N> irred_stripped;
	static F2X irred;
	ValType val[CIEL(N,8*sizeof(ValType))];
	GF2 getCoefficient(DegType i) const;
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
	F2XE<N>& fromStdRepr(const F2X& n);


	// Sets the irreducible used by the elements of the field of this size to be
	// x^N + irred.
	static void setIrred(const F2X& irred);


};

template<>
F2XE<64>& F2XE<64>::operator*=(const F2XE<64>& a);

template<>
F2XE<64> F2XE<64>::operator* (const F2XE<64>& a) const;


template<unsigned int N>
F2X  F2XE<N>::irred = F2X();

template <unsigned int N>
F2XE<N> F2XE<N>::irred_stripped = F2XE<N>(F2XE<N>());



template<unsigned int N>
void F2XE<N>::setIrred(const F2X& irred)
{
	F2XE<N>::irred = irred;
    F2XE<N>::irred_stripped.fromStdRepr(irred);
    F2XE<N>::irred.flipCoefficient(N);
}


template<unsigned int N>
F2XE<N>::F2XE(){}

template<unsigned int N>
F2XE<N>::F2XE(const F2XE<N>& a)
{
	memcpy(this->val, a.val, CIEL(N, this->bitsInEntry) * sizeof(ValType));
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
	return *this;
}




template<unsigned int N>
F2XE<N>& F2XE<N>::operator*=(const F2XE<N>& a)
{
    F2X t = (this->toStdRepr() * a.toStdRepr()) % this->irred;
    this->fromStdRepr(t);
	return *this;
}

template<unsigned int N>
F2XE<N> F2XE<N>::operator *(const F2XE<N>& a) const
{
    F2XE<N> ans(*this);
	return ans*=a;
}



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
	memset(this->val, 0, this->len * sizeof(this->val[0]));
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
	for (unsigned int i = 0 ; i <= N - 1 ; ++i)
	{
		if (this->getCoefficient(i).val())
		{
			ans.setCoefficient(i, a);
		}
	}
	return ans;
}

template <unsigned int N>
GF2 F2XE<N>::getCoefficient(DegType i) const
{
	auto cellIdx = i / this->bitsInEntry;
	if (cellIdx > this->len)
	{
		return false;
	}
	auto cell = this->val[cellIdx];
	// If i-th bit in the representation is on - return true, else - false.
	return (cell & (1 << (i % this->bitsInEntry))) == 0 ? false : true;
			
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
	F2XE<N> r(this->irred_stripped);
	F2XE<N> newr(*this);
	F2XE<N> newt;
	newt.setUnit();
    F2XE<N> q;
	while (!newr.isZero())
	{
        q.fromStdRepr(r.toStdRepr() / newr.toStdRepr());
		auto tmp = r;
		r = newr;
		newr = tmp + q * newr;
		tmp = t;
		t = newt;
		newt = tmp + q * newt;
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

template<unsigned int N>
F2XE<N>& F2XE<N>::fromStdRepr(const F2X& n)
{
	if (n.getDeg() >= N)
	{
		throw PolynomialDegreeTooHigh();
	}
     memset(this->val, 0, this->len * sizeof(this->val[0]));
    DegType d = n.getDeg();
	for (int i = 0 ; i <= d ; ++i)
	{
        if (n.getCoefficient(i).val())
        {
            this->val[i / this->bitsInEntry] ^= (1 << (i % this->bitsInEntry));
        }
	}
	return *this;
}


#endif /* F2XE_H_ */
