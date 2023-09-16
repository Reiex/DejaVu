///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Pélégrin Marius
//! \copyright The MIT License (MIT)
//! \date 2020-2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <DejaVu/Core/CoreTypes.hpp>

namespace djv
{
	class ShapeThickRect
	{
		public:

			constexpr ShapeThickRect(int64_t left, int64_t top, uint64_t width, uint64_t height, uint64_t thickness);

			class Generator
			{
				public:

					constexpr Generator(int64_t left, int64_t top, uint64_t width, uint64_t height, uint64_t thickness);

					constexpr bool getNextPixel(int64_t& x, int64_t& y);

				private:

					int64_t _cx, _cy, _x, _y;
					int64_t _a, _b, _c, _d, _e, _f;
					uint64_t _w, _h, _t;
			};

			constexpr Generator getGenerator() const;

		private:

			int64_t _left;
			int64_t _top;
			uint64_t _width;
			uint64_t _height;
			uint64_t _thickness;
	};

	class ShapeCrown
	{
		public:

			constexpr ShapeCrown(int64_t xCenter, int64_t yCenter, uint64_t outerRadius, uint64_t innerRadius);

			class Generator
			{
				public:

					constexpr Generator(int64_t xCenter, int64_t yCenter, uint64_t outerRadius, uint64_t innerRadius);

					constexpr bool getNextPixel(int64_t& x, int64_t& y);

				private:

					int64_t _cx, _cy, _irSq, _orSq;
					int64_t _x0, _y0, _x, _y, _lx, _ly;
			};

			constexpr Generator getGenerator() const;

		private:

			int64_t _xCenter;
			int64_t _yCenter;
			uint64_t _innerRadius;
			uint64_t _outerRadius;
	};

	class ShapeLine
	{
		public:

			constexpr ShapeLine(int64_t xStart, int64_t yStart, int64_t xEnd, int64_t yEnd);

			class Generator
			{
				public:

					constexpr Generator(int64_t xStart, int64_t yStart, int64_t xEnd, int64_t yEnd);

					constexpr bool getNextPixel(int64_t& x, int64_t& y);

				private:

					bool _verticalFlip, _absOrdExchange;
					int64_t _x1, _y1, _x2, _y2, _dx, _dy, _e, _e10, _e01, _x, _y;
			};

			constexpr Generator getGenerator() const;

		private:

			int64_t _xStart;
			int64_t _yStart;
			int64_t _xEnd;
			int64_t _yEnd;
	};

	class ShapeFilledRect : public ShapeThickRect
	{
		public:

			constexpr ShapeFilledRect(int64_t left, int64_t top, uint64_t width, uint64_t height);
	};

	class ShapeRect : public ShapeThickRect
	{
		public:

			constexpr ShapeRect(int64_t left, int64_t top, uint64_t width, uint64_t height);
	};

	class ShapeDisc : public ShapeCrown
	{
		public:

			constexpr ShapeDisc(int64_t xCenter, int64_t yCenter, uint64_t radius);
	};

	class ShapeCircle : public ShapeCrown
	{
		public:

			constexpr ShapeCircle(int64_t xCenter, int64_t yCenter, uint64_t radius);
	};
}
