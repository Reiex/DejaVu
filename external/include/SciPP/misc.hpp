#pragma once

#include <SciPP/types.hpp>

namespace scp
{
	const long double pi = 3.141592653589793239;
	const long double e = 2.718281828459045235;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \brief Computes x exponent n for n greater or equal to 0.
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename TBase, typename TPow = uint64_t>
	TBase expoSq(TBase const& x, TPow n);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \brief Computes the greatest common divisor (GCD) of a and b.
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T>
	T gcd(const T& a, const T& b);
}

#include <SciPP/miscT.hpp>