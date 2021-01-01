#include <DejaVu/DejaVu.hpp>

namespace djv
{
	namespace segmentation
	{
		ImageSegmentation::ImageSegmentation(uint64_t w, uint64_t h, uint64_t k) :
			groups(w, h),
			groupColors(k, { 0.f, 0.f, 0.f, 0.f }),
			groupPos(k, { 0, 0 })
		{
		}


		namespace
		{
			float colorDistanceSq(const Color& x, const Color& y)
			{
				float dr = x.r - y.r;
				float dg = x.g - y.g;
				float db = x.b - y.b;
				float da = x.a - y.a;
				return dr*dr + dg*dg + db*db + da*da;
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
				for (int64_t i = 0; i < w; i++)
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
				for (int64_t p = 0; p < k; p++)
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
		

		namespace
		{
			float pixelDistanceSq(const Point& x, const Color& xc, const Point& y, const Color& yc, uint64_t l, float m)
			{
				float dr = xc.r - yc.r;
				float dg = xc.g - yc.g;
				float db = xc.b - yc.b;
				float da = xc.a - yc.a;
				float dx = (static_cast<float>(x.x) - static_cast<float>(y.x)) / static_cast<float>(l);
				float dy = (static_cast<float>(x.y) - static_cast<float>(y.y)) / static_cast<float>(l);
				return dr*dr + dg*dg + db*db + da*da + m*dx*dx + m*dy*dy;
			}
		}

		ImageSegmentation slic(const Img& im, uint64_t k, float m, float epsilon)
		{
			if (epsilon == 0.f)
				epsilon = k * 1e-4f;

			uint64_t w = im.width(), h = im.height();
			uint64_t l = std::max(w, h);
			ImageSegmentation seg(w, h, k);

			for (uint64_t i(0); i < k; i++)
			{
				seg.groupPos[i] = { std::rand() % w, std::rand() % h };
				seg.groupColors[i] = im(seg.groupPos[i].x, seg.groupPos[i].y);
			}

			float colorDiff(2 * epsilon);
			#pragma omp parallel
			while (colorDiff > epsilon)
			{
				#pragma omp for
				for (int64_t i = 0; i < w; i++)
				{
					for (int64_t j(0); j < h; j++)
					{
						float minDistance = pixelDistanceSq(seg.groupPos[0], seg.groupColors[0], {static_cast<uint64_t>(i), static_cast<uint64_t>(j)}, im(i, j), l, m);
						seg.groups[i][j] = 0;
						for (int64_t p(1); p < k; p++)
						{
							float d = pixelDistanceSq(seg.groupPos[p], seg.groupColors[p], { static_cast<uint64_t>(i), static_cast<uint64_t>(j) }, im(i, j), l, m);
							if (d < minDistance)
							{
								minDistance = d;
								seg.groups[i][j] = p;
							}
						}
					}
				}

				#pragma omp single
				colorDiff = 0;

				#pragma omp for
				for (int64_t p = 0; p < k; p++)
				{
					Color oldColor = seg.groupColors[p];
					Point oldPos = seg.groupPos[p];
					seg.groupColors[p] = { 0.f, 0.f, 0.f, 0.f };
					seg.groupPos[p] = { 0, 0 };
					uint64_t groupCount(0);

					for (int64_t i(0); i < w; i++)
					{
						for (int64_t j(0); j < h; j++)
						{
							if (seg.groups[i][j] == p)
							{
								seg.groupColors[p].r += im(i, j, 0);
								seg.groupColors[p].g += im(i, j, 1);
								seg.groupColors[p].b += im(i, j, 2);
								seg.groupColors[p].a += im(i, j, 3);
								seg.groupPos[p].x += i;
								seg.groupPos[p].y += j;
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
						seg.groupPos[p].x /= groupCount;
						seg.groupPos[p].y /= groupCount;
					}

					float d = pixelDistanceSq(oldPos, oldColor, seg.groupPos[p], seg.groupColors[p], l, 1.f);

					#pragma omp atomic
					colorDiff += d;
				}
			}

			return seg;
		}
	}
}