#include <DejaVu/DejaVu.hpp>

namespace djv
{
	namespace operators
	{
		std::array<scp::Mat<float>, 2> simpleGradient(const scp::Mat<float>& m)
		{
			scp::Mat<float> Gx(m.m, m.n), Gy(m.m, m.n);

			#pragma omp parallel for
			for (int64_t i = 0; i < m.m; i++)
			{
				for (int64_t j = 0; j < m.n; j++)
				{
					int64_t iPreced = std::max(i - 1, static_cast<int64_t>(0));
					int64_t iNext = std::min(i + 1, static_cast<int64_t>(m.m - 1));
					int64_t jPreced = std::max(j - 1, static_cast<int64_t>(0));
					int64_t jNext = std::min(j + 1, static_cast<int64_t>(m.n - 1));

					Gx[i][j] = m[iNext][j] - m[iPreced][j];
					Gy[i][j] = m[i][jNext] - m[i][jPreced];
				}
			}

			return { Gx, Gy };
		}

		std::array<scp::Mat<float>, 2> sobel(const scp::Mat<float>& m)
		{
			scp::Mat<float> Sx(m.m, m.n), Sy(m.m, m.n);

			#pragma omp parallel for
			for (int64_t i = 0; i < m.m; i++)
			{
				for (int64_t j = 0; j < m.n; j++)
				{
					int64_t iPreced = std::max(i - 1, static_cast<int64_t>(0));
					int64_t iNext = std::min(i + 1, static_cast<int64_t>(m.m - 1));
					int64_t jPreced = std::max(j - 1, static_cast<int64_t>(0));
					int64_t jNext = std::min(j + 1, static_cast<int64_t>(m.n - 1));

					Sx[i][j] = (m[iNext][jNext] + 2*m[iNext][j] + m[iNext][jPreced]) - (m[iPreced][jNext] + 2*m[iPreced][j] + m[iPreced][jPreced]);
					Sy[i][j] = (m[iNext][jNext] + 2*m[i][jNext] + m[iPreced][jNext]) - (m[iNext][jPreced] + 2*m[i][jPreced] + m[iPreced][jPreced]);
				}
			}

			return { Sx, Sy };
		}

		std::array<scp::Mat<float>, 2> prewitt(const scp::Mat<float>& m)
		{
			scp::Mat<float> Px(m.m, m.n), Py(m.m, m.n);

			#pragma omp parallel for
			for (int64_t i = 0; i < m.m; i++)
			{
				for (int64_t j = 0; j < m.n; j++)
				{
					int64_t iPreced = std::max(i - 1, static_cast<int64_t>(0));
					int64_t iNext = std::min(i + 1, static_cast<int64_t>(m.m - 1));
					int64_t jPreced = std::max(j - 1, static_cast<int64_t>(0));
					int64_t jNext = std::min(j + 1, static_cast<int64_t>(m.n - 1));

					Px[i][j] = (m[iNext][jNext] + m[iNext][j] + m[iNext][jPreced]) - (m[iPreced][jNext] + m[iPreced][j] + m[iPreced][jPreced]);
					Py[i][j] = (m[iNext][jNext] + m[i][jNext] + m[iPreced][jNext]) - (m[iNext][jPreced] + m[i][jPreced] + m[iPreced][jPreced]);
				}
			}

			return { Px, Py };
		}


		std::array<scp::Mat<float>, 3> simpleHessian(const scp::Mat<float>& m)
		{
			scp::Mat<float> Hxx(m.m, m.n), Hxy(m.m, m.n), Hyy(m.m, m.n);

			#pragma omp parallel for
			for (int64_t i = 0; i < m.m; i++)
			{
				for (int64_t j = 0; j < m.n; j++)
				{
					int64_t iPreced = std::max(i - 1, static_cast<int64_t>(0));
					int64_t iNext = std::min(i + 1, static_cast<int64_t>(m.m - 1));
					int64_t jPreced = std::max(j - 1, static_cast<int64_t>(0));
					int64_t jNext = std::min(j + 1, static_cast<int64_t>(m.n - 1));

					Hxx[i][j] = m[iNext][j] - 2*m[i][j] + m[iPreced][j];
					Hxy[i][j] = m[iNext][jNext] + m[iPreced][jPreced] - m[iNext][jPreced] - m[iPreced][jNext];
					Hyy[i][j] = m[i][jNext] - 2*m[i][j] + m[i][jPreced];
				}
			}

			return { Hxx, Hxy, Hyy };
		}


		scp::Mat<float> simpleLaplacian(const scp::Mat<float>& m)
		{
			scp::Mat<float> r(m.m, m.n);

			#pragma omp parallel for
			for (int64_t i = 0; i < r.m; i++)
			{
				for (int64_t j = 0; j < r.n; j++)
				{
					int64_t iPreced = std::max(i - 1, static_cast<int64_t>(0));
					int64_t iNext = std::min(i + 1, static_cast<int64_t>(m.m - 1));
					int64_t jPreced = std::max(j - 1, static_cast<int64_t>(0));
					int64_t jNext = std::min(j + 1, static_cast<int64_t>(m.n - 1));

					r[i][j] = m[iNext][j] + m[i][jNext] + m[iPreced][j] + m[i][jPreced] - 4*m[i][j];
				}
			}

			return r;
		}


		scp::Mat<float> gaussianBlur(const scp::Mat<float>& m, float sigma)
		{
			scp::Mat<float> rx(m.m, m.n), r(m.m, m.n);
			scp::Vec<float> g(std::min(std::max(2*m.m - 1, 2*m.n - 1), 2*static_cast<uint64_t>(sigma*4) + 1));
			float sigmaSq = sigma * sigma;
			

			#pragma omp parallel
			{
				#pragma omp for
				for (int64_t i(0); i < g.n; i++)
				{
					float x = static_cast<float>(i) - g.n/2;
					g[i] = std::exp(-x*x / (2*sigmaSq))/(2.506628275f*sigma);
				}

				#pragma omp for
				for (int64_t i = 0; i < m.m; i++)
					for (int64_t j = 0; j < m.n; j++)
						for (int64_t k = 0; k < g.n; k++)
							if (i + k >= g.n/2 && i + k - g.n/2 < m.m)
								rx[i][j] += g[k] * m[i + k - g.n/2][j];
							else if (i + k < g.n/2)
								rx[i][j] += g[k] * m[0][j];
							else
								rx[i][j] += g[k] * m[m.m - 1][j];

				#pragma omp for
				for (int64_t i = 0; i < m.m; i++)
					for (int64_t j = 0; j < m.n; j++)
						for (int64_t k = 0; k < g.n; k++)
							if (j + k >= g.n/2 && j + k - g.n/2 < m.n)
								r[i][j] += g[k] * rx[i][j + k - g.n/2];
							else if (j + k < g.n/2)
								r[i][j] += g[k] * rx[i][0];
							else
								r[i][j] += g[k] * rx[i][m.n - 1];
			}

			return r;
		}
	}
}
