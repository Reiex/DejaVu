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

	struct Shape
	{
		public:

			virtual void draw(Img& target) const = 0;

			Color color;
	};

	class Line : public Shape
	{
		public:

			void draw(Img& target) const;

			Point p1;
			Point p2;
	};

	class Rect : public Shape
	{
		public:

			void draw(Img& target) const;

			Point topLeft;
			Point bottomRight;
			bool fill;
	};
}
