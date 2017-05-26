/*
 * Basis.h
 *
 *  Created on: Mar 16, 2017
 *      Author: matt
 */

#ifndef BASIS_H_
#define BASIS_H_
#include <memory>
#include "F2XE.h"

template <unsigned int N>
class Basis
{
private:
	unsigned int size;
	std::shared_ptr<F2XE<N>> e;
public:
	Basis(std::shared_ptr<F2XE<N>>& b, unsigned int l);
	unsigned int getSize() const { return this->size; }
	virtual ~Basis(){};
	F2XE<N> operator[](unsigned int i) const;
};

template<unsigned int N>
Basis<N>::Basis(std::shared_ptr<F2XE<N>>& b, unsigned int i) : size(i), e(std::shared_ptr<F2XE<N>>(b), [](F2XE<N>* p){delete[] p;}){}

template<unsigned int N>
F2XE<N> Basis<N>::operator [](unsigned int i) const
{
	i = i & ((1<<this->l) - 1);
	F2XE<N> ans;
	int j = 0;
	while(i>0)
	{
		if (i&1)
		{
			ans += e.get()[j];
		}
		j++;
	}
	return ans;
}
#endif /* BASIS_H_ */
