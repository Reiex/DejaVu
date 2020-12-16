#include <DejaVu/DejaVu.hpp>

namespace djv
{
	namespace edgeDetectors
	{
		scp::Mat<float> sobel(const scp::Mat<float>& m)
		{
			std::array<scp::Mat<float>, 2> grad = operators::sobel(m);
			scp::Mat<float> S = scp::hadamardProduct(grad[0], grad[0]) + scp::hadamardProduct(grad[1], grad[1]);
			
			return S / scp::maxElement(S);
		}

		scp::Mat<float> prewitt(const scp::Mat<float>& m)
		{
			std::array<scp::Mat<float>, 2> grad = operators::prewitt(m);
			scp::Mat<float> P = scp::hadamardProduct(grad[0], grad[0]) + scp::hadamardProduct(grad[1], grad[1]);

			return P / scp::maxElement(P);
		}

		scp::Mat<float> marrHildreth(const scp::Mat<float>& m, float sigma)
		{
			scp::Mat<float> r(m.m, m.n);
			scp::Mat<float> laplacian = operators::laplacianOfGaussian(m, sigma);
			std::array<scp::Mat<float>, 2> grad = operators::derivativeOfGaussian(m, sigma);

			int64_t i, j;
			#pragma omp parallel for private(i, j) shared(laplacian, grad, r)
			for (i = 0; i < laplacian.m - 1; i++)
				for (j = 0; j < laplacian.n - 1; j++)
					if ((laplacian[i][j] * laplacian[i + 1][j] < 0 || laplacian[i][j] * laplacian[i][j + 1] < 0))
						r[i][j] = grad[0][i][j]*grad[0][i][j] + grad[1][i][j]*grad[1][i][j];

			return r / scp::maxElement(r);
		}

		scp::Mat<float> canny(const scp::Mat<float>& m, float sigma)
		{
			std::array<scp::Mat<float>, 2> grad = operators::derivativeOfGaussian(m, sigma);

			int64_t i, j;
			scp::Mat<float> gradMag(m.m, m.n), gradArg(m.m, m.n);
			#pragma omp parallel for private(i, j) shared(grad, gradMag, gradArg)
			for (i = 0; i < gradMag.m; i++)
			{
				for (j = 0; j < gradMag.n; j++)
				{
					gradMag[i][j] = grad[0][i][j]*grad[0][i][j] + grad[1][i][j]*grad[1][i][j];
					gradArg[i][j] = std::atan2(grad[1][i][j], grad[0][i][j]);
				}
			}

			float pi = scp::pi;
			int64_t x, y, xForward, xBackward, yForward, yBackward;
			scp::Mat<float> r(gradMag);
			#pragma omp parallel for private(i, j, x, y, xForward, xBackward, yForward, yBackward) shared(r, gradMag, gradArg)
			for (i = 0; i < r.m; i++)
			{
				for (j = 0; j < r.n; j++)
				{
					if (gradArg[i][j] >= 7*pi/8 || gradArg[i][j] < -7*pi/8)
					{
						x = -1;
						y = 0;
					}
					else if (gradArg[i][j] >= 5*pi/8 && gradArg[i][j] < 7*pi/8)
					{
						x = -1;
						y = 1;
					}
					else if (gradArg[i][j] >= 3*pi/8 && gradArg[i][j] < 5*pi/8)
					{
						x = 0;
						y = 1;
					}
					else if (gradArg[i][j] >= pi/8 && gradArg[i][j] < 3*pi/8)
					{
						x = 1;
						y = 1;
					}
					else if (gradArg[i][j] >= -pi/8 && gradArg[i][j] < pi/8)
					{
						x = 1;
						y = 0;
					}
					else if (gradArg[i][j] >= -3*pi/8 && gradArg[i][j] < -pi/8)
					{
						x = 1;
						y = -1;
					}
					else if (gradArg[i][j] >= -5*pi/8 && gradArg[i][j] < -3*pi/8)
					{
						x = 0;
						y = -1;
					}
					else
					{
						x = -1;
						y = -1;
					}

					xForward = std::min(std::max(static_cast<int64_t>(i) + x, static_cast<int64_t>(0)), static_cast<int64_t>(r.m - 1));
					yForward = std::min(std::max(static_cast<int64_t>(j) + y, static_cast<int64_t>(0)), static_cast<int64_t>(r.n - 1));

					xBackward = std::min(std::max(static_cast<int64_t>(i) - x, static_cast<int64_t>(0)), static_cast<int64_t>(r.m - 1));
					yBackward = std::min(std::max(static_cast<int64_t>(j) - y, static_cast<int64_t>(0)), static_cast<int64_t>(r.n - 1));

					if (gradMag[xForward][yForward] > gradMag[i][j] || gradMag[xBackward][yBackward] > gradMag[i][j])
						r[i][j] = 0.f;
				}
			}

			return r;
		}
	}

