#pragma once

#include <DejaVu/types.hpp>

namespace djv
{
	struct PixelBase
	{
		// PixelType()
		// PixelType(float x)
		// PixelType(float r, float g, float b, float a)
		// PixelType(const PixelType& pixel)

		// PixelType& operator=(const PixelType& pixel)

		// PixelType& operator+=(const PixelType& pixel)
		// PixelType& operator-=(const PixelType& pixel)
		// PixelType& operator*=(float x)
		// PixelType& operator/=(float x)

		virtual uint64_t componentCount() const = 0;
		virtual float getComponent(uint64_t i) const = 0;
		virtual void setComponent(uint64_t i, float x) = 0;
		virtual void getRGBA(uint8_t& red, uint8_t& green, uint8_t& blue, uint8_t& alpha) const = 0;

		// PixelType operator+(const PixelType& pixelA, const PixelType& pixelB)
		// PixelType operator-(const PixelType& pixelA, const PixelType& pixelB)
		// PixelType operator*(const PixelType& pixel, float x)
		// PixelType operator*(float x, const PixelType& pixel)
		// PixelType operator/(const PixelType& pixelA, float x)

		// float std::norm(const PixelType& pixel)
		// float std::abs(const PixelType& pixel)
	};
}

namespace djv
{
	template<typename PixelType>
	class Img
	{
		public:

			Img(uint64_t width, uint64_t height, PixelType pixelInit = 0);
			Img(const scp::Mat<PixelType>& mat);
			Img(scp::Mat<PixelType>&& mat);
			Img(const std::string& filename, bool transpose = false, bool flipHorizontally = false, bool flipVertically = false);
			Img(const Img<PixelType>& image);
			Img(Img<PixelType>&& image) = default;

			Img<PixelType>& operator=(const Img<PixelType>& image);
			Img<PixelType>& operator=(Img<PixelType>&& image) = default;

			scp::Vec<PixelType>& operator[](uint64_t i);
			const scp::Vec<PixelType>& operator[](uint64_t i) const;

			/*
			void draw(const Drawable<PixelType>& drawable);
			void applySegmentation(const segmentation::Segmentation<PixelType>& seg);
			Img<PixelType> subImage(const Shape& shape, const PixelType& pixelInit) const;
			*/
			void saveToFile(const std::string& filename) const;
			scp::Mat<float> getComponent(uint64_t comp) const;

			uint64_t width() const;
			uint64_t height() const;
			const scp::Mat<PixelType>& getData() const;

			~Img() = default;

		protected:

			Img();

			std::unique_ptr<scp::Mat<PixelType>> _data;
	};
}

namespace djv
{
	struct GrayScalePixel : public PixelBase
	{
		GrayScalePixel(float x = 0.f);
		GrayScalePixel(float r, float g, float b, float a);
		GrayScalePixel(const GrayScalePixel& pixel) = default;
		GrayScalePixel(GrayScalePixel&& pixel) = default;

		GrayScalePixel& operator=(const GrayScalePixel& pixel) = default;
		GrayScalePixel& operator=(GrayScalePixel&& pixel) = default;

		GrayScalePixel& operator+=(const GrayScalePixel& pixel);
		GrayScalePixel& operator-=(const GrayScalePixel& pixel);
		GrayScalePixel& operator*=(float x);
		GrayScalePixel& operator/=(float x);

		uint64_t componentCount() const;
		float getComponent(uint64_t i) const;
		void setComponent(uint64_t i, float x);
		void getRGBA(uint8_t& red, uint8_t& green, uint8_t& blue, uint8_t& alpha) const;

		float value;
	};

	GrayScalePixel operator+(const GrayScalePixel& p, const GrayScalePixel& q);
	GrayScalePixel operator-(const GrayScalePixel& p, const GrayScalePixel& q);
	GrayScalePixel operator*(const GrayScalePixel& p, float x);
	GrayScalePixel operator*(float x, const GrayScalePixel& p);
	GrayScalePixel operator/(const GrayScalePixel& p, float x);
}

