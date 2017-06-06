#include <algorithm>
#include <cstring>
#include "F2X.h"


F2X::F2X() : len(0), val(nullptr) {}

DegType F2X::getDeg() const 
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
	return i;
}


// Copies the content of a.

F2X& F2X::operator =(const F2X& a)
{
	this->resize(a.len);
	memcpy(this->val.get(), a.val.get(), sizeof(ValType)*a.len);
	return *this;
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
	else
	{
		return;
	}

	this->val = new_array;
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
	return *this;
}

F2X F2X::operator *(const F2X& a) const
{
	F2X ans(*this);
	ans*=a;
	return ans;
}

F2X& F2X::operator *=(const F2X& a)
{
	auto this_len = this->getFitLen();
	auto a_len = a.getFitLen();
	auto a_deg = a.getDeg();
	auto this_deg = this->getDeg();
	F2X newPoly(a.getDeg() + this->getDeg());
	this->resize(this->getFitLen() + a.getFitLen());
	for (int i =  0 ; i <= this_deg; i++)
	{
		if (this->getCoefficient(i).val()==false)
		{
			continue;
		}
		for (int j =  0 ; j <= a_deg; j++)
		{
			if(this->getCoefficient(i)==true && a.getCoefficient(j)==true)
			{
				newPoly.flipCoefficient(i+j);
			}
		}
	}
	(*this)=newPoly;
	return *this;
}

F2X F2X::operator /(const F2X& a) const
{
	F2X ans(*this);
	ans/=a;
	return ans;
}



F2X& F2X::operator /=(const F2X& a)
{
	F2X ans(this->getDeg());
	F2X t(*this);
	while(t.getDeg() >= a.getDeg())
	{
		unsigned int addDeg = t.getDeg() - a.getDeg();
		t += (a << (addDeg));
		ans.flipCoefficient(addDeg);
	}
	*this = ans;
	return *this;
}

F2X::F2X(const F2X& a) : len(0), val(nullptr)
{
	*this = a;
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
		this->resize(index+1);
	}
	return this->val.get()[index];
}

void F2X::flipCoefficient(DegType d)
{
	auto inCellDeg = d % this->bitsInEntry;
	auto currentCell = (*this)[d/this->bitsInEntry];
	currentCell = currentCell ^ ((1<< inCellDeg));
	(*this)[d/this->bitsInEntry] = currentCell;
}

GF2 F2X::getCoefficient(DegType d) const
{
	if(this->getDeg() < d)
	{
		return GF2(false);
	}
	auto inCellDeg = d % this->bitsInEntry;
	return GF2(((*this)[d/this->bitsInEntry] & (1<< inCellDeg)) >> (inCellDeg));
}


void F2X::setCoefficient(DegType d, const GF2& a)
{
	if (a != this->getCoefficient(d))
	{
		this->flipCoefficient(d);
	}
}


F2X::F2X(unsigned int i) :  val(nullptr), len(0)
{
	this->resize(CIEL(i,this->bitsInEntry));
}

F2X F2X::operator <<(unsigned int a) const
{
	F2X ans(*this);
	ans <<= a;
	return ans;
}

const F2X& F2X::operator <<=(unsigned int a)
{
	unsigned int origDeg = this->getDeg();
	this->resize(std::max(this->len, (unsigned int)(CIEL(this->getDeg() + a, this->bitsInEntry))));
	for (unsigned int i = origDeg + a  ; i >= a ; --i)
	{
		this->setCoefficient(i, this->getCoefficient(i-a));
	}
	GF2 t(0);
	for(unsigned int i = 0 ; i < a ; ++i)
	{
		this->setCoefficient(i, t);
	}
	return *this;
}

F2X F2X::operator >>(unsigned int a) const
{
	F2X ans(*this);
	ans >>= a;
	return ans;
}

bool F2X::operator ==(const F2X& a) const
{
	if (this->getDeg() != a.getDeg())
	{
		return false;
	}
	int len = std::min(this->len, a.len);
	for (int i = 0 ; i < len ; ++i)
	{
		if ((*this)[i] != a[i])
		{
			return false;
		}
	}
	return true;
}

bool F2X::operator !=(const F2X& a) const
{
	return !this->operator ==(a);
}

const F2X& F2X::operator >>=(unsigned int a)
{
	unsigned int origDeg = this->getDeg();
	for(int i = this->getDeg() - a  ; i > 0 ; --i)
	{
		this->setCoefficient(i, this->getCoefficient(i+a));
	}
	for (int i = origDeg - a + 1; i <= origDeg ; ++i)
	{
		this->setCoefficient(i, 0);
	}
	this->fit();
	return *this;
}

F2X::F2X(std::set<DegType> &coefficients) : len(0), val(NULL)
{
	if(!coefficients.size())
	{
		return;
	}
	this->len = CIEL(*coefficients.rbegin(), this->bitsInEntry);
	this->val = std::shared_ptr<ValType>(new ValType[len], [](ValType* p){delete[] p;});
	for (auto d : coefficients)
	{
		this->flipCoefficient(d);
	}
}