	namespace lineExtractors
	{
		namespace
		{
			Line extractLine(float w, float h, float theta, float rho)
			{
				Line line{};
				line.color = { 0, 0, 0, 1 };

				float cTheta = std::cos(theta);
				float sTheta = std::sin(theta);

				bool p1Found = false;
				if (cTheta != 0)
				{
					float x = rho / cTheta;
					if (x >= 0 && x < w)
					{
						line.p1 = { static_cast<uint64_t>(x), 0 };
						p1Found = true;
					}

					x -= h * sTheta / cTheta;
					if (x >= 0 && x < w)
					{
						if (p1Found)
						{
							line.p2 = { static_cast<uint64_t>(x), static_cast<uint64_t>(h) - 1 };
							return line;
						}
						else
						{
							line.p1 = { static_cast<uint64_t>(x), static_cast<uint64_t>(h) - 1 };
							p1Found = true;
						}
					}
				}

				if (sTheta != 0)
				{
					float y = rho / sTheta;
					if (y >= 0 && y < h)
					{
						if (p1Found)
						{
							line.p2 = { 0, static_cast<uint64_t>(y) };
							return line;
						}
						else
						{
							line.p1 = { 0, static_cast<uint64_t>(y) };
							p1Found = true;
						}
					}

					y -= w * cTheta / sTheta;
					if (y >= 0 && y < h)
					{
						if (p1Found)
						{
							line.p2 = { static_cast<uint64_t>(w) - 1, static_cast<uint64_t>(y) };
							return line;
						}
						else
						{
							line.p1 = { static_cast<uint64_t>(w) - 1, static_cast<uint64_t>(y) };
							p1Found = true;
						}
					}
				}

				throw std::runtime_error("Couldn't extract hough lines");
			}
		}

		std::vector<Line> hough(const scp::Mat<float>& m, float threshold, float dTheta, float dRho)
		{
			scp::Mat<float> h(static_cast<uint64_t>(scp::pi / dTheta), static_cast<uint64_t>(2 * std::sqrt(m.m*m.m + m.n*m.n)/dRho + 3));

			int64_t i, j, k, rho;
			float theta;

			#pragma omp parallel for private(i, j, k, theta, rho) shared(h, m, dRho, dTheta)
			for (i = 0; i < m.m; i++)
			{
				for (j = 0; j < m.n; j++)
				{
					for (k = 0; k < h.m; k++)
					{
						theta = dTheta * k;
						rho = static_cast<int64_t>((std::cos(theta)*i + std::sin(theta)*j) / dRho);
						if (rho != 0)
							h[k][rho + h.n/2 + 1] += m[i][j];
					}
				}
			}

			h /= scp::maxElement(h);

			std::vector<Line> result;
			Line line;
			#pragma omp parallel for private(i, j, line) shared(h, m, threshold, result, dTheta, dRho)
			for (i = 0; i < h.m; i++)
			{
				for (j = 0; j < h.n; j++)
				{
					if (h[i][j] >= threshold)
					{
						line = extractLine(m.m, m.n, i*dTheta, (static_cast<float>(j) - h.n/2 - 1) * dRho);
						#pragma omp critical
						result.push_back(line);
					}
				}
			}

			return result;
		}
	}
}