//
// Created by user on 09/06/17.
//

#ifndef NEWFFT_F2XEX_H
#define NEWFFT_F2XEX_H

#include <memory>
#include <Basis.h>
#include <F2XE.h>

template <unsigned int N>
class F2XEX
{
private:
    DegType length;
    std::vector<F2XE<N>> p;
public:
    F2XEX() : p(nullptr) {};
    F2XEX(const F2XEX<N>& p);
    F2XEX(const std::vector<F2XE<N>>& p) : p(p) {};
    ~F2XEX() {};

    // Accessing
    const F2XE<N>& operator[](DegType d) const;
    F2XE<N>& operator[](DegType d);

    // Properties
    DegType getDeg() const;
    bool isZero() const;

    // Operations
    F2XEX<N> operator+ (const F2XEX<N>& a) const;
    F2XEX<N>& operator+= (const F2XEX<N>& a) ;
    F2XEX<N> operator* (const F2XEX<N>& a) const;
    F2XEX<N>& operator*= (const F2XEX<N>& a);

    // Evaluation
    F2XE<N> operator()(const F2XE<N>& e) const;
    std::shared_ptr<std::vector<F2XE<N>>> FFT(const Basis& b) const;

};

template<unsigned int N>
const F2XE<N> &F2XEX::operator[](DegType d) const
{

}

template<unsigned int N>
F2XE<N> &F2XEX::operator[](DegType d) {
    if (this->length <= d)
    {
        throw PolynomialDegreeTooHigh();
    }
    return this->p[d];
}

template<unsigned int N>
DegType F2XEX<N>::getDeg() const {
    for (DegType i = this->length - 1 ; i < i+1 ; --i)
    {
        if (!this->p[i].isZero())
        {
            return i;
        }
    }
    return 0;
}

template<unsigned int N>
F2XEX<N> F2XEX::operator+(const F2XEX<N> &a) const
{
    F2XEX<N> r(*this);
    return r+=a;
}

template<unsigned int N>
F2XEX<N> &F2XEX::operator+=(const F2XEX<N> &a) {
    DegType a_deg = a.getDeg();
    DegType new_deg = std::max(a_deg, this->getDeg());
    p.resize(new_deg + 1);
    for (DegType i = 0 ; i <= a_deg ; ++i)
    {
        (*this)[i] += a[i];
    }
}

template<unsigned int N>
F2XEX<N> F2XEX::operator*(const F2XEX<N> &a) const {
    F2XEX<N> ans;
    DegType a_deg = a.getDeg();
    DegType this_deg = this->getDeg();
    DegType new_deg = a_deg + this_deg;
    ans.p.resize(new_deg + 1);
    for (DegType i = 0 ; i <= a_deg ; ++i)
    {
        for (DegType j = 0; j<= this_deg; ++j)
        {
            ans[i+j] += a[i] * (*this)[j];
        }
    }
    return ans;
}

template<unsigned int N>
F2XEX<N> &F2XEX::operator*=(const F2XEX<N> &a)
{
    (*this) = (*this) * a;
    return *this;
}

template<unsigned int N>
F2XE<N> F2XEX::operator()(const F2XE<N> &e) const
{
    F2XE<N> x;
    F2XE<N> ans;
    ans.setZero();
    x.setUnit();
    DegType d = this->getDeg();
    for (int i = 0 ; i < d ; ++i)
    {
        ans += x * (*this)[i];
        x *= e;
    }
    return ans;
}

template<unsigned int N>
F2XEX::F2XEX(const F2XEX<N> &p)
{
    this->F2XEX(p.p);
}

template <unsigned int N>
bool F2XEX<N>::isZero() const
{
    return (0 == this->getDeg() && (0 == this->p.size() || (*this)[0].isZero()));
}

template<unsigned int N>
std::shared_ptr<std::vector<F2XE<N>>> F2XEX<N>::FFT(const Basis &b) const
{
    //we're assuming for now that the number of elements in b > degree of this.
    std::shared_ptr<std::vector<F2XE<N>>> ans = std::make_shared(std::vector<F2XE<N>>());
    std::vector<F2XE<N>>& eval = *(ans.get());
    eval.resize((uint32_t)1<<b.getSize());
}


#endif //NEWFFT_F2XEX_H