namespace std
{
	float norm(const djv::GrayScalePixel& pixel);
	float abs(const djv::GrayScalePixel& pixel);
}

namespace djv
{
	class GrayScaleImg : public Img<GrayScalePixel>
	{
		public:

			using Img<GrayScalePixel>::Img;

			GrayScaleImg() = delete;
			GrayScaleImg(const Img<GrayScalePixel>& image);
			GrayScaleImg(Img<GrayScalePixel>&& image);
			GrayScaleImg(const ColorImg& image, float redWeight = 0.299f, float greenWeight = 0.587f, float blueWeight = 0.114f);
			GrayScaleImg(const scp::Mat<float>& image);
			GrayScaleImg(const GrayScaleImg& image);
			GrayScaleImg(GrayScaleImg&& image) = default;

			GrayScaleImg& operator=(const GrayScaleImg& image);
			GrayScaleImg& operator=(GrayScaleImg&& image) = default;

			~GrayScaleImg() = default;
	};
}

namespace djv
{
	struct ColorPixel : public PixelBase
	{
		ColorPixel(float x = 0.f);
		ColorPixel(float red, float green, float blue, float alpha);
		ColorPixel(const ColorPixel& pixel) = default;
		ColorPixel(ColorPixel&& pixel) = default;

		ColorPixel& operator=(const ColorPixel& pixel) = default;
		ColorPixel& operator=(ColorPixel&& pixel) = default;

		ColorPixel& operator+=(const ColorPixel& pixel);
		ColorPixel& operator-=(const ColorPixel& pixel);
		ColorPixel& operator*=(float x);
		ColorPixel& operator/=(float x);

		uint64_t componentCount() const;
		float getComponent(uint64_t i) const;
		void setComponent(uint64_t i, float x);
		void getRGBA(uint8_t & red, uint8_t & green, uint8_t & blue, uint8_t & alpha) const;

		float r;
		float g;
		float b;
		float a;
	};

	ColorPixel operator+(const ColorPixel& p, const ColorPixel& q);
	ColorPixel&& operator+(ColorPixel&& p, const ColorPixel& q);
	ColorPixel&& operator+(const ColorPixel& p, ColorPixel&& q);
	ColorPixel&& operator+(ColorPixel&& p, ColorPixel&& q);

	ColorPixel operator-(const ColorPixel& p, const ColorPixel& q);
	ColorPixel&& operator-(ColorPixel&& p, const ColorPixel& q);

	ColorPixel operator*(const ColorPixel& p, float x);
	ColorPixel&& operator*(ColorPixel&& p, float x);
	ColorPixel operator*(float x, const ColorPixel& p);
	ColorPixel&& operator*(float x, ColorPixel&& p);

	ColorPixel operator/(const ColorPixel& p, float x);
	ColorPixel&& operator/(ColorPixel&& p, float x);
}

namespace std
{
	float norm(const djv::ColorPixel& pixel);
	float abs(const djv::ColorPixel& pixel);
}

namespace djv
{
	class ColorImg : public Img<ColorPixel>
	{
		public:

			using Img<ColorPixel>::Img;

			ColorImg() = delete;
			ColorImg(const Img<ColorPixel>& image);
			ColorImg(Img<ColorPixel>&& image);
			ColorImg(const GrayScaleImg& image);
			ColorImg(const scp::Mat<float>& red, const scp::Mat<float>& green, const scp::Mat<float>& blue, const scp::Mat<float>& alpha);
			ColorImg(const ColorImg& image);
			ColorImg(ColorImg&& image) = default;

			ColorImg& operator=(const ColorImg& image);
			ColorImg& operator=(ColorImg&& image) = default;

			~ColorImg() = default;
	};
}

#include <DejaVu/Core/ImgT.hpp>
