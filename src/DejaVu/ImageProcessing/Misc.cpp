#include <DejaVu/DejaVu.hpp>

namespace djv
{
	namespace operators
	{
		std::array<scp::Mat<float>, 2> simpleGradient(const scp::Mat<float>& m)
		{
			scp::Mat<float> Gx(m.shape(0), m.shape(1)), Gy(m.shape(0), m.shape(1));

			#pragma omp parallel for
			for (int64_t i = 0; i < m.shape(0); i++)
			{
				for (int64_t j = 0; j < m.shape(1); j++)
				{
					int64_t iPreced = std::max(i - 1, static_cast<int64_t>(0));
					int64_t iNext = std::min(i + 1, static_cast<int64_t>(m.shape(0) - 1));
					int64_t jPreced = std::max(j - 1, static_cast<int64_t>(0));
					int64_t jNext = std::min(j + 1, static_cast<int64_t>(m.shape(1) - 1));

					Gx[i][j] = m[iNext][j] - m[iPreced][j];
					Gy[i][j] = m[i][jNext] - m[i][jPreced];
				}
			}

			return { Gx, Gy };
		}

		std::array<scp::Mat<float>, 2> sobel(const scp::Mat<float>& m)
		{
			scp::Mat<float> Sx(m.shape(0), m.shape(1)), Sy(m.shape(0), m.shape(1));

			#pragma omp parallel for
			for (int64_t i = 0; i < m.shape(0); i++)
			{
				for (int64_t j = 0; j < m.shape(1); j++)
				{
					int64_t iPreced = std::max(i - 1, static_cast<int64_t>(0));
					int64_t iNext = std::min(i + 1, static_cast<int64_t>(m.shape(0) - 1));
					int64_t jPreced = std::max(j - 1, static_cast<int64_t>(0));
					int64_t jNext = std::min(j + 1, static_cast<int64_t>(m.shape(1) - 1));

					Sx[i][j] = (m[iNext][jNext] + 2*m[iNext][j] + m[iNext][jPreced]) - (m[iPreced][jNext] + 2*m[iPreced][j] + m[iPreced][jPreced]);
					Sy[i][j] = (m[iNext][jNext] + 2*m[i][jNext] + m[iPreced][jNext]) - (m[iNext][jPreced] + 2*m[i][jPreced] + m[iPreced][jPreced]);
				}
			}

			return { Sx, Sy };
		}

		std::array<scp::Mat<float>, 2> prewitt(const scp::Mat<float>& m)
		{
			scp::Mat<float> Px(m.shape(0), m.shape(1)), Py(m.shape(0), m.shape(1));

			#pragma omp parallel for
			for (int64_t i = 0; i < m.shape(0); i++)
			{
				for (int64_t j = 0; j < m.shape(1); j++)
				{
					int64_t iPreced = std::max(i - 1, static_cast<int64_t>(0));
					int64_t iNext = std::min(i + 1, static_cast<int64_t>(m.shape(0) - 1));
					int64_t jPreced = std::max(j - 1, static_cast<int64_t>(0));
					int64_t jNext = std::min(j + 1, static_cast<int64_t>(m.shape(1) - 1));

					Px[i][j] = (m[iNext][jNext] + m[iNext][j] + m[iNext][jPreced]) - (m[iPreced][jNext] + m[iPreced][j] + m[iPreced][jPreced]);
					Py[i][j] = (m[iNext][jNext] + m[i][jNext] + m[iPreced][jNext]) - (m[iNext][jPreced] + m[i][jPreced] + m[iPreced][jPreced]);
				}
			}

			return { Px, Py };
		}


