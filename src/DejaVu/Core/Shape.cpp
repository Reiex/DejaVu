#include <DejaVu/DejaVu.hpp>

namespace djv
{
	namespace shape
	{
		ThickRect::ThickRect(int64_t left, int64_t top, uint64_t width, uint64_t height, uint64_t thickness) :
			_left(left),
			_top(top),
			_width(width),
			_height(height),
			_thickness(thickness)
		{
		}

		ThickRect::PixelIterator::PixelIterator(int64_t left, int64_t top, uint64_t width, uint64_t height, uint64_t thickness) :
			_cx(left),
			_cy(top),
			_x(left),
			_y(top),

			_a(top + height),
			_b(left + width - 1),
			_c(left + thickness - 1),
			_d(left + width - thickness),
			_e(top + thickness),
			_f(top + height - thickness),

			_w(width),
			_h(height),
			_t(thickness)
		{
			assert(width > 0);
			assert(height > 0);
			assert(thickness > 0 && thickness <= (width + 1) / 2 && thickness <= (height + 1) / 2);
		}

		bool ThickRect::PixelIterator::getPixel(int64_t& x, int64_t& y)
		{
			x = _x;
			y = _y;

			bool r = (_y != _a);

			if (_x == _b)
			{
				_x = 0;
				++_y;
			}
			else if (_x == _c && _y >= _e && _y < _f && _d > _c)
			{
				_x = _d;
			}
			else
			{
				++_x;
			}

			return r;
		}

		std::unique_ptr<Shape::PixelIterator> ThickRect::getPixelIterator() const
		{
			return std::make_unique<ThickRect::PixelIterator>(_left, _top, _width, _height, _thickness);
		}
	
		FilledRect::FilledRect(int64_t left, int64_t top, uint64_t width, uint64_t height) : ThickRect(left, top, width, height, std::min((width + 1) / 2, (height + 1) / 2))
		{
		}

		Rect::Rect(int64_t left, int64_t top, uint64_t width, uint64_t height) : ThickRect(left, top, width, height, 1)
		{
		}
	

		Crown::Crown(int64_t xCenter, int64_t yCenter, uint64_t outerRadius, uint64_t innerRadius) :
			_xCenter(xCenter),
			_yCenter(yCenter),
			_outerRadius(outerRadius),
			_innerRadius(innerRadius)
		{
		}

		Crown::PixelIterator::PixelIterator(int64_t xCenter, int64_t yCenter, uint64_t outerRadius, uint64_t innerRadius) :
			_cx(xCenter),
			_cy(yCenter),
			_orSq(outerRadius*outerRadius),
			_irSq(innerRadius*innerRadius),
			
			_x0(xCenter - outerRadius + 1),
			_y0(yCenter - outerRadius + 1),
			_x(_x0),
			_y(_y0),
			_lx(xCenter + outerRadius - 1),
			_ly(yCenter + outerRadius - 1)
		{
			assert(innerRadius != outerRadius);
		}

		bool Crown::PixelIterator::getPixel(int64_t& x, int64_t& y)
		{
			int64_t dx, dy, dSq;
			if (_x == _x0 && _y == _y0)
			{
				dx = _cx - _x;
				dy = _cy - _y;
				dSq = dx * dx + dy * dy;
				while (dSq >= _orSq)
				{
					++_x;
					dx = _cx - _x;
					dy = _cy - _y;
					dSq = dx * dx + dy * dy;
				}
			}

			x = _x;
			y = _y;

			dx = _cx - _x;
			dy = _cy - _y;
			dSq = dx * dx + dy * dy;
			
			do
			{
				++_x;
				if (_x > _lx)
				{
					_x = _x0;
					++_y;
				}

				if (_y > _ly)
				{
					return false;
				}

				dx = _cx - _x;
				dy = _cy - _y;
				dSq = dx * dx + dy * dy;
			} while (dSq >= _orSq || dSq < _irSq);

			return true;
		}

		std::unique_ptr<Shape::PixelIterator> Crown::getPixelIterator() const
		{
			return std::make_unique<Crown::PixelIterator>(_xCenter, _yCenter, _outerRadius, _innerRadius);
		}

		Disc::Disc(int64_t xCenter, int64_t yCenter, uint64_t radius) : Crown(xCenter, yCenter, radius, 0)
		{
		}

		Circle::Circle(int64_t xCenter, int64_t yCenter, uint64_t radius) : Crown(xCenter, yCenter, radius, radius - 1)
		{
		}
	}
}
