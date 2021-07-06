#pragma once

#include <DejaVu/ImageProcessing/Misc.hpp>

namespace djv
{
	namespace operators
	{
		template<typename PixelType>
		std::array<Img<PixelType>, 2> simpleGradient(const Img<PixelType>& m)
		{
			Img<PixelType> Gx(m.width(), m.height()), Gy(m.width(), m.height());

			#pragma omp parallel for
			for (int64_t i = 0; i < m.width(); ++i)
			{
				for (int64_t j = 0; j < m.height(); ++j)
				{
					int64_t iPreced = std::max(i - 1, static_cast<int64_t>(0));
					int64_t iNext = std::min(i + 1, static_cast<int64_t>(m.width() - 1));
					int64_t jPreced = std::max(j - 1, static_cast<int64_t>(0));
					int64_t jNext = std::min(j + 1, static_cast<int64_t>(m.height() - 1));

					Gx[i][j] = m[iNext][j] - m[iPreced][j];
					Gy[i][j] = m[i][jNext] - m[i][jPreced];
				}
			}

			return { Gx, Gy };
		}

		template<typename PixelType>
		std::array<Img<PixelType>, 2> sobel(const Img<PixelType>& m)
		{
			Img<PixelType> Sx(m.width(), m.height()), Sy(m.width(), m.height());

			#pragma omp parallel for
			for (int64_t i = 0; i < m.width(); ++i)
			{
				for (int64_t j = 0; j < m.height(); ++j)
				{
					int64_t iPreced = std::max(i - 1, static_cast<int64_t>(0));
					int64_t iNext = std::min(i + 1, static_cast<int64_t>(m.width() - 1));
					int64_t jPreced = std::max(j - 1, static_cast<int64_t>(0));
					int64_t jNext = std::min(j + 1, static_cast<int64_t>(m.height() - 1));

					Sx[i][j] = (m[iNext][jNext] + 2.f*m[iNext][j] + m[iNext][jPreced]) - (m[iPreced][jNext] + 2.f*m[iPreced][j] + m[iPreced][jPreced]);
					Sy[i][j] = (m[iNext][jNext] + 2.f*m[i][jNext] + m[iPreced][jNext]) - (m[iNext][jPreced] + 2.f*m[i][jPreced] + m[iPreced][jPreced]);
				}
			}

			return { Sx, Sy };
		}

		template<typename PixelType>
		std::array<Img<PixelType>, 2> prewitt(const Img<PixelType>& m)
		{
			Img<PixelType> Px(m.width(), m.height()), Py(m.width(), m.height());

			#pragma omp parallel for
			for (int64_t i = 0; i < m.width(); ++i)
			{
				for (int64_t j = 0; j < m.height(); ++j)
				{
					int64_t iPreced = std::max(i - 1, static_cast<int64_t>(0));
					int64_t iNext = std::min(i + 1, static_cast<int64_t>(m.width() - 1));
					int64_t jPreced = std::max(j - 1, static_cast<int64_t>(0));
					int64_t jNext = std::min(j + 1, static_cast<int64_t>(m.height() - 1));

					Px[i][j] = (m[iNext][jNext] + m[iNext][j] + m[iNext][jPreced]) - (m[iPreced][jNext] + m[iPreced][j] + m[iPreced][jPreced]);
					Py[i][j] = (m[iNext][jNext] + m[i][jNext] + m[iPreced][jNext]) - (m[iNext][jPreced] + m[i][jPreced] + m[iPreced][jPreced]);
				}
			}

			return { Px, Py };
		}


