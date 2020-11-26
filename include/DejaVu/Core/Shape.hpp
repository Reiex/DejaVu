#pragma once

#include <DejaVu/types.hpp>

namespace djv
{
	struct Color
	{
		float r;
		float g;
		float b;
		float a;
	};

	struct Point
	{
		uint64_t x;
		uint64_t y;
	};

	class Shape
	{
		public:

			virtual void draw(Img& target) const = 0;

			Color color;
	};

	class ClosedShape : public Shape
	{
		public:

			bool fill;
			uint64_t borderThickness;
	};

	class OpenShape : public Shape
	{
		public:

			uint64_t thickness;
	};

	class Line : public OpenShape
	{
		public:

			Point p1;
			Point p2;
	};

	class Rect : public ClosedShape
	{
		public:

			Point top_left;
			Point bottom_right;
	};

	class Circle : public ClosedShape
	{
		public:

			Point center;
			uint64_t radius;
	};
}
