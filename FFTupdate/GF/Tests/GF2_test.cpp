#include <GF2.h>
#include <F2X.h>
#include <F2XE.h>
#include <algorithm>
#include <gtest/gtest.h>

TEST(GF2Tests, TestAddition)
{
	ASSERT_NO_THROW({
	GF2 a(true);	
	GF2 c(true);	
	GF2 b(false);
	ASSERT_NE(a,b);
	ASSERT_EQ(c,a);
	});
}

TEST(F2XTests, GetCoefficient)
{
	ASSERT_NO_THROW({
	F2X a = F2X(6);
	GF2 b(true);
	a.setCoefficient(5, b);
	ASSERT_EQ(a.getCoefficient(5), b);});
}

TEST(F2XTests,  OffLimitsUsage)
{
	// Creating a polynomial of degree 4.
	F2X a (4);
	GF2 b(true);
	// Trying to modify the coefficient of x^5, should throw an exception!
	ASSERT_NO_THROW(a.setCoefficient(5, b));
	ASSERT_EQ(a.getCoefficient(5), b);
}
TEST(F2XTests, Addition)
{
	F2X a(4);
	a.flipCoefficient(2);
	a.flipCoefficient(1);
	F2X b(4);
	b.flipCoefficient(2);
	b.flipCoefficient(0);
	F2X c(4);
	c.flipCoefficient(0);
	c.flipCoefficient(1);
	F2X d;
	ASSERT_EQ(c, a+b);
	ASSERT_EQ(d, a+a);
	a.flipCoefficient(100);
	b.flipCoefficient(101);
	c.flipCoefficient(100);
	c.flipCoefficient(101);
	ASSERT_EQ(c, a+b);
}
TEST(F2XTests, ShiftLeft)
{
	F2X a(4);
	a.flipCoefficient(1);
	F2X b(4);
	b.flipCoefficient(2);
	F2X c = a << 1;
	ASSERT_EQ(b, c);
	a.flipCoefficient(2);
	b.flipCoefficient(3);
	ASSERT_EQ(b, a<<1);
	F2X d;
	d.flipCoefficient(100);
	d.flipCoefficient(101);
	ASSERT_EQ(d, a<<99);
}


TEST(F2XTests, ShiftRight)
{
	F2X a(4);
	a.flipCoefficient(1);
	F2X b(4);
	b.flipCoefficient(2);
	F2X c = b >> 1;
	ASSERT_EQ(a, c);
	a.flipCoefficient(2);
	b.flipCoefficient(3);
	ASSERT_EQ(b>>1, a);
	F2X d;
	d.flipCoefficient(100);
	d.flipCoefficient(101);
	ASSERT_EQ(d>>99, a);
}

TEST(F2XTests, GetDeg)
{
	F2X a(4);
	a.flipCoefficient(4);
	ASSERT_EQ(a.getDeg(), 4);
	a.flipCoefficient(1000);
	ASSERT_EQ(a.getDeg(), 1000);
}
TEST(F2XTests, Multiplication)
{
	F2X a(4);
	a.flipCoefficient(2);
	F2X b(4);
	b.flipCoefficient(3);
	F2X c(5);
	c.flipCoefficient(5);
	ASSERT_EQ(a*b, c);
	a = F2X(100);
	b = F2X(100);
	a.flipCoefficient(100);
	b.flipCoefficient(100);
	a.flipCoefficient(101);
	b.flipCoefficient(101);
	c = F2X(200);
	c.flipCoefficient(200);
	c.flipCoefficient(202);
	ASSERT_EQ(c, a*b);
}

TEST(F2XTests, Div)
{
	F2X a(4);
	a.flipCoefficient(3);
	F2X b(4);
	b.flipCoefficient(2);
	F2X c(4);
	c.flipCoefficient(1);
	ASSERT_EQ(a/b, c);
	a = F2X(100);
	a.flipCoefficient(100);
	b = F2X(90);
	b.flipCoefficient(50);
	ASSERT_EQ(a/b, b);
}

TEST(F2XTests, CreateFromSet)
{
	std::set<DegType> coefficients{1,2,10,100,200,450};
	F2X p(coefficients);
	for(DegType d : coefficients)
	{
		ASSERT_TRUE(p.getCoefficient(d).val());
	}
	ASSERT_EQ(p.getDeg(), *coefficients.rbegin());
}

TEST(F2XETests, UsingStdRepr)
{
    // We create an element in GF(2^63).
	F2XE<63> a = F2XE<63>();

    // Nullify it, just to be sure.
	a.setZero();

    // Create a polinomial, will be of degree 12.
	F2X poly(12);

    // Setting the polynomial to be x^1 + x^3 + x^6 + x^10.
	DegType coefficients[] = {1,3,6,10};
	for (int i = 0 ; i < 4 ; ++i)
	{
		poly.setCoefficient(coefficients[i], GF2(true));
	}

    a.fromStdRepr(poly);
    F2X n = a.toStdRepr();
	for (int i = 0 ; i < 4 ; ++i)
	{
		ASSERT_TRUE(n.getCoefficient(coefficients[i]).val()) << "Coefficient is not set!";
	}
	for (int i = 0 ; i < 12 ; ++i)
	{
		if (std::find(coefficients, coefficients + 4, i) == coefficients + 4)
		{
			ASSERT_FALSE(n.getCoefficient(i).val()) << "Coefficient is set!";
		}
	}
}

