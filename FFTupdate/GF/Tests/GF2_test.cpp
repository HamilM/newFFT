#include <GF2.h>
#include <F2X.h>
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
	ASSERT_THROW({
		a.setCoefficient(5, b);
		}, std::exception);
}
