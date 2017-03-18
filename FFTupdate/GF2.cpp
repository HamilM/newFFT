/*
 * GF2.cpp
 *
 *  Created on: Mar 5, 2017
 *      Author: matt
 */

#include "GF2.h"

GF2::GF2(): v(0) {}

GF2::GF2(bool c) : v(c) {}

GF2::GF2(const GF2& g) : v(g.v) {}

GF2& GF2::operator =(const GF2& a)
{
	this->v = a.v;
	return *this;
}

bool GF2::operator ==(const GF2& a) const
{
	return this->v == a.v;
}

GF2 GF2::operator !() const
{
	return GF2(!this->v);
}

bool GF2::operator !=(const GF2& a) const
{
	return !(this->operator==(a));
}

GF2::~GF2() {
}

bool GF2::val() const
{
	return this->v;
}
