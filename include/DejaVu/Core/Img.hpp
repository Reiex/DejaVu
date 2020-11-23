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

			Img() = delete;
			Img(const std::string& filename);
			Img(uint64_t width, uint64_t height, ColorChannels activeChannels = static_cast<ColorChannels>(15));
			Img(const Img& image) = default;
			Img(Img&& image) = default;

			Img& operator=(const Img& image) = default;
			Img& operator=(Img&& image) = default;

			float& operator()(uint64_t x, uint64_t y, ColorComponent component);
			const float& operator()(uint64_t x, uint64_t y, ColorComponent component) const;
			float& operator()(uint64_t x, uint64_t y, uint8_t component);
			const float& operator()(uint64_t x, uint64_t y, uint8_t component) const;

			Img subRect(uint64_t left, uint64_t top, uint64_t width, uint64_t height);

			void setActiveChannels(ColorChannels activeChannels);
			ColorChannels getActiveChannels() const;
			const Mat& getComponent(ColorComponent channel) const;

			~Img() = default;

		private:

			struct RawImg
			{
				uint64_t width;
				uint64_t height;
				uint64_t channelCount;
				uint8_t* data;
			};

			static RawImg loadRaw(const std::string& filename);

			Img(RawImg raw);

			Mat _r;
			Mat _g;
			Mat _b;
			Mat _a;

			ColorChannels _activeChannels;
	};
}
