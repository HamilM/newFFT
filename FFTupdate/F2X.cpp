#include "F2X.h"
#include <cstring>


F2X::F2X() : len(0), val(nullptr) {}


// Copies the content of a.
F2X& F2X::operator =(const F2X& a)
{
	this->resize(a.len);
	memcpy(this->val.get(), a.val.get(), sizeof(ValType)*a.len);
}
void F2X::resize(unsigned int len)
{
	std::shared_ptr<ValType> new_array = std::shared_ptr<ValType>(new ValType[len], [](ValType* p){delete[] p;});
	if (len > this->len)
	{
		if (this->len > 0)
		{
			memcpy(new_array.get(), this->val.get(), this->len * sizeof(ValType));
		}
		memset(new_array.get()+this->len, 0, sizeof(ValType)*(len - this->len));
	}
	else if (len < this->len)
	{
		memcpy(new_array.get(), this->val.get(), len * sizeof(ValType));
	}
	this->len = len;
}

F2X F2X::operator +(const F2X& a) const
{
	F2X ret(*this);
	ret+=a;
	return ret;
}

F2X& F2X::operator +=(const F2X& a)
{
	if (a.len > this->len)
	{
		this->resize(a.len);
	}
	for (int i = 0 ;  i < a.len ; ++i)
	{
		this->val.get()[i] ^= a.val.get()[i];
	}
}

F2X F2X::operator *(const F2X& a) const
{

}

F2X& F2X::operator *=(const F2X& a)
{
	auto this_len = this->getFitLen();
	auto a_len = a.getFitLen();
	F2X newPoly(a_len + this_len);
	this->resize(this->getFitLen() + a.getFitLen());
	for (DegType i = 0 ; i < a_len ; ++i)
	{
	for (DegType ii = 0 ; ii < BITS_IN_BYTE ; ++ii)
	{
	for (DegType j = 0 ; j < this_len ; ++j)
	{
	for (DegType jj = 0 ; jj < BITS_IN_BYTE ; ++jj)
	{
		ValType a_val = a[i]
	}
	}
	}
	}


}

F2X F2X::operator /(const F2X& a) const
{
}



F2X& F2X::operator /=(const F2X& a)
{
}

F2X::F2X(const F2X& a) : len(0), val(nullptr)
{
	*this = a;
}

DegType F2X::deg() const
{
	if (0 == this->len)
	{
		return 0;
	}
	DegType i = this->getFitLen() - 1;
	DegType last = (*this)[i];
	i*= sizeof(ValType)*BITS_IN_BYTE;
	last>>=1;
	while (0  != last)
	{
		i++;
		last>>=1;
	}

}

void F2X::fit()
{
	int i = this->len - 1;
	while((*this)[i] == 0 && i > 0) { --i; };
	this->resize(i+1);
}

DegType F2X::getFitLen() const
{
	int i = this->len - 1;
	while((*this)[i] == 0 && i > 0) { --i; };
	return i+1;
}

const ValType& F2X::operator [](size_t index) const
{
	if (index >= this->len)
	{
		throw std::exception();
	}
	return this->val.get()[index];
}

ValType& F2X::operator [](size_t index)
{
	if (index >= this->len)
	{
		throw std::exception();
	}
	return this->val.get()[index];
}

void F2X::flipCoefficient(DegType d)
{
	this->setCoefficient(d,)
}

void F2X::setCoefficient(DegType d, int)
{
}

DegType F2X::getCoefficient(DegType d)
{
	auto inCellDeg = d % this->bitsInEntry;
	return ((*this)[d/this->bitsInEntry] & (1<< inCellDeg)) >> (1<< inCellDeg);
}

F2X::F2X(unsigned int i) : len (0), val(nullptr)
{
	this->resize(i);
}
