#pragma once

#include <DejaVu/types.hpp>

namespace djv
{
	namespace kernel
	{
		// Gradient
		const std::array<scp::Mat<float>, 2>& sobel();
		const std::array<scp::Mat<float>, 2>& prewitt();
		std::array<scp::Mat<float>, 2> derivativeOfGaussian(float sigma = 1.f, uint64_t patchSize = 0);

		// Laplacian
		scp::Mat<float> laplacianOfGaussian(float sigma = 1.4f, uint64_t patchSize = 0);

		// Blur
		scp::Mat<float> gaussian(float sigma = 1.f, uint64_t patchSize = 0);
	}

	// Processings

	// Gradient

	// Laplacian

	// Blur
	scp::Mat<float> gaussianBlur(const scp::Mat<float>& m, float sigma = 1.f, uint64_t patchSize = 0);
}
