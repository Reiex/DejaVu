#pragma once

#include <DejaVu/types.hpp>

namespace djv
{
	scp::Mat<float> marrHildrethEdgeDetector(scp::Mat<float> m, float sigma0 = 1.f, float scaleFactor = 1.5f, float threshold = 0.001f);
}