		template<typename PixelType>
		std::array<Img<PixelType>, 3> simpleHessian(const Img<PixelType>& m)
		{
			Img<PixelType> Hxx(m.width(), m.height()), Hxy(m.width(), m.height()), Hyy(m.width(), m.height());

			#pragma omp parallel for
			for (int64_t i = 0; i < m.width(); ++i)
			{
				for (int64_t j = 0; j < m.height(); ++j)
				{
					int64_t iPreced = std::max(i - 1, static_cast<int64_t>(0));
					int64_t iNext = std::min(i + 1, static_cast<int64_t>(m.width() - 1));
					int64_t jPreced = std::max(j - 1, static_cast<int64_t>(0));
					int64_t jNext = std::min(j + 1, static_cast<int64_t>(m.height() - 1));

					Hxx[i][j] = m[iNext][j] - 2.f*m[i][j] + m[iPreced][j];
					Hxy[i][j] = m[iNext][jNext] + m[iPreced][jPreced] - m[iNext][jPreced] - m[iPreced][jNext];
					Hyy[i][j] = m[i][jNext] - 2.f*m[i][j] + m[i][jPreced];
				}
			}

			return { Hxx, Hxy, Hyy };
		}

		
		template<typename PixelType>
		Img<PixelType> simpleLaplacian(const Img<PixelType>& m)
		{
			Img<PixelType> r(m.width(), m.height());

			#pragma omp parallel for
			for (int64_t i = 0; i < r.width(); ++i)
			{
				for (int64_t j = 0; j < r.height(); ++j)
				{
					int64_t iPreced = std::max(i - 1, static_cast<int64_t>(0));
					int64_t iNext = std::min(i + 1, static_cast<int64_t>(m.width() - 1));
					int64_t jPreced = std::max(j - 1, static_cast<int64_t>(0));
					int64_t jNext = std::min(j + 1, static_cast<int64_t>(m.height() - 1));

					r[i][j] = m[iNext][j] + m[i][jNext] + m[iPreced][j] + m[i][jPreced] - 4.f*m[i][j];
				}
			}

			return r;
		}
	}

	namespace blur
	{
		template<typename PixelType>
		Img<PixelType> gaussian(const Img<PixelType>& m, float sigma)
		{
			Img<PixelType> rx(m.width(), m.height()), r(m.width(), m.height());
			scp::Vec<float> g(std::min(std::max(2*m.width() - 1, 2*m.height() - 1), 2*static_cast<uint64_t>(sigma*4) + 1));
			float twoSigmaSq = 2 * sigma * sigma;
			float sigmaSqrtTwoPi = 2.506628275f * sigma;
			

			#pragma omp parallel
			{
				#pragma omp for
				for (int64_t i = 0; i < g.shape(0); ++i)
				{
					float x = static_cast<float>(i) - g.shape(0)/2.f;
					g[i] = std::exp(-x*x / twoSigmaSq)/sigmaSqrtTwoPi;
				}

				#pragma omp for
				for (int64_t i = 0; i < m.width(); ++i)
				{
					for (int64_t j = 0; j < m.height(); ++j)
					{
						for (int64_t k = 0; k < g.shape(0); ++k)
						{
							if (i + k >= g.shape(0)/2 && i + k - g.shape(0)/2 < m.width())
							{
								rx[i][j] += g[k] * m[i + k - g.shape(0)/2][j];
							}
							else if (i + k < g.shape(0)/2)
							{
								rx[i][j] += g[k] * m[0][j];
							}
							else
							{
								rx[i][j] += g[k] * m[m.width() - 1][j];
							}
						}
					}
				}

				#pragma omp for
				for (int64_t i = 0; i < m.width(); ++i)
				{
					for (int64_t j = 0; j < m.height(); ++j)
					{
						for (int64_t k = 0; k < g.shape(0); ++k)
						{
							if (j + k >= g.shape(0)/2 && j + k - g.shape(0)/2 < m.height())
							{
								r[i][j] += g[k] * rx[i][j + k - g.shape(0)/2];
							}
							else if (j + k < g.shape(0)/2)
							{
								r[i][j] += g[k] * rx[i][0];
							}
							else
							{
								r[i][j] += g[k] * rx[i][m.height() - 1];
							}
						}
					}
				}
			}

			return r;
		}

