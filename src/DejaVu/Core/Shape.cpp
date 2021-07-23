#include <DejaVu/DejaVu.hpp>

namespace djv
{
	namespace shape
	{
		bool ThickRect::PixelIterator::getPixel(int64_t& x, int64_t& y)
		{
			return false;
		}

		std::unique_ptr<Shape::PixelIterator> ThickRect::getPixelIterator() const
		{
			return std::make_unique<ThickRect::PixelIterator>();
		}
	}
}
