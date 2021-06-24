#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <complex>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace scp
{
	class Int;
	template<typename T> class Frac;
	class Rational;
	template<typename T> struct Quaternion;
	template<typename T> class Polynomial;

	template<uint64_t N> struct TensorPosition;
	template<uint64_t N> class TensorIterator;
	template<typename T, uint64_t N> class Tensor;
	enum class ConvolutionMethod;
	template<typename T> class Mat;
	template<typename T> class Vec;
}
