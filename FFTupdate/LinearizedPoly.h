/*
 * LinearizedPoly.h
 *
 *  Created on: Mar 16, 2017
 *      Author: matt
 */

#ifndef LINEARIZEDPOLY_H_
#define LINEARIZEDPOLY_H_

#include <memory>
#include "F2XE.h"

template<unsigned int N>
class LinearizedPoly
{
private:
	int len;
	std::shared_ptr<F2XE<N>> v;
public:
	LinearizedPoly(const F2XE<N>&);
	LinearizedPoly(const LinearizedPoly<N>&);
	virtual ~LinearizedPoly();

	LinearizedPoly<N>& addItem(const F2XE<N>& a);
	F2XE<N> operator()(const F2XE<N>& a);
};

template<unsigned int N>
LinearizedPoly<N>::LinearizedPoly(const F2XE<N>&) : len(2) ,  v(std::shared_ptr<F2XE<N>>(new F2XE<N>[2], [](F2XE<N>* p){delete[] p;}))
{
}

template<unsigned int N>
LinearizedPoly<N>::LinearizedPoly(const LinearizedPoly<N>&a) : len(a.len), v(a.v){}

template<unsigned int N>
LinearizedPoly<N>& addItem(const F2XE<N>& a);
{
}

template<unsigned int N>
F2XE<N> LinearizedPoly<N>::operator()(const F2XE<N>& a)
{
	F2XE<N> n(a);
	F2XE<N> ans;
	if (this->len == 0)
	{
		ans.setZero();
		return ans;
	}
	ans = this->v.get()[0];
	for(int i = 1 ; i < len ; ++i )
	{
		ans += n*this->v.get()[i];
		n.sqr();
	}
}
#endif /* LINEARIZEDPOLY_H_ */
