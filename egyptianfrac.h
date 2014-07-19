#ifndef __EGYPTIANFRAC_H__
#define __EGYPTIANFRAC_H__

#include <tuple>
#include <utility>

#include "concepts.h"

// Returns gcd of two integers a and b.
// 
// a and b are integers, and a, b != 0.
template<Integer T>
T gcd(T a, T b) {
	while (b != 0)
		std::tie(a, b) = std::make_pair(b, a % b);
	return a;
}

// Reduce the fraction to the lowest terms.
//
// a and b are integers, b != 0. Returns a1, b1 such that
// a / b == a1 / b1 and gcd(a1, b1) == 1.
template<Integer T>
std::pair<T, T> reduce_fraction(T a, T b) {
	expects(b != 0);
	auto d = gcd(a, b);
	return {a / d, b / d};
}

#endif __EGYPTIANFRAC_H__
