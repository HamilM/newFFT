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
	F2X a = F2X();
	GF2 b(true);
	a.setCoefficient(5, b);
	a.setCoefficient(2, b);
	ASSERT_EQ(a.getCoefficient(5), b);
}
