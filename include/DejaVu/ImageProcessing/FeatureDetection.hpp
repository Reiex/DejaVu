#pragma once

#include <DejaVu/types.hpp>

namespace djv
{
	namespace edgeDetector
	{
		scp::Mat<float> sobel(const scp::Mat<float>& m, float threshold = 0.5f);
		scp::Mat<float> prewitt(const scp::Mat<float>& m, float threshold = 0.5f);

		scp::Mat<float> marrHildreth(const scp::Mat<float>& m, float sigma0 = 1.f, float scaleFactor = 2.f, float threshold = 1e-3f);
		scp::Mat<float> canny(const scp::Mat<float>& m, float sigma0 = 1.f, float scaleFactor = 2.f, float threshold = 1e-3f);
	}

	namespace lineExtractor
	{
		std::vector<Line> hough(const scp::Mat<float>& m, float threshold = 0.5f, float dTheta = 0.1f, float dRho = 5.f);
	}

}
