#include <DejaVu/DejaVu.hpp>

namespace djv
{
	namespace edgeDetector
	{
		scp::Mat<float> sobel(const scp::Mat<float>& m)
		{
			scp::Mat<float> Sx = scp::convolve(m, kernel::sobel()[0], scp::ConvolveMethod::Continuous);
			scp::Mat<float> Sy = scp::convolve(m, kernel::sobel()[1], scp::ConvolveMethod::Continuous);
			scp::Mat<float> S = scp::hadamardProduct(Sx, Sx) + scp::hadamardProduct(Sy, Sy);
			
			return S / scp::maxElement(S);
		}

		scp::Mat<float> prewitt(const scp::Mat<float>& m)
		{
			scp::Mat<float> Px = scp::convolve(m, kernel::prewitt()[0], scp::ConvolveMethod::Continuous);
			scp::Mat<float> Py = scp::convolve(m, kernel::prewitt()[1], scp::ConvolveMethod::Continuous);
			scp::Mat<float> P = scp::hadamardProduct(Px, Px) + scp::hadamardProduct(Py, Py);

			return P / scp::maxElement(P);
		}

		scp::Mat<float> marrHildreth(const scp::Mat<float>& m, float sigma)
		{
			scp::Mat<float> r(m.m, m.n);
			scp::Mat<float> laplacian = scp::convolve(m, kernel::laplacianOfGaussian(sigma), scp::ConvolveMethod::Continuous);
			std::array<scp::Mat<float>, 2> grad = {
				scp::convolve(m, kernel::derivativeOfGaussian(sigma)[0], scp::ConvolveMethod::Continuous),
				scp::convolve(m, kernel::derivativeOfGaussian(sigma)[1], scp::ConvolveMethod::Continuous)
			};
			scp::Mat<float> gradMag = scp::hadamardProduct(grad[0], grad[0]) + scp::hadamardProduct(grad[1], grad[1]);

			gradMag /= scp::maxElement(gradMag);

			for (uint64_t i(0); i < laplacian.m - 1; i++)
				for (uint64_t j(0); j < laplacian.n - 1; j++)
					if ((laplacian[i][j] * laplacian[i + 1][j] < 0 || laplacian[i][j] * laplacian[i][j + 1] < 0))
						r[i][j] = gradMag[i][j];

			return r;
		}

		scp::Mat<float> canny(const scp::Mat<float>& m, float sigma)
		{
			std::array<scp::Mat<float>, 2> grad = {
				scp::convolve(m, kernel::derivativeOfGaussian(sigma)[0], scp::ConvolveMethod::Continuous),
				scp::convolve(m, kernel::derivativeOfGaussian(sigma)[1], scp::ConvolveMethod::Continuous)
			};
			scp::Mat<float> gradMag = scp::hadamardProduct(grad[0], grad[0]) + scp::hadamardProduct(grad[1], grad[1]);
			gradMag /= scp::maxElement(gradMag);

			scp::Mat<float> gradArg(m.m, m.n);
			for (uint64_t i(0); i < gradArg.m; i++)
				for (uint64_t j(0); j < gradArg.n; j++)
					gradArg[i][j] = std::atan2f(grad[1][i][j], grad[0][i][j]);
			
			float pi = scp::pi;
			scp::Mat<float> r = gradMag;
			for (uint64_t i(0); i < r.m; i++)
			{
				for (uint64_t j(0); j < r.n; j++)
				{
					int64_t x(0), y(0);
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
					else if (gradArg[i][j] >= -7*pi/8 && gradArg[i][j] < -5*pi/8)
					{
						x = -1;
						y = -1;
					}

					int64_t xForward = std::min(std::max(static_cast<int64_t>(i) + x, 0LL), static_cast<int64_t>(r.m - 1));
					int64_t yForward = std::min(std::max(static_cast<int64_t>(j) + y, 0LL), static_cast<int64_t>(r.n - 1));

					int64_t xBackward = std::min(std::max(static_cast<int64_t>(i) - x, 0LL), static_cast<int64_t>(r.m - 1));
					int64_t yBackward = std::min(std::max(static_cast<int64_t>(j) - y, 0LL), static_cast<int64_t>(r.n - 1));

					if (gradMag[xForward][yForward] > gradMag[i][j] || gradMag[xBackward][yBackward] > gradMag[i][j])
						r[i][j] = 0.f;
				}
			}

			return r;
		}
	}

	namespace lineExtractor
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

			for (uint64_t i(0); i < m.m; i++)
			{
				for (uint64_t j(0); j < m.n; j++)
				{
					for (uint64_t k(0); k < h.m; k++)
					{
						double theta = static_cast<double>(dTheta) * k;
						int64_t rho = static_cast<int64_t>((static_cast<double>(std::cos(theta))*i + static_cast<double>(std::sin(theta))*j) / dRho);
						if (rho != 0)
							h[k][rho + h.n/2 + 1] += m[i][j];
					}
				}
			}

			float hMax = h[0][0];

			for (uint64_t i(0); i < h.m; i++)
				for (uint64_t j(0); j < h.n; j++)
					if (h[i][j] > hMax)
						hMax = h[i][j];

			h /= hMax;

			std::vector<Line> result;
			for (uint64_t i(0); i < h.m; i++)
			{
				for (uint64_t j(0); j < h.n; j++)
				{
					if (h[i][j] >= threshold)
					{
						float theta = i * dTheta;
						float rho = (static_cast<float>(j) - h.n/2 - 1) * dRho;

						result.push_back(extractLine(m.m, m.n, theta, rho));
					}
				}
			}

			/*

			TODO:
				- Filtre gaussien et recherche de maximum local
				- Découpage segment (histogramme)

			*/

			return result;
		}
	}
}