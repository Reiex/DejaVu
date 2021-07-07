#pragma once

#include <DejaVu/types.hpp>

namespace djv
{
	template<uint64_t N>
	class PixelBase
	{
		public:

			PixelBase(float x = 0.f);
			PixelBase(float r, float g, float b, float a);
			PixelBase(const PixelBase<N>& pixel) = default;
			PixelBase(PixelBase<N>&& pixel) = default;

			PixelBase<N>& operator=(const PixelBase<N>& pixel) = default;
			PixelBase<N>& operator=(PixelBase<N>&& pixel) = default;

			float& operator[](uint64_t i);
			const float& operator[](uint64_t i) const;

			PixelBase<N>& operator+=(const PixelBase<N>& pixel);
			PixelBase<N>& operator-=(const PixelBase<N>& pixel);
			PixelBase<N>& operator*=(const PixelBase<N>& pixel);
			PixelBase<N>& operator/=(const PixelBase<N>& pixel);
			PixelBase<N>& operator%=(const PixelBase<N>& pixel);
			PixelBase<N>& operator+=(float x);
			PixelBase<N>& operator-=(float x);
			PixelBase<N>& operator*=(float x);
			PixelBase<N>& operator/=(float x);
			PixelBase<N>& operator%=(float x);

			static const uint64_t componentCount = N;
			void getRGBA(uint8_t& red, uint8_t& green, uint8_t& blue, uint8_t& alpha) const;

		protected:

			float components[N];
	};

	template<uint64_t N>
	PixelBase<N> operator+(const PixelBase<N>& pixelA, const PixelBase<N>& pixelB);
	template<uint64_t N>
	PixelBase<N>&& operator+(PixelBase<N>&& pixelA, const PixelBase<N>& pixelB);
	template<uint64_t N>
	PixelBase<N>&& operator+(const PixelBase<N>& pixelA, PixelBase<N>&& pixelB);
	template<uint64_t N>
	PixelBase<N>&& operator+(PixelBase<N>&& pixelA, PixelBase<N>&& pixelB);

	template<uint64_t N>
	PixelBase<N> operator-(const PixelBase<N>& pixelA, const PixelBase<N>& pixelB);
	template<uint64_t N>
	PixelBase<N>&& operator-(PixelBase<N>&& pixelA, const PixelBase<N>& pixelB);

	template<uint64_t N>
	PixelBase<N> operator*(const PixelBase<N>& pixelA, const PixelBase<N>& pixelB);
	template<uint64_t N>
	PixelBase<N>&& operator*(PixelBase<N>&& pixelA, const PixelBase<N>& pixelB);
	template<uint64_t N>
	PixelBase<N>&& operator*(const PixelBase<N>& pixelA, PixelBase<N>&& pixelB);
	template<uint64_t N>
	PixelBase<N>&& operator*(PixelBase<N>&& pixelA, PixelBase<N>&& pixelB);

	template<uint64_t N>
	PixelBase<N> operator/(const PixelBase<N>& pixelA, const PixelBase<N>& pixelB);
	template<uint64_t N>
	PixelBase<N>&& operator/(PixelBase<N>&& pixelA, const PixelBase<N>& pixelB);

	template<uint64_t N>
	PixelBase<N> operator%(const PixelBase<N>& pixelA, const PixelBase<N>& pixelB);
	template<uint64_t N>
	PixelBase<N>&& operator%(PixelBase<N>&& pixelA, const PixelBase<N>& pixelB);

	template<uint64_t N>
	PixelBase<N> operator+(const PixelBase<N>& pixel, float x);
	template<uint64_t N>
	PixelBase<N>&& operator+(PixelBase<N>&& pixel, float x);
	template<uint64_t N>
	PixelBase<N> operator+(float x, const PixelBase<N>& pixel);
	template<uint64_t N>
	PixelBase<N>&& operator+(float x, PixelBase<N>&& pixel);

	template<uint64_t N>
	PixelBase<N> operator-(const PixelBase<N>& pixel, float x);
	template<uint64_t N>
	PixelBase<N>&& operator-(PixelBase<N>&& pixel, float x);

	template<uint64_t N>
	PixelBase<N> operator*(const PixelBase<N>& pixel, float x);
	template<uint64_t N>
	PixelBase<N>&& operator*(PixelBase<N>&& pixel, float x);
	template<uint64_t N>
	PixelBase<N> operator*(float x, const PixelBase<N>& pixel);
	template<uint64_t N>
	PixelBase<N>&& operator*(float x, PixelBase<N>&& pixel);

	template<uint64_t N>
	PixelBase<N> operator/(const PixelBase<N>& pixel, float x);
	template<uint64_t N>
	PixelBase<N>&& operator/(PixelBase<N>&& pixel, float x);

	template<uint64_t N>
	PixelBase<N> operator%(const PixelBase<N>& pixel, float x);
	template<uint64_t N>
	PixelBase<N>&& operator%(PixelBase<N>&& pixel, float x);
}

namespace std
{
	template<uint64_t N>
	float norm(const djv::PixelBase<N>& pixel);
	template<uint64_t N>
	float abs(const djv::PixelBase<N>& pixel);
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

			scp::Mat<float> getComponent(uint64_t i) const;
			void setComponent(uint64_t i, const scp::Mat<float>& comp);
			// Img<PixelType> subImage(const Shape& shape, const PixelType& pixelInit) const;

			void normalize(PixelType min = 0.f, PixelType max = 1.f);
			// void scale(float factor);
			// void scale(float xFactor, float yFactor);
			// void scale(uint64_t width, uint64_t height);
			// void rotate(float angle);
			// void flipHorizontally();
			// void flipVertically();
			// void transpose();

			// void draw(const Drawable<PixelType>& drawable);

			uint64_t width() const;
			uint64_t height() const;
			const scp::Mat<PixelType>& getData() const;
			void saveToFile(const std::string& filename) const;

			~Img() = default;

		protected:

			Img();

			std::unique_ptr<scp::Mat<PixelType>> _data;
	};
}

namespace djv
{
	struct GrayScalePixel : public PixelBase<1>
	{
		using PixelBase<1>::PixelBase;

		GrayScalePixel(const PixelBase<1>& pixel);
		GrayScalePixel(PixelBase<1>&& pixel);
		GrayScalePixel(float r, float g, float b, float a);
		GrayScalePixel(const GrayScalePixel& pixel) = default;
		GrayScalePixel(GrayScalePixel&& pixel) = default;

		GrayScalePixel& operator=(const GrayScalePixel& pixel) = default;
		GrayScalePixel& operator=(GrayScalePixel&& pixel) = default;
	};
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
	struct RGBAPixel : public PixelBase<4>
	{
		using PixelBase<4>::PixelBase;

		RGBAPixel(const PixelBase<4>& pixel);
		RGBAPixel(PixelBase<4>&& pixel);
		RGBAPixel(const RGBAPixel& pixel) = default;
		RGBAPixel(RGBAPixel&& pixel) = default;

		RGBAPixel& operator=(const RGBAPixel& pixel) = default;
		RGBAPixel& operator=(RGBAPixel&& pixel) = default;
	};
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
