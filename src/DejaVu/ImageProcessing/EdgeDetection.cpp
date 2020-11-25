#include <DejaVu/DejaVu.hpp>

namespace djv
{
	scp::Mat<float> laplacianOfGaussian(scp::Mat<float> m, float sigma, uint64_t patchSize)
	{
		if (patchSize == 0)
			patchSize = 4*(static_cast<uint64_t>(sigma) + 1);

		scp::Mat<float> G(2*patchSize + 1, 2*patchSize + 1);

		for (uint64_t i(0); i < G.m; i++)
		{
			for (uint64_t j(0); j < G.n; j++)
			{
				float x = static_cast<float>(i) - patchSize;
				float y = static_cast<float>(j) - patchSize;
				float sigmaSq = sigma * sigma;
				float alpha = -(x*x + y*y) / (2*sigmaSq);
				G[i][j] = -std::exp(alpha) * (1 + alpha) / (3.14159266f*sigmaSq*sigmaSq);
			}
		}

		return scp::convolve(m, G);
	}
}