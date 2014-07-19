#include "../egyptianfrac.h"

#include "testbench.h"

TESTBENCH()

TEST(can_compute_gcd) {
	int a = 75;
	int b = 45;
	VERIFY_EQ(15, gcd(75, 45));
}

TEST(can_reduce_fraction) {
	auto fraction = reduce_fraction(75, 45);
	VERIFY_EQ(5, fraction.first);
	VERIFY_EQ(3, fraction.second);
}

TESTFIXTURE(egyptianfrac)
