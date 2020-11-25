#include <DejaVu/DejaVu.hpp>

namespace djv
{
	// Gradient

	std::array<scp::Mat<float>, 2> sobelGradient(const scp::Mat<float>& m)
	{
		static const scp::Mat<float> Sx{ {
				{-1.f, 0.f, 1.f},
				{-2.f, 0.f, 2.f},
				{-1.f, 0.f, 1.f}}
		};
		static const scp::Mat<float> Sy{ {
				{-1.f, -2.f, -1.f},
				{ 0.f,  0.f,  0.f},
				{ 1.f,  2.f,  1.f}}
		};

		return { scp::convolve(m, Sx), scp::convolve(m, Sy) };
	}

	// Blur

	scp::Mat<float> gaussianBlur(const scp::Mat<float>& m, float sigma, uint64_t patchSize)
	{
		if (patchSize == 0)
			patchSize = 4 * (static_cast<uint64_t>(sigma) + 1);

		scp::Mat<float> G(2 * patchSize + 1, 2 * patchSize + 1);

		for (uint64_t i(0); i < G.m; i++)
		{
			for (uint64_t j(0); j < G.n; j++)
			{
				float x = static_cast<float>(i) - patchSize;
				float y = static_cast<float>(j) - patchSize;
				G[i][j] = std::exp(-(x*x + y*y)/(2*sigma*sigma)) / (6.2831853f*sigma*sigma);
			}
		}

		return scp::convolve(m, G);
	}
}
