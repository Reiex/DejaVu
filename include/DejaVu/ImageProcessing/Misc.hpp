#pragma once

#include <DejaVu/types.hpp>

namespace djv
{
	// Gradient

	std::array<scp::Mat<float>, 2> sobelGradient(const scp::Mat<float>& m);

	// Blur

	scp::Mat<float> gaussianBlur(const scp::Mat<float>& m, float sigma = 1.f, uint64_t patchSize = 0);
}
