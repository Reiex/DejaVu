#include <DejaVu/DejaVu.hpp>

namespace djv
{
	void Line::draw(Img& target) const
	{
		bool directionInversed = false;
		bool verticalFlip = false;
		bool absOrdExchange = false;

		int64_t x1, y1, x2, y2;
		if (p1.x > p2.x)
		{
			directionInversed = true;
			x1 = p2.x;
			y1 = p2.y;
			x2 = p1.x;
			y2 = p1.y;
		}
		else
		{
			x1 = p1.x;
			y1 = p1.y;
			x2 = p2.x;
			y2 = p2.y;
		}

		if (y1 > y2)
		{
			verticalFlip = true;
			y1 = -y1;
			y2 = -y2;
		}

		if (y2 - y1 > x2 - x1)
		{
			absOrdExchange = true;
			int64_t x1Tmp = x1, x2Tmp = x2;
			x1 = y1;
			y1 = x1Tmp;
			x2 = y2;
			y2 = x2Tmp;
		}

		int64_t dx = x2 - x1;
		int64_t dy = y2 - y1;

		int64_t y = y1;
		int64_t e = -dx;
		int64_t e10 = dy * 2;
		int64_t e01 = -dx * 2;

		for (int64_t x = x1; x <= x2; x++)
		{
			int64_t realX = x, realY = y;
			if (absOrdExchange)
			{
				int64_t tmpX = realX;
				realX = realY;
				realY = tmpX;
			}
			if (verticalFlip)
			{
				realY = -realY;
			}

			if (realX >= 0 && realX < target.width() && realY >= 0 && realY < target.height())
			{
				target(realX, realY, ColorComponent::R) = color.r;
				target(realX, realY, ColorComponent::G) = color.g;
				target(realX, realY, ColorComponent::B) = color.b;
				target(realX, realY, ColorComponent::A) = color.a;
			}

			e += e10;
			if (e >= 0)
			{
				y++;
				e += e01;
			}
		}
	}

	void Rect::draw(Img& target) const
	{
		assert(topLeft.x < bottomRight.x && topLeft.y < bottomRight.y);

		Point realBottomRight{ std::min(bottomRight.x, target.width()), std::min(bottomRight.y, target.height()) };

		if (fill)
		{
			for (uint64_t x(topLeft.x); x <= bottomRight.x; x++)
			{
				for (uint64_t y(topLeft.y); y <= bottomRight.y; y++)
				{
					target(x, y, ColorComponent::R) = color.r;
					target(x, y, ColorComponent::G) = color.g;
					target(x, y, ColorComponent::B) = color.b;
					target(x, y, ColorComponent::A) = color.a;
				}
			}
		}
		else
		{
			for (uint64_t x(topLeft.x); x <= bottomRight.x; x++)
			{
				target(x, topLeft.y, ColorComponent::R) = color.r;
				target(x, topLeft.y, ColorComponent::G) = color.g;
				target(x, topLeft.y, ColorComponent::B) = color.b;
				target(x, topLeft.y, ColorComponent::A) = color.a;

				target(x, bottomRight.y, ColorComponent::R) = color.r;
				target(x, bottomRight.y, ColorComponent::G) = color.g;
				target(x, bottomRight.y, ColorComponent::B) = color.b;
				target(x, bottomRight.y, ColorComponent::A) = color.a;
			}

			for (uint64_t y(topLeft.y + 1); y < bottomRight.y; y++)
			{
				target(topLeft.x, y, ColorComponent::R) = color.r;
				target(topLeft.x, y, ColorComponent::G) = color.g;
				target(topLeft.x, y, ColorComponent::B) = color.b;
				target(topLeft.x, y, ColorComponent::A) = color.a;

				target(bottomRight.x, y, ColorComponent::R) = color.r;
				target(bottomRight.x, y, ColorComponent::G) = color.g;
				target(bottomRight.x, y, ColorComponent::B) = color.b;
				target(bottomRight.x, y, ColorComponent::A) = color.a;
			}
		}
	}
}
