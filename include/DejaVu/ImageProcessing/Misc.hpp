#pragma once

#include <DejaVu/types.hpp>

namespace djv
{
	// Kernels

	// Gradient

	const std::array<scp::Mat<float>, 2>& sobelKernel();
	std::array<scp::Mat<float>, 2> derivativeOfGaussianKernel(float sigma = 1.f, uint64_t patchSize = 0);

	// Laplacian

	scp::Mat<float> laplacianOfGaussianKernel(float sigma = 1.4f, uint64_t patchSize = 0);

	// Blur

	scp::Mat<float> gaussianKernel(float sigma = 1.f, uint64_t patchSize = 0);


	// Processings

	// Gradient

	// Laplacian

	// Blur

	scp::Mat<float> gaussianBlur(const scp::Mat<float>& m, float sigma = 1.f, uint64_t patchSize = 0);
}
