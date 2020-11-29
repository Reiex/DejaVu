#pragma once

#include <DejaVu/types.hpp>

namespace djv
{
	// Edge detectors

	scp::Mat<float> marrHildrethEdgeDetector(const scp::Mat<float>& m, float sigma0 = 1.f, float scaleFactor = 2.f, float threshold = 1e-3f);

	// Edge extractors

	std::vector<Line> houghTransform(const scp::Mat<float>& m, float dTheta = 5e-2f, float dRho = 10.f, float threshold = 0.6f);
}
