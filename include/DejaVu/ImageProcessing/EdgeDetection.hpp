#pragma once

#include <DejaVu/types.hpp>

namespace djv
{
	scp::Mat<float> laplacianOfGaussian(scp::Mat<float> m, float sigma = 1.4f, uint64_t patchSize = 0);
}
