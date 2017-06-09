/*
 * Basis.h
 *
 *  Created on: Mar 16, 2017
 *      Author: matt
 */

#ifndef BASIS_H_
#define BASIS_H_
#include <memory>
#include <vector>
#include "F2XE.h"

template <typename T>
class Basis
{
private:
	std::vector<T> e;
public:
    Basis(){};
	Basis(const std::vector<T>& b);
	unsigned long getSize() const { return e.size(); }
    Basis& add(const T& a);
	virtual ~Basis(){};
	T operator[](unsigned int i) const;
};


template <typename T>
T Basis<T>::operator [](unsigned int i) const
{
	i = i & ((1<<e.size()) - 1);
	T ans;
	int j = 0;
	while(i>0)
	{
		if (i&1)
		{
			ans += e[j];
		}
		j++;
		i >>=1 ;
	}
	return ans;
}

template <typename T>
Basis<T>::Basis(const std::vector<T> &b) : e(b) {}

template <typename T>
Basis<T> &Basis<T>::add(const T &a)
{
    e.push_back(a);
    return *this;
};

#endif /* BASIS_H_ */
