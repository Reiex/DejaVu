#include <DejaVu/DejaVu.hpp>

namespace djv
{
	namespace kernels
	{
		const std::array<scp::Mat<float>, 2>& sobel()
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

		const std::array<scp::Mat<float>, 2>& prewitt()
		{
			static const std::array<scp::Mat<float>, 2> S{
				scp::Mat<float>{ {
					{-1.f, -1.f, -1.f},
					{ 0.f,  0.f,  0.f},
					{ 1.f,  1.f,  1.f}
				} }, scp::Mat<float>{ {
					{-1.f, 0.f, 1.f},
					{-1.f, 0.f, 1.f},
					{-1.f, 0.f, 1.f}
			} } };

			return S;
		}

		std::array<scp::Mat<float>, 2> derivativeOfGaussian(float sigma, uint64_t patchSize)
		{
			if (patchSize == 0)
				patchSize = 3 * (static_cast<uint64_t>(sigma) + 1);

			std::array<scp::Mat<float>, 2> D{
				scp::Mat<float>(2 * patchSize + 1, 2 * patchSize + 1),
				scp::Mat<float>(2 * patchSize + 1, 2 * patchSize + 1)
			};

			float sigmaSq = sigma * sigma;
			for (uint64_t i(0); i < D[0].m; i++)
			{
				for (uint64_t j(0); j < D[0].n; j++)
				{
					float x = static_cast<float>(i) - patchSize;
					float y = static_cast<float>(j) - patchSize;
					float alpha = -(x * x + y * y) / (2 * sigmaSq);
					D[0][i][j] = -std::exp(alpha) * x / (6.2831853f * sigmaSq * sigmaSq);
					D[1][i][j] = -std::exp(alpha) * y / (6.2831853f * sigmaSq * sigmaSq);
				}
			}

			return D;
		}


		scp::Mat<float> laplacianOfGaussian(float sigma, uint64_t patchSize)
		{
			if (patchSize == 0)
				patchSize = 3 * (static_cast<uint64_t>(sigma) + 1);

			scp::Mat<float> G(2 * patchSize + 1, 2 * patchSize + 1);

			float sigmaSq = sigma * sigma;
			for (uint64_t i(0); i < G.m; i++)
			{
				for (uint64_t j(0); j < G.n; j++)
				{
					float x = static_cast<float>(i) - patchSize;
					float y = static_cast<float>(j) - patchSize;
					float alpha = -(x * x + y * y) / (2 * sigmaSq);
					G[i][j] = -std::exp(alpha) * (1 + alpha) / (3.14159266f * sigmaSq * sigmaSq);
				}
			}

			return G;
		}


		scp::Mat<float> gaussian(float sigma, uint64_t patchSize)
		{
			if (patchSize == 0)
				patchSize = 3 * (static_cast<uint64_t>(sigma) + 1);

			scp::Mat<float> G(2 * patchSize + 1, 2 * patchSize + 1);

			float sigmaSq = sigma * sigma;

			int64_t i, j;
			float x, y;
			for (i = 0; i < G.m; i++)
			{
				for (j = 0; j < G.n; j++)
				{
					x = static_cast<float>(i) - patchSize;
					y = static_cast<float>(j) - patchSize;
					G[i][j] = std::exp(-(x * x + y * y) / (2 * sigmaSq)) / (6.2831853f * sigmaSq);
				}
			}

			return G;
		}
	}

	namespace operators
	{
		std::array<scp::Mat<float>, 2> sobel(const scp::Mat<float>& m)
		{
			scp::Mat<float> Sx(m.m, m.n), Sy(m.m, m.n);

			int64_t iPreced, iNext, jPreced, jNext;
			int64_t i, j;
			#pragma omp parallel for private(i, j, iPreced, iNext, jPreced, jNext) shared(Sx, Sy, m)
			for (i = 0; i < m.m; i++)
			{
				for (j = 0; j < m.n; j++)
				{
					iPreced = std::max(i - 1, static_cast<int64_t>(0));
					iNext = std::min(i + 1, static_cast<int64_t>(m.m - 1));
					jPreced = std::max(j - 1, static_cast<int64_t>(0));
					jNext = std::min(j + 1, static_cast<int64_t>(m.n - 1));

					Sx[i][j] = (m[iNext][jNext] + 2*m[iNext][j] + m[iNext][jPreced]) - (m[iPreced][jNext] + 2*m[iPreced][j] + m[iPreced][jPreced]);
					Sy[i][j] = (m[iNext][jNext] + 2*m[i][jNext] + m[iPreced][jNext]) - (m[iNext][jPreced] + 2*m[i][jPreced] + m[iPreced][jPreced]);
				}
			}

			return { Sx / scp::maxElement(Sx), Sy / scp::maxElement(Sy) };
		}

