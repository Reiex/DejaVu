#pragma once

#include <DejaVu/types.hpp>

namespace djv
{
	enum class ColorChannels
	{
		R = 1,
		G = 2,
		B = 4,
		A = 8
	};

	ColorChannels operator|(ColorChannels a, ColorChannels b);
	ColorChannels operator&(ColorChannels a, ColorChannels b);

	enum class ColorComponent
	{
		R = 0,
		G = 1,
		B = 2,
		A = 3
	};

	class Img
	{
		public:

			Img(uint64_t width = 1, uint64_t height = 1);
			Img(const std::string& filename);
			Img(const scp::Mat<float>& grayScale);
			Img(const Img& image);
			Img(Img&& image) = default;

			Img& operator=(const Img& image);
			Img& operator=(Img&& image) = default;

			float& operator()(uint64_t x, uint64_t y, ColorComponent component);
			const float& operator()(uint64_t x, uint64_t y, ColorComponent component) const;
			float& operator()(uint64_t x, uint64_t y, uint8_t component);
			const float& operator()(uint64_t x, uint64_t y, uint8_t component) const;

			void saveToFile(const std::string& filename) const;
			Img subRect(uint64_t left, uint64_t top, uint64_t width, uint64_t height) const;

			scp::Mat<float>& getComponent(ColorComponent component);
			const scp::Mat<float>& getComponent(ColorComponent component) const;
			uint64_t width() const;
			uint64_t height() const;

			~Img() = default;

		private:

			uint64_t _width;
			uint64_t _height;

			std::unique_ptr<scp::Mat<float>> _r;
			std::unique_ptr<scp::Mat<float>> _g;
			std::unique_ptr<scp::Mat<float>> _b;
			std::unique_ptr<scp::Mat<float>> _a;
	};
}
