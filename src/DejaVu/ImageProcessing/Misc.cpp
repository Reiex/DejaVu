#include <DejaVu/DejaVu.hpp>

namespace djv
{
	// Kernels

	// Gradient

	const std::array<scp::Mat<float>, 2>& sobelKernel()
	{
		static const std::array<scp::Mat<float>, 2> S{
			scp::Mat<float>{ {
				{-1.f, -2.f, -1.f},
				{ 0.f,  0.f,  0.f},
				{ 1.f,  2.f,  1.f}
			} }, scp::Mat<float>{ {
				{-1.f, 0.f, 1.f},
				{-2.f, 0.f, 2.f},
				{-1.f, 0.f, 1.f}
		} } };

		return S;
	}

	std::array<scp::Mat<float>, 2> derivativeOfGaussianKernel(float sigma, uint64_t patchSize)
	{
		if (patchSize == 0)
			patchSize = 3 * (static_cast<uint64_t>(sigma) + 1);

		std::array<scp::Mat<float>, 2> D{
			scp::Mat<float>(2*patchSize + 1, 2*patchSize + 1),
			scp::Mat<float>(2*patchSize + 1, 2*patchSize + 1)
		};

		float sigmaSq = sigma * sigma;
		for (uint64_t i(0); i < D[0].m; i++)
		{
			for (uint64_t j(0); j < D[0].n; j++)
			{
				float x = static_cast<float>(i) - patchSize;
				float y = static_cast<float>(j) - patchSize;
				float alpha = -(x*x + y*y) / (2*sigmaSq);
				D[0][i][j] = -std::exp(alpha) * x / (6.2831853f * sigmaSq * sigmaSq);
				D[1][i][j] = -std::exp(alpha) * y / (6.2831853f * sigmaSq * sigmaSq);
			}
		}

		return D;
	}

	// Laplacian

	scp::Mat<float> laplacianOfGaussianKernel(float sigma, uint64_t patchSize) 
	{
		if (patchSize == 0)
			patchSize = 3 * (static_cast<uint64_t>(sigma) + 1);

		scp::Mat<float> G(2*patchSize + 1, 2*patchSize + 1);

		float sigmaSq = sigma * sigma;
		for (uint64_t i(0); i < G.m; i++)
		{
			for (uint64_t j(0); j < G.n; j++)
			{
				float x = static_cast<float>(i) - patchSize;
				float y = static_cast<float>(j) - patchSize;
				float alpha = -(x*x + y*y) / (2*sigmaSq);
				G[i][j] = -std::exp(alpha) * (1 + alpha) / (3.14159266f*sigmaSq*sigmaSq);
			}
		}

		return G;
	}

	// Blur

	scp::Mat<float> gaussianKernel(float sigma, uint64_t patchSize)
	{
		if (patchSize == 0)
			patchSize = 3 * (static_cast<uint64_t>(sigma) + 1);

		scp::Mat<float> G(2*patchSize + 1, 2*patchSize + 1);

		float sigmaSq = sigma * sigma;
		for (uint64_t i(0); i < G.m; i++)
		{
			for (uint64_t j(0); j < G.n; j++)
			{
				float x = static_cast<float>(i) - patchSize;
				float y = static_cast<float>(j) - patchSize;
				G[i][j] = std::exp(-(x*x + y*y)/(2*sigmaSq)) / (6.2831853f*sigmaSq);
			}
		}

		return G;
	}

	
	// Processings

	// Gradient

	// Laplacian

	// Blur

	scp::Mat<float> gaussianBlur(const scp::Mat<float>& m, float sigma, uint64_t patchSize)
	{
		if (patchSize == 0)
			patchSize = 3 * (static_cast<uint64_t>(sigma) + 1);

		scp::Mat<float> r(m.m, m.n);

		scp::Vec<float> g(2*patchSize + 1);
		float sigmaSq = sigma * sigma;
		for (uint64_t i(0); i < g.n; i++)
		{
			float x = static_cast<float>(i) - patchSize;
			g[i] = std::exp(-x*x / (2*sigmaSq))/(2.506628275f*sigma);
		}

		for (uint64_t i(0); i < m.m; i++)
		{
			for (uint64_t j(0); j < m.n; j++)
			{
				for (uint64_t k(0); k < g.n; k++)
				{
					if (i + k >= patchSize && i + k - patchSize < m.m)
						r[i][j] += g[k] * m[i + k - patchSize][j];
					else if (i + k < patchSize)
						r[i][j] += g[k] * m[0][j];
					else
						r[i][j] += g[k] * m[m.m - 1][j];
				}
			}
		}

		for (uint64_t i(0); i < m.m; i++)
		{
			r[i] = scp::convolve(r[i], g, scp::ConvolveMethod::Continuous);
		}

		return r;
	}
}
