#pragma once

#include <DejaVu/types.hpp>

namespace djv
{
	scp::Mat<float> marrHildrethEdgeDetector(scp::Mat<float> m, float sigma0 = 1.f, float scaleFactor = 2.f, float threshold = 1e-3f);
}
