#pragma once

#include <DejaVu/types.hpp>

namespace djv
{
	scp::Mat<float> marrHildrethEdgeDetector(scp::Mat<float> m, float sigma0, float scaleFactor);
}
