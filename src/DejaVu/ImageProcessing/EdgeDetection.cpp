#include <DejaVu/DejaVu.hpp>

namespace djv
{
	scp::Mat<float> marrHildrethEdgeDetector(scp::Mat<float> m, float sigma0, float scaleFactor, float threshold)
	{
		scp::Mat<float> r(m.m, m.n), gPreced(m.m, m.n);
		float sigma = sigma0;
		std::array<scp::Mat<float>, 2> gradKernel = { scp::Mat<float>{ { {-1.f, 0.f, 1.f} } }, scp::Mat<float>{ { {-1.f}, {0.f}, {1.f} } } };
		scp::Mat<float> g = gaussianBlur(m, sigma);
		sigma *= scaleFactor;
		uint64_t scaleCount(1);

		while (4*sigma < m.m/2 && 4*sigma < m.n/2)
		{
			scaleCount++;
			gPreced = g;
			g = gaussianBlur(m, sigma);

			scp::Mat<float> dog = g - gPreced;
			std::array<scp::Mat<float>, 2> grad = { scp::convolve(g, gradKernel[0]), scp::convolve(g, gradKernel[1]) };
			scp::Mat<float> gradMag = scp::hadamardProduct(grad[0], grad[0]) + scp::hadamardProduct(grad[1], grad[1]);
			
			for (uint64_t i(0); i < dog.m - 1; i++)
			{
				for (uint64_t j(0); j < dog.n - 1; j++)
				{
					if ((dog[i][j] * dog[i+1][j] < 0 || dog[i][j] * dog[i][j+1] < 0) && gradMag[i][j] > threshold)
					{
						r[i][j] += 1.f;
					}
				}
			}

			sigma *= scaleFactor;
		}

		return r/static_cast<float>(scaleCount);
	}
}