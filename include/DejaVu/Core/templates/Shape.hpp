///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Reiex
//! \copyright The MIT License (MIT)
//! \date 2020-2022
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <DejaVu/Core/CoreDecl.hpp>

namespace djv
{
	constexpr ShapeThickRect::ShapeThickRect(int64_t left, int64_t top, uint64_t width, uint64_t height, uint64_t thickness) :
		_left(left),
		_top(top),
		_width(width),
		_height(height),
		_thickness(thickness)
	{
	}

	constexpr ShapeThickRect::Generator::Generator(int64_t left, int64_t top, uint64_t width, uint64_t height, uint64_t thickness) :
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

	constexpr bool ShapeThickRect::Generator::getNextPixel(int64_t& x, int64_t& y)
	{
		x = _x;
		y = _y;

		bool r = (_y != _a);

		if (_x == _b)
		{
			_x = _cx;
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

	constexpr ShapeThickRect::Generator ShapeThickRect::getGenerator() const
	{
		return ShapeThickRect::Generator(_left, _top, _width, _height, _thickness);
	}


	constexpr ShapeCrown::ShapeCrown(int64_t xCenter, int64_t yCenter, uint64_t outerRadius, uint64_t innerRadius) :
		_xCenter(xCenter),
		_yCenter(yCenter),
		_outerRadius(outerRadius),
		_innerRadius(innerRadius)
	{
	}

	constexpr ShapeCrown::Generator::Generator(int64_t xCenter, int64_t yCenter, uint64_t outerRadius, uint64_t innerRadius) :
		_cx(xCenter),
		_cy(yCenter),
		_orSq(outerRadius* outerRadius),
		_irSq(innerRadius* innerRadius),

		_x0(xCenter - outerRadius + 1),
		_y0(yCenter - outerRadius + 1),
		_x(_x0),
		_y(_y0),
		_lx(xCenter + outerRadius - 1),
		_ly(yCenter + outerRadius - 1)
	{
		assert(innerRadius != outerRadius);
	}

	constexpr bool ShapeCrown::Generator::getNextPixel(int64_t& x, int64_t& y)
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

	constexpr ShapeCrown::Generator ShapeCrown::getGenerator() const
	{
		return ShapeCrown::Generator(_xCenter, _yCenter, _outerRadius, _innerRadius);
	}


	constexpr ShapeLine::ShapeLine(int64_t xStart, int64_t yStart, int64_t xEnd, int64_t yEnd) :
		_xStart(xStart),
		_yStart(yStart),
		_xEnd(xEnd),
		_yEnd(yEnd)
	{
		if (xStart > xEnd)
		{
			std::swap(_xStart, _xEnd);
			std::swap(_yStart, _yEnd);
		}
	}

	constexpr ShapeLine::Generator::Generator(int64_t xStart, int64_t yStart, int64_t xEnd, int64_t yEnd) :
		_verticalFlip(false),
		_absOrdExchange(false),
		_x1(xStart),
		_y1(yStart),
		_x2(xEnd),
		_y2(yEnd)
	{
		if (_x1 > _x2)
		{
			std::swap(_x1, _x2);
			std::swap(_y1, _y2);
		}

		if (_y1 > _y2)
		{
			_verticalFlip = true;
			_y1 = -_y1;
			_y2 = -_y2;
		}

		if (_y2 - _y1 > _x2 - _x1)
		{
			_absOrdExchange = true;
			std::swap(_x1, _y1);
			std::swap(_x2, _y2);
		}

		_dx = _x2 - _x1;
		_dy = _y2 - _y1;

		_e = -_dx;
		_e10 = _dy * 2;
		_e01 = _e * 2;

		_x = _x1;
		_y = _y1;
	}

	constexpr bool ShapeLine::Generator::getNextPixel(int64_t& x, int64_t& y)
	{
		x = _x;
		y = _y;

		if (_absOrdExchange)
		{
			std::swap(x, y);
		}
		if (_verticalFlip)
		{
			y = -y;
		}

		_e += _e10;
		if (_e >= 0)
		{
			_y++;
			_e += _e01;
		}

		if (_x > _x2)
		{
			return false;
		}

		++_x;
		return true;
	}

	constexpr ShapeLine::Generator ShapeLine::getGenerator() const
	{
		return ShapeLine::Generator(_xStart, _yStart, _xEnd, _yEnd);
	}


	constexpr ShapeFilledRect::ShapeFilledRect(int64_t left, int64_t top, uint64_t width, uint64_t height) : ShapeThickRect(left, top, width, height, std::min((width + 1) / 2, (height + 1) / 2))
	{
	}


	constexpr ShapeRect::ShapeRect(int64_t left, int64_t top, uint64_t width, uint64_t height) : ShapeThickRect(left, top, width, height, 1)
	{
	}
	

	constexpr ShapeDisc::ShapeDisc(int64_t xCenter, int64_t yCenter, uint64_t radius) : ShapeCrown(xCenter, yCenter, radius, 0)
	{
	}


	constexpr ShapeCircle::ShapeCircle(int64_t xCenter, int64_t yCenter, uint64_t radius) : ShapeCrown(xCenter, yCenter, radius, radius - 1)
	{
	}
}
