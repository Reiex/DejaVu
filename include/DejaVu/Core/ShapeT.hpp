#pragma once

#include <DejaVu/Core/Shape.hpp>

namespace djv
{
	template<typename PixelType>
	void Shape::draw(Img<PixelType>& dst, const PixelType& color) const
	{
		std::unique_ptr<Shape::PixelIterator> it = getPixelIterator();
		
		uint64_t w = dst.width(), h = dst.height();
		int64_t x, y;
		while (it->getPixel(x, y))
		{
			if (x >= 0 && y >= 0 && x < w && y < h)
			{
				dst[x][y] = color;
			}
		}
	}
}