		std::array<scp::Mat<float>, 2> prewitt(const scp::Mat<float>& m)
		{
			scp::Mat<float> Px(m.m, m.n), Py(m.m, m.n);

			int64_t iPreced, iNext, jPreced, jNext;
			int64_t i, j;
			#pragma omp parallel for private(i, j, iPreced, iNext, jPreced, jNext) shared(Px, Py, m)
			for (i = 0; i < m.m; i++)
			{
				for (j = 0; j < m.n; j++)
				{
					iPreced = std::max(i - 1, static_cast<int64_t>(0));
					iNext = std::min(i + 1, static_cast<int64_t>(m.m - 1));
					jPreced = std::max(j - 1, static_cast<int64_t>(0));
					jNext = std::min(j + 1, static_cast<int64_t>(m.n - 1));

					Px[i][j] = (m[iNext][jNext] + m[iNext][j] + m[iNext][jPreced]) - (m[iPreced][jNext] + m[iPreced][j] + m[iPreced][jPreced]);
					Py[i][j] = (m[iNext][jNext] + m[i][jNext] + m[iPreced][jNext]) - (m[iNext][jPreced] + m[i][jPreced] + m[iPreced][jPreced]);
				}
			}

			return { Px / scp::maxElement(Px), Py / scp::maxElement(Py) };
		}

		std::array<scp::Mat<float>, 2> derivativeOfGaussian(const scp::Mat<float>& m, float sigma)
		{
			scp::Mat<float> g = gaussianBlur(m, sigma);
			scp::Mat<float> gx(m.m, m.n), gy(m.m, m.n);

			int64_t iPreced, iNext, jPreced, jNext;
			int64_t i, j;
			#pragma omp parallel for private(i, j, iPreced, iNext, jPreced, jNext) shared(gx, gy, g)
			for (i = 0; i < g.m; i++)
			{
				for (j = 0; j < g.n; j++)
				{
					iPreced = std::max(i - 1, static_cast<int64_t>(0));
					iNext = std::min(i + 1, static_cast<int64_t>(m.m - 1));
					jPreced = std::max(j - 1, static_cast<int64_t>(0));
					jNext = std::min(j + 1, static_cast<int64_t>(m.n - 1));

					gx[i][j] = g[iNext][j] - g[iPreced][j];
					gy[i][j] = g[i][jNext] - g[i][jPreced];
				}
			}

			return { gx / scp::maxElement(gx), gy / scp::maxElement(gy) };
		}


		scp::Mat<float> laplacianOfGaussian(const scp::Mat<float>& m, float sigma)
		{
			scp::Mat<float> g = gaussianBlur(m, sigma);
			scp::Mat<float> r(m.m, m.n);

			int64_t iPreced, iNext, jPreced, jNext;
			int64_t i, j;
			#pragma omp parallel for private(i, j, iPreced, iNext, jPreced, jNext) shared(r, g)
			for (i = 0; i < r.m; i++)
			{
				for (j = 0; j < r.n; j++)
				{
					iPreced = std::max(i - 1, static_cast<int64_t>(0));
					iNext = std::min(i + 1, static_cast<int64_t>(m.m - 1));
					jPreced = std::max(j - 1, static_cast<int64_t>(0));
					jNext = std::min(j + 1, static_cast<int64_t>(m.n - 1));

					r[i][j] = g[iNext][j] + g[i][jNext] + g[iPreced][j] + g[i][jPreced] - 4*g[i][j];
				}
			}

			return r / std::max(scp::maxElement(r), -scp::minElement(r));
		}


		scp::Mat<float> gaussianBlur(const scp::Mat<float>& m, float sigma)
		{
			scp::Mat<float> rx(m.m, m.n), r(m.m, m.n);
			scp::Vec<float> g(std::min(std::max(2*m.m - 1, 2*m.n - 1), 2*static_cast<uint64_t>(sigma*4) + 1));
			float sigmaSq = sigma * sigma;
			for (uint64_t i(0); i < g.n; i++)
			{
				float x = static_cast<float>(i) - g.n/2;
				g[i] = std::exp(-x*x / (2*sigmaSq))/(2.506628275f*sigma);
			}

			int64_t i, j, k;
			#pragma omp parallel for private(i, j, k) shared(m, g, rx)
			for (i = 0; i < m.m; i++)
				for (j = 0; j < m.n; j++)
					for (k = 0; k < g.n; k++)
						if (i + k >= g.n/2 && i + k - g.n/2 < m.m)
							rx[i][j] += g[k] * m[i + k - g.n/2][j];
						else if (i + k < g.n/2)
							rx[i][j] += g[k] * m[0][j];
						else
							rx[i][j] += g[k] * m[m.m - 1][j];

			#pragma omp parallel for private(i, j, k) shared(m, g, r)
			for (i = 0; i < m.m; i++)
				for (j = 0; j < m.n; j++)
					for (k = 0; k < g.n; k++)
						if (j + k >= g.n/2 && j + k - g.n/2 < m.n)
							r[i][j] += g[k] * rx[i][j + k - g.n/2];
						else if (j + k < g.n/2)
							r[i][j] += g[k] * rx[i][0];
						else
							r[i][j] += g[k] * rx[i][m.n - 1];

			return r;
		}
	}
}
