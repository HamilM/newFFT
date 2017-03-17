/*
 * F2XE.cpp
 *
 *  Created on: Mar 1, 2017
 *      Author: matt
 */

#include "F2XE.h"

__m128i clmul_mask = {-1, 0};

template<>
F2XE<64> F2XE<64>::operator* (const F2XE<64>& a) const
{
	F2XE<64> r(a);
	return r*=(*this);
}

template<>
F2XE<64>& F2XE<64>::operator*=(const F2XE<64>& a)
{
	register __m128i l = _mm_loadu_si128((__m128i*)this->irred->val);
	register __m128i t;
	t = _mm_clmulepi64_si128(
			_mm_loadu_si128((__m128i*)a.val),
			_mm_loadu_si128((__m128i*)this->val),
			0);
	t = _mm_xor_si128(_mm_clmulepi64_si128(t,l,1), _mm_and_si128(t,clmul_mask));
	t = _mm_xor_si128(_mm_clmulepi64_si128(t,l,1), t);
	_mm_storel_epi64((__m128i*)this->val, t);
	return *this;
}
