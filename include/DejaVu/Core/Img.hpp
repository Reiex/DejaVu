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

		static const uint64_t componentCount = 0;
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

			Img(uint64_t width, uint64_t height, PixelType pixelInit = 0.f);
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

		static const uint64_t componentCount = 1;
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
			GrayScaleImg(const RGBAImg& image, float redWeight = 0.299f, float greenWeight = 0.587f, float blueWeight = 0.114f);
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
	struct RGBAPixel : public PixelBase
	{
		RGBAPixel(float x = 0.f);
		RGBAPixel(float red, float green, float blue, float alpha);
		RGBAPixel(const RGBAPixel& pixel) = default;
		RGBAPixel(RGBAPixel&& pixel) = default;

		RGBAPixel& operator=(const RGBAPixel& pixel) = default;
		RGBAPixel& operator=(RGBAPixel&& pixel) = default;

		RGBAPixel& operator+=(const RGBAPixel& pixel);
		RGBAPixel& operator-=(const RGBAPixel& pixel);
		RGBAPixel& operator*=(float x);
		RGBAPixel& operator/=(float x);

		static const uint64_t componentCount = 4;
		float getComponent(uint64_t i) const;
		void setComponent(uint64_t i, float x);
		void getRGBA(uint8_t & red, uint8_t & green, uint8_t & blue, uint8_t & alpha) const;

		float r;
		float g;
		float b;
		float a;
	};

	RGBAPixel operator+(const RGBAPixel& p, const RGBAPixel& q);
	RGBAPixel&& operator+(RGBAPixel&& p, const RGBAPixel& q);
	RGBAPixel&& operator+(const RGBAPixel& p, RGBAPixel&& q);
	RGBAPixel&& operator+(RGBAPixel&& p, RGBAPixel&& q);

	RGBAPixel operator-(const RGBAPixel& p, const RGBAPixel& q);
	RGBAPixel&& operator-(RGBAPixel&& p, const RGBAPixel& q);

	RGBAPixel operator*(const RGBAPixel& p, float x);
	RGBAPixel&& operator*(RGBAPixel&& p, float x);
	RGBAPixel operator*(float x, const RGBAPixel& p);
	RGBAPixel&& operator*(float x, RGBAPixel&& p);

	RGBAPixel operator/(const RGBAPixel& p, float x);
	RGBAPixel&& operator/(RGBAPixel&& p, float x);
}

namespace std
{
	float norm(const djv::RGBAPixel& pixel);
	float abs(const djv::RGBAPixel& pixel);
}

namespace djv
{
	class RGBAImg : public Img<RGBAPixel>
	{
		public:

			using Img<RGBAPixel>::Img;

			RGBAImg() = delete;
			RGBAImg(const Img<RGBAPixel>& image);
			RGBAImg(Img<RGBAPixel>&& image);
			RGBAImg(const GrayScaleImg& image);
			RGBAImg(const scp::Mat<float>& red, const scp::Mat<float>& green, const scp::Mat<float>& blue, const scp::Mat<float>& alpha);
			RGBAImg(const RGBAImg& image);
			RGBAImg(RGBAImg&& image) = default;

			RGBAImg& operator=(const RGBAImg& image);
			RGBAImg& operator=(RGBAImg&& image) = default;

			~RGBAImg() = default;
	};
}

#include <DejaVu/Core/ImgT.hpp>
