#pragma once

#include <DejaVu/ImageProcessing/Clustering.hpp>

namespace djv
{
	namespace clustering
	{
		template<typename PixelType>
		Img<PixelType> Clustering::appliedTo(const Img<PixelType>& image) const
		{
			assert(clusterMap.shape(0) == image.width() && clusterMap.shape(1) == image.height());

			// Compute color of each group

			std::vector<PixelType> groupColors(clusterMap.maxElement() + 1);
			#pragma omp parallel for
			for (int64_t p = 0; p < groupColors.size(); ++p)
			{
				uint64_t groupCount = 0;
				for (uint64_t i = 0; i < image.width(); ++i)
				{
					for (uint64_t j = 0; j < image.height(); ++j)
					{
						if (clusterMap[i][j] == p)
						{
							++groupCount;
							groupColors[p] += image[i][j];
						}
					}
				}

				if (groupCount != 0)
				{
					groupColors[p] /= groupCount;
				}
			}

			// Apply color to the clustering

			scp::Mat<PixelType> m(clusterMap.shape(0), clusterMap.shape(1));
			PixelType black;

			#pragma omp parallel for
			for (int64_t i = 0; i < clusterMap.shape(0); ++i)
			{
				for (uint64_t j = 0; j < clusterMap.shape(1); ++j)
				{
					if (clusterMap[i][j] < groupColors.size())
					{
						m[i][j] = groupColors[clusterMap[i][j]];
					}
					else
					{
						m[i][j] = black;
					}
				}
			}

			return Img<PixelType>(m);
		}

		template<typename PixelType>
		Clustering kMeans(const Img<PixelType>& m, uint64_t k, float epsilon)
		{
			assert(k != 0);

			uint64_t w = m.width(), h = m.height();
			Clustering clust(w, h);
			std::vector<PixelType> groupColors(k);

			for (uint64_t i = 0; i < k; ++i)
			{
				groupColors[i] = m[std::rand() % w][std::rand() % h];
			}

			float colorDiff(2 * epsilon);
			#pragma omp parallel
			while (colorDiff > epsilon)
			{
				// Compute to which group each pixel belong

				#pragma omp for
				for (int64_t i = 0; i < w; ++i)
				{
					for (int64_t j = 0; j < h; ++j)
					{
						float minDistance = std::norm(groupColors[0] - m[i][j]);
						uint64_t group = 0;
						for (int64_t p = 1; p < k; ++p)
						{
							float d = std::norm(groupColors[p] - m[i][j]);
							if (d < minDistance)
							{
								minDistance = d;
								group = p;
							}
						}

						clust.clusterMap[i][j] = group;
					}
				}

				// Compute how much the colors of each group changed and update it

				#pragma omp single
				colorDiff = 0;

				#pragma omp for
				for (int64_t p = 0; p < k; ++p)
				{
					PixelType oldColor = groupColors[p];
					groupColors[p] = PixelType();
					uint64_t groupCount(0);

					for (int64_t i = 0; i < w; ++i)
					{
						for (int64_t j = 0; j < h; ++j)
						{
							if (clust.clusterMap[i][j] == p)
							{
								groupColors[p] += m[i][j];
								groupCount++;
							}
						}
					}

					if (groupCount != 0)
					{
						groupColors[p] /= groupCount;
					}

					float d = std::norm(oldColor - groupColors[p]);

					#pragma omp atomic
					colorDiff += d;
				}

				#pragma omp barrier
				{}
			}

			return clust;
		}

		namespace
		{
			template<typename PixelType>
			float slicDist(const std::array<float, 2>& x, const PixelType& xc, const std::array<float, 2>& y, const PixelType& yc, const std::array<float, 2>& l, float m)
			{
				float dx = (x[0] - y[0])/l[0];
				float dy = (x[1] - y[1])/l[1];
				return std::norm(xc - yc) + m*(dx*dx + dy*dy);
			}
		}

		template<typename PixelType>
		Clustering slic(const Img<PixelType>& m, uint64_t k, float posWeight, float epsilon)
		{
			if (epsilon == -1.f)
			{
				epsilon = 1e-4f * k;
			}

			uint64_t w = m.width(), h = m.height();
			std::array<float, 2> l = { w, h };
			Clustering clust(w, h);
			std::vector<PixelType> groupColors(k);
			std::vector<std::array<float, 2>> groupPos(k);

			for (uint64_t i(0); i < k; i++)
			{
				groupPos[i] = { static_cast<float>(std::rand() % w), static_cast<float>(std::rand() % h) };
				groupColors[i] = m[groupPos[i][0]][groupPos[i][1]];
			}

			float colorDiff(2 * epsilon);
			#pragma omp parallel
			while (colorDiff > epsilon)
			{
				#pragma omp for
				for (int64_t i = 0; i < w; ++i)
				{
					for (int64_t j = 0; j < h; ++j)
					{
						float minDistance = slicDist(groupPos[0], groupColors[0], { static_cast<float>(i), static_cast<float>(j) }, m[i][j], l, posWeight);
						clust.clusterMap[i][j] = 0;
						for (int64_t p(1); p < k; p++)
						{
							float d = slicDist(groupPos[p], groupColors[p], { static_cast<float>(i), static_cast<float>(j) }, m[i][j], l, posWeight);
							if (d < minDistance)
							{
								minDistance = d;
								clust.clusterMap[i][j] = p;
							}
						}
					}
				}

				#pragma omp single
				colorDiff = 0;

				#pragma omp for
				for (int64_t p = 0; p < k; ++p)
				{
					PixelType oldColor = groupColors[p];
					std::array<float, 2> oldPos = groupPos[p];
					groupColors[p] = PixelType();
					groupPos[p] = { 0.f, 0.f };
					uint64_t groupCount = 0;

					for (int64_t i = 0; i < w; ++i)
					{
						for (int64_t j = 0; j < h; ++j)
						{
							if (clust.clusterMap[i][j] == p)
							{
								groupColors[p] += m[i][j];
								groupPos[p][0] += i;
								groupPos[p][1] += j;
								groupCount++;
							}
						}
					}

					if (groupCount != 0)
					{
						groupColors[p] /= groupCount;
						groupPos[p][0] /= groupCount;
						groupPos[p][1] /= groupCount;
					}

					float d = slicDist(oldPos, oldColor, groupPos[p], groupColors[p], l, posWeight);

					#pragma omp atomic
					colorDiff += d;
				}

				#pragma omp barrier
				{}
			}

			return clust;
		}
	}
}
