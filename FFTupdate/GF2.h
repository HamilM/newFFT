/*
 * GF2.h
 *
 *  Created on: Mar 5, 2017
 *      Author: matt
 */

#ifndef GF2_H_
#define GF2_H_
#include <stdbool.h>
class GF2 {
private:
	bool v;
public:
	GF2();
	GF2(bool c);
	GF2(const GF2& g);
	GF2& operator= (const GF2&);
	bool operator== (const GF2&) const;
	bool operator!= (const GF2&) const;
	bool val() const;
	GF2 operator!() const;
	~GF2();
};

#endif /* GF2_H_ */
