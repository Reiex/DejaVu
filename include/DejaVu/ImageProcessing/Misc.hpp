#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief General purpose image processing functions.
/// \author Reiex
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <DejaVu/types.hpp>

namespace djv
{
	namespace kernel
	{
		const std::array<scp::Mat<float>, 2>& sobel();
		const std::array<scp::Mat<float>, 2>& prewitt();
		std::array<scp::Mat<float>, 2> derivativeOfGaussian(float sigma = 1.f, uint64_t patchSize = 0);

		scp::Mat<float> laplacianOfGaussian(float sigma = 1.4f, uint64_t patchSize = 0);

		scp::Mat<float> gaussian(float sigma = 1.f, uint64_t patchSize = 0);
	}

	namespace operators
	{
		std::array<scp::Mat<float>, 2> sobel(const scp::Mat<float>& m);
		std::array<scp::Mat<float>, 2> prewitt(const scp::Mat<float>& m);
		std::array<scp::Mat<float>, 2> derivativeOfGaussian(const scp::Mat<float>& m, float sigma = 1.f, uint64_t patchSize = 0);

		scp::Mat<float> laplacianOfGaussian(const scp::Mat<float>& m, float sigma = 1.f, uint64_t patchSize = 0);

		scp::Mat<float> gaussianBlur(const scp::Mat<float>& m, float sigma = 1.f, uint64_t patchSize = 0);
	}
}
