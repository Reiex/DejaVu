#pragma once

#include <DejaVu/types.hpp>

namespace djv
{
	template<typename PixelType>
	class Img
	{
		public:

			Img(uint64_t width, uint64_t height, PixelType pixelInit = 0);
			Img(const scp::Mat<PixelType>& mat);
			Img(scp::Mat<PixelType>&& mat);
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

			uint64_t width() const;
			uint64_t height() const;
			const scp::Mat<PixelType>& getData() const;

			~Img() = default;

		protected:

			Img();

			void saveRawToFile(const std::string& filename, std::vector<uint8_t>& rawData) const;

			std::unique_ptr<scp::Mat<PixelType>> _data;
	};

	class ImgGrayscale : public Img<float>
	{
		public:

			using Img<float>::Img;

			ImgGrayscale() = delete;
			ImgGrayscale(const Img<float>& image);
			ImgGrayscale(Img<float>&& image);
			ImgGrayscale(const std::string& filename, bool transpose = false, bool flipHorizontally = false, bool flipVertically = false, float redWeight = 0.299f, float greenWeight = 0.587f, float blueWeight = 0.114f);
			ImgGrayscale(const ImgRGBA& image, float redWeight = 0.299f, float greenWeight = 0.587f, float blueWeight = 0.114f);
			ImgGrayscale(const ImgGrayscale& image) = default;
			ImgGrayscale(ImgGrayscale&& image) = default;

			void saveToFile(const std::string& filename) const;

			~ImgGrayscale() = default;
	};

	struct PixelRGBA
	{
		PixelRGBA(float x = 0.f);
		PixelRGBA(float red, float green, float blue, float alpha);
		PixelRGBA(const PixelRGBA& p) = default;
		PixelRGBA(PixelRGBA&& p) = default;

		PixelRGBA& operator=(const PixelRGBA& p) = default;
		PixelRGBA& operator=(PixelRGBA&& p) = default;

		float& operator[](uint8_t i);
		const float& operator[](uint8_t i) const;

		enum class Component
		{
			RED,
			GREEN,
			BLUE,
			ALPHA
		};

		float& getComponent(Component component);
		const float& getComponent(Component component) const;

		PixelRGBA& operator+=(const PixelRGBA& p);
		PixelRGBA& operator-=(const PixelRGBA& p);
		PixelRGBA& operator*=(float x);
		PixelRGBA& operator/=(float x);

		float r;
		float g;
		float b;
		float a;
	};

	PixelRGBA operator+(const PixelRGBA& p, const PixelRGBA& q);
	PixelRGBA&& operator+(PixelRGBA&& p, const PixelRGBA& q);
	PixelRGBA&& operator+(const PixelRGBA& p, PixelRGBA&& q);
	PixelRGBA&& operator+(PixelRGBA&& p, PixelRGBA&& q);

	PixelRGBA operator-(const PixelRGBA& p, const PixelRGBA& q);
	PixelRGBA&& operator-(PixelRGBA&& p, const PixelRGBA& q);

	PixelRGBA operator*(const PixelRGBA& p, float x);
	PixelRGBA&& operator*(PixelRGBA&& p, float x);
	PixelRGBA operator*(float x, const PixelRGBA& p);
	PixelRGBA&& operator*(float x, PixelRGBA&& p);

	PixelRGBA operator/(const PixelRGBA& p, float x);
	PixelRGBA&& operator/(PixelRGBA&& p, float x);
}

namespace std
{
	float norm(const djv::PixelRGBA& p);
	float abs(const djv::PixelRGBA& p);
}

namespace djv
{
	class ImgRGBA : public Img<PixelRGBA>
	{
		public:

			using Img<PixelRGBA>::Img;

			ImgRGBA() = delete;
			ImgRGBA(const Img<PixelRGBA>& image);
			ImgRGBA(Img<PixelRGBA>&& image);
			ImgRGBA(const std::string& filename, bool transpose = false, bool flipHorizontally = false, bool flipVertically = false);
			ImgRGBA(const ImgGrayscale& image);
			ImgRGBA(const scp::Mat<float>& red, const scp::Mat<float>& green, const scp::Mat<float>& blue, const scp::Mat<float>& alpha);
			ImgRGBA(const ImgRGBA& image) = default;
			ImgRGBA(ImgRGBA&& image) = default;

			scp::Mat<float> getComponent(PixelRGBA::Component component) const;

			void saveToFile(const std::string& filename) const;

			~ImgRGBA() = default;
	};
}

#include <DejaVu/Core/ImgT.hpp>
