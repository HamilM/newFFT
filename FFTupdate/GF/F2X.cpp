#include <algorithm>
#include <cstring>
#include "F2X.h"

F2X::F2X() : val(std::vector<ValType>()) {}

DegType F2X::getDeg() const
{
	if (0 == this->val.size())
	{
		return 0;
	}
	auto it = this->val.rbegin();
    while (*it == 0 && it != this->val.rend() ) { ++it; }
    if (it == this->val.rend())
    {
        --it;
    }
    ValType last = *it;
    DegType i = (this->val.rend() - it - 1) * sizeof(ValType)*BITS_IN_BYTE;
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
	//this->resize(a.len);
	this->val = a.val;
	return *this;
}
/*void F2X::resize(unsigned int len)
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
}*/

F2X F2X::operator +(const F2X& a) const
{
	F2X ret(*this);
	ret+=a;
	return ret;
}

F2X& F2X::operator +=(const F2X& a)
{
	for (int i = 0 ;  i < a.val.size() ; ++i)
	{
		this->val[i] ^= a.val[i];
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
{ ;
	auto a_deg = a.getDeg();
	auto this_deg = this->getDeg();
	F2X newPoly(a_deg + this_deg);
	this->resize(CIEL(this_deg +a_deg , this->bitsInEntry));
	for (DegType i =  0 ; i <= this_deg; i++)
	{
		if (!this->getCoefficient(i).val())
		{
			continue;
		}
		for (DegType j =  0 ; j <= a_deg; j++)
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

F2X::F2X(const F2X& a)
{
	*this = a;
}


const ValType& F2X::operator [](size_t index) const
{
	if (index >= this->val.size())
	{
		throw std::exception();
	}
	return this->val[index];
}

ValType& F2X::operator [](size_t index)
{
	if (index >= this->val.size())
	{
        this->resize(index+1);
	}
	return this->val[index];
}

void F2X::flipCoefficient(DegType d)
{
    if (d/this->bitsInEntry >= this->val.size())
    {
        this->resize(d/this->bitsInEntry + 1);
    }
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
	return GF2((bool)((*this)[d/this->bitsInEntry] & (1<< inCellDeg)));
}


void F2X::setCoefficient(DegType d, const GF2& a)
{
	if (a != this->getCoefficient(d))
	{
		this->flipCoefficient(d);
	}
}


F2X::F2X(unsigned int i)
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
	this->resize(std::max((DegType)this->val.size(), (CIEL(this->getDeg() + a, this->bitsInEntry))));
	for (unsigned int i = origDeg + a + 1 ; i >= a + 1 ; --i)
	{
		this->setCoefficient(i - 1, this->getCoefficient(i-a - 1));
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
	unsigned long len = std::min(this->val.size(), a.val.size());
	for (unsigned long i = 0 ; i < len ; ++i)
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
	for(DegType i = this->getDeg() - a  ; i > 0 ; --i)
	{
		this->setCoefficient(i, this->getCoefficient(i+a));
	}
	for (DegType i = origDeg - a + 1; i <= origDeg ; ++i)
	{
		this->setCoefficient(i, 0);
	}
	return *this;
}

F2X::F2X(const std::set<DegType> &coefficients)
{
	if(!coefficients.size())
	{
		return;
	}
	uint32_t len = CIEL(*coefficients.rbegin(), this->bitsInEntry);
	this->val = std::vector<ValType>(len);
    memset(this->val.data(), 0, sizeof(ValType)*len);
	for (auto d : coefficients)
	{
		this->flipCoefficient(d);
	}
}

F2X &F2X::operator%=(const F2X &a)
{
    F2X t(*this);
    while(t.getDeg() >= a.getDeg())
    {
        unsigned int addDeg = t.getDeg() - a.getDeg();
        t += (a << (addDeg));
    }
    *this = t;
    return *this;
}

F2X F2X::operator%(const F2X &a) const {
    F2X result(*this);
    return result%=a;
}

F2X &F2X::resize(unsigned long len)
{
    unsigned long prev_size = this->val.size();
    this->val.resize(len);
    memset(this->val.data() + prev_size, 0, sizeof(this->val[0])*(len - prev_size));
    return *this;
}
