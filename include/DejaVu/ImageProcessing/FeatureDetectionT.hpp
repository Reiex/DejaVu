#pragma once

#include <DejaVu/ImageProcessing/FeatureDetection.hpp>

namespace djv
{
	namespace edgeDetectors
	{
		template<typename PixelType>
		Img<PixelType> marrHildreth(const Img<PixelType>& m)
		{
			Img<PixelType> r(m.width(), m.height());
			std::array<Img<PixelType>, 2> grad = operators::simpleGradient(m);
			Img<PixelType> laplacian = operators::simpleLaplacian(m);

			#pragma omp parallel for
			for (int64_t i = 0; i < laplacian.width() - 1; ++i)
			{
				for (int64_t j = 0; j < laplacian.height() - 1; ++j)
				{
					for (uint64_t k = 0; k < PixelType::componentCount; ++k)
					{
						float l = laplacian[i][j][k], lx = laplacian[i + 1][j][k], ly = laplacian[i + 1][j][k];
						if (l*lx < 0 || l*ly < 0)
						{
							float gx = grad[0][i][j][k], gy = grad[1][i][j][k];
							r[i][j][k] = gx*gx + gy*gy;
						}
					}
				}
			}

			return r;
		}

		template<typename PixelType>
		Img<PixelType> canny(const Img<PixelType>& m, float sigma)
		{
			std::array<Img<PixelType>, 2> grad = operators::simpleGradient(m);
			Img<PixelType> gradMag(m.width(), m.height()), gradArg(m.width(), m.height());

			#pragma omp parallel for
			for (int64_t i = 0; i < gradMag.width(); ++i)
			{
				for (int64_t j = 0; j < gradMag.height(); ++j)
				{
					for (uint64_t k = 0; k < PixelType::componentCount; ++k)
					{
						float gx = grad[0][i][j][k], gy = grad[1][i][j][k];
						gradMag[i][j][k] = gx*gx + gy*gy;
						gradArg[i][j][k] = std::atan2(gy, gx);
					}
				}
			}

			float pi = scp::pi;
			Img<PixelType> r(gradMag);
			#pragma omp parallel for
			for (int64_t i = 0; i < r.width(); ++i)
			{
				for (int64_t j = 0; j < r.height(); ++j)
				{
					for (uint64_t k = 0; k < PixelType::componentCount; ++k)
					{
						int64_t x, y;

						float garg = gradArg[i][j][k];

						if (garg >= 7*pi/8 || garg < -7*pi/8)
						{
							x = -1;
							y = 0;
						}
						else if (garg >= 5*pi/8 && garg < 7*pi/8)
						{
							x = -1;
							y = 1;
						}
						else if (garg >= 3*pi/8 && garg < 5*pi/8)
						{
							x = 0;
							y = 1;
						}
						else if (garg >= pi/8 && garg < 3*pi/8)
						{
							x = 1;
							y = 1;
						}
						else if (garg >= -pi/8 && garg < pi/8)
						{
							x = 1;
							y = 0;
						}
						else if (garg >= -3*pi/8 && garg < -pi/8)
						{
							x = 1;
							y = -1;
						}
						else if (garg >= -5*pi/8 && garg < -3*pi/8)
						{
							x = 0;
							y = -1;
						}
						else
						{
							x = -1;
							y = -1;
						}

						int64_t xForward, xBackward, yForward, yBackward;

						xForward = std::min(std::max(static_cast<int64_t>(i) + x, static_cast<int64_t>(0)), static_cast<int64_t>(r.width() - 1));
						yForward = std::min(std::max(static_cast<int64_t>(j) + y, static_cast<int64_t>(0)), static_cast<int64_t>(r.height() - 1));

						xBackward = std::min(std::max(static_cast<int64_t>(i) - x, static_cast<int64_t>(0)), static_cast<int64_t>(r.width() - 1));
						yBackward = std::min(std::max(static_cast<int64_t>(j) - y, static_cast<int64_t>(0)), static_cast<int64_t>(r.height() - 1));

						float gmag = gradMag[i][j][k], gmagForward = gradMag[xForward][yForward][k], gmagBackward = gradMag[xBackward][yBackward][k];
						if (gmagForward > gmag || gmagBackward > gmag)
						{
							r[i][j][k] = 0.f;
						}
					}
				}
			}

			return r;
		}
	}