		std::array<scp::Mat<float>, 3> simpleHessian(const scp::Mat<float>& m)
		{
			scp::Mat<float> Hxx(m.shape(0), m.shape(1)), Hxy(m.shape(0), m.shape(1)), Hyy(m.shape(0), m.shape(1));

			#pragma omp parallel for
			for (int64_t i = 0; i < m.shape(0); i++)
			{
				for (int64_t j = 0; j < m.shape(1); j++)
				{
					int64_t iPreced = std::max(i - 1, static_cast<int64_t>(0));
					int64_t iNext = std::min(i + 1, static_cast<int64_t>(m.shape(0) - 1));
					int64_t jPreced = std::max(j - 1, static_cast<int64_t>(0));
					int64_t jNext = std::min(j + 1, static_cast<int64_t>(m.shape(1) - 1));

					Hxx[i][j] = m[iNext][j] - 2*m[i][j] + m[iPreced][j];
					Hxy[i][j] = m[iNext][jNext] + m[iPreced][jPreced] - m[iNext][jPreced] - m[iPreced][jNext];
					Hyy[i][j] = m[i][jNext] - 2*m[i][j] + m[i][jPreced];
				}
			}

			return { Hxx, Hxy, Hyy };
		}


		scp::Mat<float> simpleLaplacian(const scp::Mat<float>& m)
		{
			scp::Mat<float> r(m.shape(0), m.shape(1));

			#pragma omp parallel for
			for (int64_t i = 0; i < r.shape(0); i++)
			{
				for (int64_t j = 0; j < r.shape(1); j++)
				{
					int64_t iPreced = std::max(i - 1, static_cast<int64_t>(0));
					int64_t iNext = std::min(i + 1, static_cast<int64_t>(m.shape(0) - 1));
					int64_t jPreced = std::max(j - 1, static_cast<int64_t>(0));
					int64_t jNext = std::min(j + 1, static_cast<int64_t>(m.shape(1) - 1));

					r[i][j] = m[iNext][j] + m[i][jNext] + m[iPreced][j] + m[i][jPreced] - 4*m[i][j];
				}
			}

			return r;
		}
	}

	namespace blur
	{
		scp::Mat<float> gaussian(const scp::Mat<float>& m, float sigma)
		{
			scp::Mat<float> rx(m.shape(0), m.shape(1)), r(m.shape(0), m.shape(1));
			scp::Vec<float> g(std::min(std::max(2*m.shape(0) - 1, 2*m.shape(1) - 1), 2*static_cast<uint64_t>(sigma*4) + 1));
			float twoSigmaSq = 2 * sigma * sigma;
			float sigmaSqrtTwoPi = 2.506628275f * sigma;
			

			#pragma omp parallel
			{
				#pragma omp for
				for (int64_t i = 0; i < g.shape(0); i++)
				{
					float x = static_cast<float>(i) - g.shape(0)/2;
					g[i] = std::exp(-x*x / twoSigmaSq)/sigmaSqrtTwoPi;
				}

				#pragma omp for
				for (int64_t i = 0; i < m.shape(0); i++)
					for (int64_t j = 0; j < m.shape(1); j++)
						for (int64_t k = 0; k < g.shape(0); k++)
							if (i + k >= g.shape(0)/2 && i + k - g.shape(0)/2 < m.shape(0))
								rx[i][j] += g[k] * m[i + k - g.shape(0)/2][j];
							else if (i + k < g.shape(0)/2)
								rx[i][j] += g[k] * m[0][j];
							else
								rx[i][j] += g[k] * m[m.shape(0) - 1][j];

				#pragma omp for
				for (int64_t i = 0; i < m.shape(0); i++)
					for (int64_t j = 0; j < m.shape(1); j++)
						for (int64_t k = 0; k < g.shape(0); k++)
							if (j + k >= g.shape(0)/2 && j + k - g.shape(0)/2 < m.shape(1))
								r[i][j] += g[k] * rx[i][j + k - g.shape(0)/2];
							else if (j + k < g.shape(0)/2)
								r[i][j] += g[k] * rx[i][0];
							else
								r[i][j] += g[k] * rx[i][m.shape(1) - 1];
			}

			return r;
		}
	