		template<typename PixelType>
		Img<PixelType> gaussianBilateral(const Img<PixelType>& m, float sigmaSpace, float sigmaIntensity)
		{
			Img<PixelType> r(m.width(), m.height());
			int64_t patchSize = 2 * static_cast<int64_t>(sigmaSpace * 3) + 1;
			float twoSigmaSpaceSq = 2 * sigmaSpace * sigmaSpace;
			float twoSigmaIntensitySq = 2 * sigmaIntensity * sigmaIntensity;

			#pragma omp parallel for
			for (int64_t i = 0; i < m.width(); ++i)
			{
				for (int64_t j = 0; j < m.height(); ++j)
				{
					float s = 0.f;

					for (int64_t p = -patchSize; p <= patchSize; ++p)
					{
						for (int64_t q = -patchSize; q <= patchSize; ++q)
						{
							int64_t x = std::max(std::min(i + p, static_cast<int64_t>(m.width() - 1)), int64_t(0));
							int64_t y = std::max(std::min(j + q, static_cast<int64_t>(m.height() - 1)), int64_t(0));
							float coeff = std::exp(-(p*p + q*q)/twoSigmaSpaceSq - std::norm(m[x][y] - m[i][j])/twoSigmaIntensitySq);
							s += coeff;
							r[i][j] += m[x][y] * coeff;
						}
					}

					r[i][j] /= s;
				}
			}

			return r;
		}

		template<typename PixelType>
		Img<PixelType> median(const Img<PixelType>& m, uint64_t n)
		{
			Img<PixelType> r(m.width(), m.height());
			uint64_t histSize = (2*n + 1)*(2*n + 1);
			uint64_t histMiddle = histSize / 2;

			#pragma omp parallel for
			for (int64_t i = 0; i < m.width(); ++i)
			{
				for (int64_t j = 0; j < m.height(); ++j)
				{
					std::vector<std::vector<float>> h(PixelType::componentCount);
					for (int64_t p = -static_cast<int64_t>(n); p <= static_cast<int64_t>(n); ++p)
					{
						for (int64_t q = -static_cast<int64_t>(n); q <= static_cast<int64_t>(n); ++q)
						{
							int64_t x = std::max(std::min(i + p, static_cast<int64_t>(m.width() - 1)), int64_t(0));
							int64_t y = std::max(std::min(j + q, static_cast<int64_t>(m.height() - 1)), int64_t(0));
							for (uint64_t k = 0; k < PixelType::componentCount; ++k)
							{
								h[k].push_back(m[x][y][k]);
							}
						}
					}

					for (uint64_t k = 0; k < PixelType::componentCount; ++k)
					{
						std::sort(h[k].begin(), h[k].end());
						r[i][j][k] = h[k][histMiddle];
					}
				}
			}

			return r;
		}
	
		template<typename PixelType>
		Img<PixelType> mean(const Img<PixelType>& m, uint64_t n)
		{
			Img<PixelType> rx(m.width(), m.height()), r(m.width(), m.height());
			float meanCount = (2*n + 1)*(2*n + 1);

			#pragma omp parallel
			{
				#pragma omp for
				for (int64_t i = 0; i < m.width(); i++)
				{
					for (int64_t j = 0; j < m.height(); j++)
					{
						for (int64_t k = -static_cast<int64_t>(n); k < static_cast<int64_t>(n); k++)
						{
							if (i + k >= 0 && i + k < m.width())
							{
								rx[i][j] += m[i + k][j];
							}
							else if (i + k < 0)
							{
								rx[i][j] += m[0][j];
							}
							else
							{
								rx[i][j] += m[m.width() - 1][j];
							}
						}
					}
				}

				#pragma omp for
				for (int64_t i = 0; i < m.width(); i++)
				{
					for (int64_t j = 0; j < m.height(); j++)
					{
						for (int64_t k = -static_cast<int64_t>(n); k < static_cast<int64_t>(n); k++)
						{
							if (j + k >= 0 && j + k < m.height())
							{
								r[i][j] += rx[i][j + k];
							}
							else if (j + k < 0)
							{
								r[i][j] += rx[i][0];
							}
							else
							{
								r[i][j] += rx[i][m.height() - 1];
							}
						}

						r[i][j] /= meanCount;
					}
				}
			}

			return r;
		}
	}
}
