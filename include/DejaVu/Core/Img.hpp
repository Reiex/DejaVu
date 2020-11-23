#pragma once

#include <DejaVu/types.hpp>

namespace djv
{
	enum class ColorChannel
	{
		RED = 1,
		GREEN = 2,
		BLUE = 4,
		ALPHA = 8
	};

	ColorChannel operator|(ColorChannel a, ColorChannel b);
	ColorChannel operator&(ColorChannel a, ColorChannel b);

	struct Pixel
	{
		float& r;
		float& g;
		float& b;
		float& a;
	};

	class Img
	{
		public:

			Img() = delete;
			Img(const std::string& filename);
			Img(uint64_t width, uint64_t height, ColorChannel activeChannels = static_cast<ColorChannel>(15));
			Img(const Img& image) = default;
			Img(Img&& image) = default;

			Img& operator=(const Img& image) = default;
			Img& operator=(Img&& image) = default;

			Pixel& operator()(uint64_t x, uint64_t y);
			const Pixel& operator()(uint64_t x, uint64_t y) const;

			Img subRect(uint64_t left, uint64_t top, uint64_t width, uint64_t height);

			void setActiveChannels(ColorChannel activeChannels);
			ColorChannel getActiveChannels() const;
			const Mat& getComponent(ColorChannel channel) const;

			~Img() = default;

		private:

			Mat _r;
			Mat _g;
			Mat _b;
			Mat _a;

			ColorChannel _activeChannels;
	};
}
