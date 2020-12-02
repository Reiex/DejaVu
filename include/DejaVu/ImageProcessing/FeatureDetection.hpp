#pragma once

#include <DejaVu/types.hpp>

namespace djv
{
	namespace edgeDetector
	{
		scp::Mat<float> sobel(const scp::Mat<float>& m);
		scp::Mat<float> prewitt(const scp::Mat<float>& m);

		scp::Mat<float> marrHildreth(const scp::Mat<float>& m, float sigma = 1.5f);
		scp::Mat<float> canny(const scp::Mat<float>& m, float sigma = 1.5f);
	}

	namespace lineExtractor
	{
		std::vector<Line> hough(const scp::Mat<float>& m, float threshold = 0.5f, float dTheta = 0.1f, float dRho = 5.f);
	}

}
