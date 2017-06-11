#include <algorithm>
#include <gtest/gtest.h>
#include <GF2.h>
#include <F2X.h>
#include <F2XE.h>
#include <Basis.h>
#include <LinearizedPoly.h>
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
	ASSERT_TRUE(d == a+a);
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

TEST(F2XETests, MultiplyingSmallField)
{
    F2XE<3> a;
    F2XE<3> b;
    std::set<DegType> c_a{1,2};
    std::set<DegType> c_b{0,1};
    std::set<DegType> c_irred{0,1};
    F2X p_a(c_a);
    F2X p_b(c_b);
    F2X p_irred(c_irred);
    a.fromStdRepr(p_a); //<- Creating x + x^2.
    b.fromStdRepr(p_b); // <- Creating 1 + x.
    F2XE<3>::setIrred(p_irred); // <- Irreducible is x^3 + x + 1.
    F2XE<3> c = a*b;
    F2X p_c = c.toStdRepr();
    ASSERT_TRUE(p_c.getCoefficient(0).val());
    for (int i = 1 ; i < 3 ; ++i)
    {
        ASSERT_FALSE(p_c.getCoefficient(i).val());
    }
}

TEST(F2XETests, MultiplyBiggerField)
{
    F2XE<64> a;
    F2XE<64> b;
    std::set<DegType> c_a{10, 63};
    std::set<DegType> c_b{1, 40};
    std::set<DegType> c_irred{0,1,3,4};
    F2X p_a(c_a);
    F2X p_b(c_b);
    F2X p_irred(c_irred);
    a.fromStdRepr(c_a);
    b.fromStdRepr(c_b);
    F2XE<64>::setIrred(p_irred);
    F2XE<64> c = a*b;
    F2X p_c = c.toStdRepr();
    std::set<DegType> expected_coeffs{0,1,3,4,11,39,40,42,43,50};
    for (int i = 0 ; i < 64 ; ++i)
    {
        if (expected_coeffs.find(i) == expected_coeffs.end())
        {
            ASSERT_FALSE(p_c.getCoefficient(i).val());
        }
        else
        {
            ASSERT_TRUE(p_c.getCoefficient(i).val());
        }
    }
}

TEST (BasisTests, BasisQueryTests)
{
    std::set<DegType> c_a{1};
    std::set<DegType> c_b{35};
    std::set<DegType> c_c{13};
    std::vector<F2XE<64>> basis_elements;
    Basis<F2XE<64>> b;
    b.add(F2XE<64>(c_a));
    b.add(F2XE<64>(c_b));
    b.add(F2XE<64>(c_c));

    ASSERT_TRUE(b[0].isZero());
    ASSERT_EQ(b[1], F2XE<64>(c_a));
    ASSERT_EQ(b[2], F2XE<64>(c_b));
    ASSERT_EQ(b[4], F2XE<64>(c_c));
    ASSERT_EQ(b[1]+b[2], b[3]);
    ASSERT_EQ(b[1] + b[4], b[5]);
}


TEST(LinearizedPolyTests, LinearizedPolyEvaluationTest)
{
    // We create a linearized polynomial for a basis.
    std::set<DegType> c_a{1};
    std::set<DegType> c_b{2};
    std::set<DegType> c_c{3};
    std::set<DegType> c_d{4};

    LinearizedPoly<64> lp;

    ASSERT_TRUE(lp(F2XE<64>().setZero()).isZero());
    ASSERT_FALSE(lp(F2XE<64>(c_a)).isZero());

    lp.addItem(F2XE<64>(c_a));
    lp.addItem(F2XE<64>(c_b));
    lp.addItem(F2XE<64>(c_c));

    Basis<F2XE<64>> b;
    b.add(F2XE<64>(c_a));
    b.add(F2XE<64>(c_b));
    b.add(F2XE<64>(c_c));

    for (int i = 0 ; i < 1<<3 ; ++i)
    {
        ASSERT_TRUE(lp(b[i]).isZero());
    }
    ASSERT_FALSE(lp(F2XE<64>(c_d)).isZero());
}