	namespace cornerDetectors
	{
		/*scp::Mat<float> harris(const scp::Mat<float>& m, float alpha, const scp::Mat<float>& window)
		{
			std::array<scp::Mat<float>, 2> grad = operators::simpleGradient(m);
			std::array<scp::Mat<float>, 3> gradSq = { scp::Mat<float>(m.shape(0), m.shape(1)), scp::Mat<float>(m.shape(0), m.shape(1)), scp::Mat<float>(m.shape(0), m.shape(1)) };
			scp::Mat<float> R(m.shape(0), m.shape(1)), result(m.shape(0), m.shape(1));
			int64_t ox = window.shape(0) / 2, oy = window.shape(1) / 2;

			#pragma omp parallel
			{
				#pragma omp for
				for (int64_t i = 0; i < m.shape(0); i++)
				{
					for (int64_t j = 0; j < m.shape(1); j++)
					{
						gradSq[0][i][j] = grad[0][i][j] * grad[0][i][j];
						gradSq[1][i][j] = grad[0][i][j] * grad[1][i][j];
						gradSq[2][i][j] = grad[1][i][j] * grad[1][i][j];
					}
				}

				#pragma omp for
				for (int64_t i = 0; i < m.shape(0); i++)
				{
					for (int64_t j = 0; j < m.shape(1); j++)
					{
						scp::Mat<float> M(2, 2);
						float a(0), b(0), d(0);
						for (int64_t p = 0; p < window.shape(0); p++)
						{
							for (int64_t q = 0; q < window.shape(1); q++)
							{
								int64_t x = std::max(std::min(i + p - ox, static_cast<int64_t>(m.shape(0) - 1)), int64_t(0));
								int64_t y = std::max(std::min(j + q - oy, static_cast<int64_t>(m.shape(1) - 1)), int64_t(0));
								a += gradSq[0][x][y] * window[p][q];
								b += gradSq[1][x][y] * window[p][q];
								d += gradSq[2][x][y] * window[p][q];
							}
						}

						R[i][j] = a*d - b*b - alpha*(a + d)*(a + d);
					}
				}

				#pragma omp for
				for (int64_t i = 0; i < m.shape(0); i++)
				{
					for (int64_t j = 0; j < m.shape(1); j++)
					{
						bool localMaximum(R[i][j] > 0);
						if (localMaximum)
						{
							for (int64_t p = -1; p < 2; p++)
							{
								for (int64_t q = -1; q < 2; q++)
								{
									int64_t x = std::max(std::min(i + p, static_cast<int64_t>(m.shape(0) - 1)), int64_t(0));
									int64_t y = std::max(std::min(j + q, static_cast<int64_t>(m.shape(1) - 1)), int64_t(0));
									localMaximum = localMaximum && R[i][j] >= R[x][y];
								}
							}
						}

						if (localMaximum)
							result[i][j] = R[i][j];
					}
				}
			}

			return result;
		}*/
	}

	namespace lineExtractors
	{
		/*namespace
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
			scp::Mat<float> h(static_cast<uint64_t>(scp::pi / dTheta), static_cast<uint64_t>(2 * std::sqrt(m.shape(0)*m.shape(0) + m.shape(1)*m.shape(1))/dRho + 3));

			#pragma omp parallel for
			for (int64_t i = 0; i < m.shape(0); i++)
			{
				for (int64_t j = 0; j < m.shape(1); j++)
				{
					for (int64_t k = 0; k < h.shape(0); k++)
					{
						float theta = dTheta * k;
						int64_t rho = static_cast<int64_t>((std::cos(theta)*i + std::sin(theta)*j) / dRho);
						if (rho != 0)
							h[k][rho + h.shape(1)/2 + 1] += m[i][j];
					}
				}
			}

			h /= h.maxElement();

			std::vector<Line> result;
			#pragma omp parallel for
			for (int64_t i = 0; i < h.shape(0); i++)
			{
				for (int64_t j = 0; j < h.shape(1); j++)
				{
					if (h[i][j] >= threshold)
					{
						Line line = extractLine(m.shape(0), m.shape(1), i*dTheta, (static_cast<float>(j) - h.shape(1)/2 - 1) * dRho);
						#pragma omp critical
						result.push_back(line);
					}
				}
			}

			return result;
		}*/
	}
}