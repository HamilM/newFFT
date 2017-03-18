/*
 * FFT.h
 *
 *  Created on: Mar 17, 2017
 *      Author: matt
 */

#ifndef FFT_H_
#define FFT_H_
#include "Basis.h"
#include "LinearizedPoly.h"

template<unsigned int N>
class FFT {
private:
	std::shared_ptr<Basis<N>> b;
	F2XE<N> shift;
	std::shared_ptr<std::shared_ptr<F2XE<N>>> w;
public:
	FFT(const std::shared_ptr<Basis<N>>& a, const F2XE<N>& s);
	virtual ~FFT(){};
};

template<unsigned int N>
FFT<N>::FFT(const std::shared_ptr<Basis<N>>& a, const F2XE<N>& s) : b(a), shift(s)
{

	//We now prepare the auxilliary arrays of Wi,j.
	unsigned int bLen = this->b->getSize();
	std::shared_ptr<F2XE<N>>* w = new std::shared_ptr<F2XE<N>>[bLen];
	LinearizedPoly<N> t((*this->b)[1]);
	for (unsigned int i = 0 ; i < bLen ; ++i)
	{
		w[i] = std::shared_ptr<F2XE<N>>(new F2XE<N>[bLen-1-i], [](F2XE<N>* p){delete[] p;});
		//Make evaluations of t at specific points.
		for (unsigned int j = 0 ; j < 1<<(bLen -1 - i) ; ++j)
		{
			w[i].get()[j] = t(this->shift + (*this->b)[2*j]) / (*this->b[1<<(i)]);
		}
		t.addItem((*this->b)[1<<(i)]);
	}
	this->w = std::shared_ptr<std::shared_ptr<F2XE<N>>>(w, [](std::shared_ptr<F2XE<N>>* p){delete[] p;});
}
#endif /* FFT_H_ */
