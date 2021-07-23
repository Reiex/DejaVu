#pragma once

#include <DejaVu/Core/types.hpp>

namespace djv
{
	class Shape
	{
		public:

			class PixelIterator
			{
				public:

					virtual bool getPixel(int64_t& x, int64_t& y) = 0;
			};

			virtual std::unique_ptr<Shape::PixelIterator> getPixelIterator() const = 0;

			template<typename PixelType>
			void draw(Img<PixelType>& dst, const PixelType& color) const;

		private:
	};

	namespace shape
	{
		class ThickRect : public Shape
		{
			public:

				ThickRect(int64_t left, int64_t top, uint64_t width, uint64_t height, uint64_t thickness);

				class PixelIterator : public Shape::PixelIterator
				{
					public:

						PixelIterator(int64_t left, int64_t top, uint64_t width, uint64_t height, uint64_t thickness);

						virtual bool getPixel(int64_t& x, int64_t& y);

					private:

						int64_t _cx, _cy, _x, _y;
						int64_t _a, _b, _c, _d, _e, _f;
						uint64_t _w, _h, _t;
				};

				virtual std::unique_ptr<Shape::PixelIterator> getPixelIterator() const;

			private:

				int64_t _left;
				int64_t _top;
				uint64_t _width;
				uint64_t _height;
				uint64_t _thickness;
		};

		class FilledRect : public ThickRect
		{
			public:

				FilledRect(int64_t left, int64_t top, uint64_t width, uint64_t height);
		};

		class Rect : public ThickRect
		{
			public:

				Rect(int64_t left, int64_t top, uint64_t width, uint64_t height);
		};

		class Crown : public Shape
		{
			public:

				Crown(int64_t xCenter, int64_t yCenter, uint64_t outerRadius, uint64_t innerRadius);

				class PixelIterator : public Shape::PixelIterator
				{
					public:

						PixelIterator(int64_t xCenter, int64_t yCenter, uint64_t outerRadius, uint64_t innerRadius);

						virtual bool getPixel(int64_t& x, int64_t& y);

					private:

						int64_t _cx, _cy, _irSq, _orSq;
						int64_t _x0, _y0, _x, _y, _lx, _ly;
				};

				virtual std::unique_ptr<Shape::PixelIterator> getPixelIterator() const;

			private:

				int64_t _xCenter;
				int64_t _yCenter;
				uint64_t _innerRadius;
				uint64_t _outerRadius;
		};

		class Disc : public Crown
		{
			public:

				Disc(int64_t xCenter, int64_t yCenter, uint64_t radius);
		};

		class Circle : public Crown
		{
			public:

				Circle(int64_t xCenter, int64_t yCenter, uint64_t radius);
		};
	}
}

#include <DejaVu/Core/ShapeT.hpp>
