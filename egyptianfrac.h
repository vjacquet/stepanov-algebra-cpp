#ifndef __EGYPTIANFRAC_H__
#define __EGYPTIANFRAC_H__

#include <forward_list>
#include <functional>
#include <numeric>
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
template<Integer T>
std::pair<T, T> reduce_fraction(std::pair<T, T> f) {
	return reduce_fraction(f.first, f.second);
}

// Perform one step of egyptian fraction computation.
// 
// Takes integers a and b such that 2 <= a < b
// Returns positive integers(n, a1, b1), such that a / b == 1 / n + a1 / b1
// and n is the smallest such integer.
// 
// # Let q and r be the quotient and remainder when dividing b by a.
// # i.e.qa + r = b.Then, as per the requirements :
// #
// #     1 / (n - 1)  >  a / b         >  1 / n
// #  =>   n - 1  <  b/a         <  n
// #  =>   n - 1  <  (qa + r)/a  <  n
// #  =>   n - 1  <  q + r/a     <  n
// #
// # Therefore, n = q + 1. And,
// #
// #     a1 / b1 = a / b - 1 / n
// #           = (an - b) / bn
// #
// # Since a1 and b1 are the smallest such integers, we need to simplify
// # the fraction by dividing by the GCD of the numerator and the denominator.
template<Integer T>
std::tuple<T, T, T> egyptian_fraction_step(T a, T b) {
	expects(2 <= a && a < b);
	auto n = (b / a) + 1;
	return std::make_tuple(n, a*n - b, b *n);
}

// Convert a rational number to an egyptian fraction.
// 
// Takes integers a and b such that 1 <= a < b.
// Returns a list lst of positive integers representing the egyptian fraction
// such that a / b == sum(1 / x for x in lst)
// 
// The numerator of the remaining fraction after each step is an - b.
// We have n = q + 1, and b = aq + r.r > 0 because if the while condition.
// Therefore,
// 
// an - b = a(q + 1) - (aq + r)
// = aq + a - aq - r
// = a - r < a
// 
// Each step reduces the numerator, and therefore the process must terminate.
template<Integer T>
std::forward_list<T> egyptian_fraction(T a, T b) {
	expects(1 <= a && a < b);
	std::forward_list<T> lst;
	T n;
	while ((b % a) != 0) {
		std::tie(n, a, b) = egyptian_fraction_step(a, b);
		lst.push_front(n);
	}
	lst.push_front(b / a);
	return lst;
}

// Returns a tuple (a1, b1) such that a1/b1 == a/b + 1/n.
template<Integer T>
std::pair<T, T> add_unit_fraction(std::pair<T, T> p, T n) {
	T& a = p.first;
	T& b = p.second;
	return {a * n + b, b*n};
}

// Convert an egyptian fraction to a rational number.
//
// Takes a list lst of positive integers representing an egyptian fraction.
// Returns a tuple of positive integers(a, b) such that
// a / b == sum(1 / x for x in lst) and gcd(a, b) == 1.
template<Integer T>
std::pair<T, T> reduce_egyptian_fraction(std::forward_list<T> const& lst) {
	std::pair<T, T> init = {0, 1};
	return reduce_fraction(std::accumulate(begin(lst), end(lst), init, add_unit_fraction<T>));
}

#endif __EGYPTIANFRAC_H__
