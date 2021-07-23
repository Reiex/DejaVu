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

				class PixelIterator : public Shape::PixelIterator
				{
					public:

						virtual bool getPixel(int64_t& x, int64_t& y);
				};

				virtual std::unique_ptr<Shape::PixelIterator> getPixelIterator() const;

			private:

				int64_t _left;
				int64_t _top;
				uint64_t _width;
				uint64_t _height;
		};

		class FilledRect : public ThickRect
		{

		};

		class Rect : public ThickRect
		{

		};

		class Crown : public Shape
		{

		};

		class Disc : public Crown
		{

		};

		class Circle : public Crown
		{

		};
	}
}

#include <DejaVu/Core/ShapeT.hpp>
