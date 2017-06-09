/*
 * LinearizedPoly.h
 *
 *  Created on: Mar 16, 2017
 *      Author: matt
 */

#ifndef LINEARIZEDPOLY_H_
#define LINEARIZEDPOLY_H_

#include <memory>
#include <vector>
#include "F2XE.h"

template<unsigned int N>
class LinearizedPoly
{
private:
	std::vector<F2XE<N>> v;
public:
	LinearizedPoly();
	LinearizedPoly(const LinearizedPoly<N>&);

	virtual ~LinearizedPoly(){};

	LinearizedPoly<N>& addItem(const F2XE<N>& a);
	F2XE<N> operator()(const F2XE<N>& a);
};

template<unsigned int N>
LinearizedPoly<N>::LinearizedPoly()
{
	this->v.push_back(F2XE<N>().setUnit());
}

template<unsigned int N>
LinearizedPoly<N>::LinearizedPoly(const LinearizedPoly<N>&a)
{
	this->v = a.v;
}

template<unsigned int N>
LinearizedPoly<N>& LinearizedPoly<N>::addItem(const F2XE<N>& a)
{
	auto newLen = this->v.size() + 1;
	std::vector<F2XE<N>> newPoly(newLen);
	for (int i = 0 ; i < this->v.size(); ++i)
	{
		newPoly[i+1] = this->v[i];
		newPoly[i+1].sqr();
	}
	F2XE<N> multiplier = this->operator()(a);
	for (int i = 0 ; i < this->v.size(); ++i)
	{
		newPoly[i] += this->v[i]*multiplier;
	}
	this->v = newPoly;
	return *this;
}

template<unsigned int N>
F2XE<N> LinearizedPoly<N>::operator()(const F2XE<N>& a)
{
	F2XE<N> n(a);
	F2XE<N> ans;
	if (this->v.size() == 0)
	{
		ans.setZero();
		return ans;
	}
	//ans = this->v[0];
	for(int i = 0 ; i < v.size() ; ++i )
	{
		ans += n*this->v[i];
		n.sqr();
	}
}
#endif /* LINEARIZEDPOLY_H_ */
