#include <DejaVu/DejaVu.hpp>

namespace djv
{
	namespace segmentation
	{
		ImageSegmentation::ImageSegmentation(uint64_t w, uint64_t h, uint64_t k) :
			groups(w, h),
			groupColors(k)
		{
		}

		namespace
		{
			float colorDistanceSq(const Color& x, const Color& y)
			{
				return (x.r - y.r)*(x.r - y.r) + (x.g - y.g)*(x.g - y.g) + (x.b - y.b)*(x.b - y.b) + (x.a - y.a)*(x.a - y.a);
			}
		}

		ImageSegmentation kMeans(const Img& m, uint64_t k, float epsilon)
		{
			if (epsilon == 0.f)
				epsilon = k * 1e-5f;

			uint64_t w = m.width(), h = m.height();
			ImageSegmentation seg(w, h, k);

			for (uint64_t i(0); i < k; i++)
				seg.groupColors[i] = m(std::rand() % w, std::rand() % h);

			float colorDiff(2 * epsilon);
			#pragma omp parallel
			while (colorDiff > epsilon)
			{
				#pragma omp for
				for (int64_t i(0); i < w; i++)
				{
					for (int64_t j(0); j < h; j++)
					{
						float minDistance = colorDistanceSq(seg.groupColors[0], m(i, j));
						uint64_t group = 0;
						for (int64_t p(1); p < k; p++)
						{
							float d = colorDistanceSq(seg.groupColors[p], m(i, j));
							if (d < minDistance)
							{
								minDistance = d;
								group = p;
							}
						}

						seg.groups[i][j] = group;
					}
				}

				#pragma omp single
				colorDiff = 0;

				#pragma omp for
				for (int64_t p(0); p < k; p++)
				{
					Color oldColor = seg.groupColors[p];
					seg.groupColors[p] = { 0, 0, 0, 0 };
					uint64_t groupCount(0);

					for (int64_t i(0); i < w; i++)
					{
						for (int64_t j(0); j < h; j++)
						{
							if (seg.groups[i][j] == p)
							{
								seg.groupColors[p].r += m(i, j, 0);
								seg.groupColors[p].g += m(i, j, 1);
								seg.groupColors[p].b += m(i, j, 2);
								seg.groupColors[p].a += m(i, j, 3);
								groupCount++;
							}
						}
					}

					if (groupCount != 0)
					{
						seg.groupColors[p].r /= groupCount;
						seg.groupColors[p].g /= groupCount;
						seg.groupColors[p].b /= groupCount;
						seg.groupColors[p].a /= groupCount;
					}

					float d = colorDistanceSq(oldColor, seg.groupColors[p]);

					#pragma omp atomic
					colorDiff += d;
				}
			}

			return seg;
		}
	}
}