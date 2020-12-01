#include <DejaVu/DejaVu.hpp>

namespace djv
{
	namespace edgeDetector
	{
		scp::Mat<float> sobel(const scp::Mat<float>& m, float threshold)
		{
			scp::Mat<float> Sx = scp::convolve(m, kernel::sobel()[0], scp::ConvolveMethod::Continuous);
			scp::Mat<float> Sy = scp::convolve(m, kernel::sobel()[1], scp::ConvolveMethod::Continuous);
			scp::Mat<float> S = scp::hadamardProduct(Sx, Sx) + scp::hadamardProduct(Sy, Sy);

			float Smax = S[0][0];
			for (uint64_t i(0); i < S.m; i++)
				for (uint64_t j(0); j < S.n; j++)
					if (S[i][j] > Smax)
						Smax = S[i][j];
			
			float thresholdSq = threshold * threshold;
			for (uint64_t i(0); i < S.m; i++)
				for (uint64_t j(0); j < S.n; j++)
					if (S[i][j] > thresholdSq)
						S[i][j] = std::sqrt((S[i][j] - thresholdSq)/(1.f - thresholdSq));
					else
						S[i][j] = 0;
			
			return S;
		}

		scp::Mat<float> prewitt(const scp::Mat<float>& m, float threshold)
		{
			scp::Mat<float> Px = scp::convolve(m, kernel::prewitt()[0], scp::ConvolveMethod::Continuous);
			scp::Mat<float> Py = scp::convolve(m, kernel::prewitt()[1], scp::ConvolveMethod::Continuous);
			scp::Mat<float> P = scp::hadamardProduct(Px, Px) + scp::hadamardProduct(Py, Py);

			float Pmax = P[0][0];
			for (uint64_t i(0); i < P.m; i++)
				for (uint64_t j(0); j < P.n; j++)
					if (P[i][j] > Pmax)
						Pmax = P[i][j];

			float thresholdSq = threshold * threshold;
			for (uint64_t i(0); i < P.m; i++)
				for (uint64_t j(0); j < P.n; j++)
					if (P[i][j] > thresholdSq)
						P[i][j] = std::sqrt((P[i][j] - thresholdSq) / (1.f - thresholdSq));
					else
						P[i][j] = 0;

			return P;
		}

		scp::Mat<float> marrHildreth(const scp::Mat<float>& m, float threshold, float sigma0, float scaleFactor)
		{
			scp::Mat<float> r(m.m, m.n), gPreced(m.m, m.n);
			float sigma = sigma0;
			std::array<scp::Mat<float>, 2> gradKernel = { scp::Mat<float>{ { {-1.f, 0.f, 1.f} } }, scp::Mat<float>{ { {-1.f}, {0.f}, {1.f} } } };
			scp::Mat<float> g = gaussianBlur(m, sigma);
			sigma *= scaleFactor;
			uint64_t scaleCount(1);

			while (4 * sigma < m.m / 2 && 4 * sigma < m.n / 2)
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
						if ((dog[i][j] * dog[i + 1][j] < 0 || dog[i][j] * dog[i][j + 1] < 0) && gradMag[i][j] > threshold)
						{
							r[i][j] += 1.f;
						}
					}
				}

				sigma *= scaleFactor;
			}

			return r / static_cast<float>(scaleCount);
		}

		scp::Mat<float> canny(const scp::Mat<float>& m, float threshold, float sigma0, float scaleFactor)
		{
			/*
				https://en.wikipedia.org/wiki/Canny_edge_detector
			*/
			return m;
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