		scp::Mat<float> gaussianBilateral(const scp::Mat<float>& m, float sigmaSpace, float sigmaIntensity)
		{
			scp::Mat<float> r(m.shape(0), m.shape(1));
			int64_t patchSize = 2 * static_cast<int64_t>(sigmaSpace * 3) + 1;
			float twoSigmaSpaceSq = 2 * sigmaSpace * sigmaSpace;
			float twoSigmaIntensitySq = 2 * sigmaIntensity * sigmaIntensity;

			#pragma omp parallel for
			for (int64_t i = 0; i < m.shape(0); i++)
			{
				for (int64_t j = 0; j < m.shape(1); j++)
				{
					float s = 0.f;

					for (int64_t p = -patchSize; p <= patchSize; p++)
					{
						for (int64_t q = -patchSize; q <= patchSize; q++)
						{
							int64_t x = std::max(std::min(i + p, static_cast<int64_t>(m.shape(0) - 1)), int64_t(0));
							int64_t y = std::max(std::min(j + q, static_cast<int64_t>(m.shape(1) - 1)), int64_t(0));
							float intensityDiff = m[x][y] - m[i][j];
							float coeff = std::exp(-(p * p + q * q) / twoSigmaSpaceSq - intensityDiff * intensityDiff / twoSigmaIntensitySq);
							s += coeff;
							r[i][j] += m[x][y] * coeff;
						}
					}

					r[i][j] /= s;
				}
			}

			return r;
		}

		scp::Mat<float> median(const scp::Mat<float>& m, uint64_t n)
		{
			scp::Mat<float> r(m.shape(0), m.shape(1));
			uint64_t histSize = (2*n + 1)*(2*n + 1);
			uint64_t histMiddle = histSize / 2;

			#pragma omp parallel for
			for (int64_t i = 0; i < m.shape(0); i++)
			{
				for (int64_t j = 0; j < m.shape(1); j++)
				{
					std::vector<float> h;
					for (int64_t p = -static_cast<int64_t>(n); p <= static_cast<int64_t>(n); p++)
					{
						for (int64_t q = -static_cast<int64_t>(n); q <= static_cast<int64_t>(n); q++)
						{
							int64_t x = std::max(std::min(i + p, static_cast<int64_t>(m.shape(0) - 1)), int64_t(0));
							int64_t y = std::max(std::min(j + q, static_cast<int64_t>(m.shape(1) - 1)), int64_t(0));
							h.push_back(m[x][y]);
						}
					}

					std::sort(h.begin(), h.end());
					r[i][j] = h[histMiddle];
				}
			}

			return r;
		}
	
		scp::Mat<float> mean(const scp::Mat<float>& m, uint64_t n)
		{
			scp::Mat<float> rx(m.shape(0), m.shape(1)), r(m.shape(0), m.shape(1));
			int64_t meanCount = (2*n + 1)*(2*n + 1);

			#pragma omp parallel
			{

				#pragma omp for
				for (int64_t i = 0; i < m.shape(0); i++)
					for (int64_t j = 0; j < m.shape(1); j++)
						for (int64_t k = -static_cast<int64_t>(n); k < static_cast<int64_t>(n); k++)
							if (i + k >= 0 && i + k < m.shape(0))
								rx[i][j] += m[i + k][j] / meanCount;
							else if (i + k < 0)
								rx[i][j] += m[0][j] / meanCount;
							else
								rx[i][j] += m[m.shape(0) - 1][j] / meanCount;

				#pragma omp for
				for (int64_t i = 0; i < m.shape(0); i++)
					for (int64_t j = 0; j < m.shape(1); j++)
						for (int64_t k = -static_cast<int64_t>(n); k < static_cast<int64_t>(n); k++)
							if (j + k >= 0 && j + k < m.shape(1))
								r[i][j] += rx[i][j + k];
							else if (j + k < 0)
								r[i][j] += rx[i][0];
							else
								r[i][j] += rx[i][m.shape(1) - 1];
			}

			return r;
		}
	}
}
