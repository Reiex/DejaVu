#include <DejaVu/DejaVu.hpp>

namespace djv
{
	ColorChannels operator|(ColorChannels a, ColorChannels b)
	{
		return static_cast<ColorChannels>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
	}

	ColorChannels operator&(ColorChannels a, ColorChannels b)
	{
		return static_cast<ColorChannels>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
	}

	Img::Img(const std::string& filename) : Img(loadRaw(filename))
	{

	}

	Img::Img(uint64_t width, uint64_t height, ColorChannels activeChannels)
	{

	}

	float& Img::operator()(uint64_t x, uint64_t y, ColorComponent component)
	{

	}

	const float& Img::operator()(uint64_t x, uint64_t y, ColorComponent component) const
	{

	}

	float& Img::operator()(uint64_t x, uint64_t y, uint8_t component)
	{

	}

	const float& Img::operator()(uint64_t x, uint64_t y, uint8_t component) const
	{

	}

	Img Img::subRect(uint64_t left, uint64_t top, uint64_t width, uint64_t height)
	{

	}

	void Img::setActiveChannels(ColorChannels activeChannels)
	{

	}

	ColorChannels Img::getActiveChannels() const
	{

	}

	const Mat& Img::getComponent(ColorComponent channel) const
	{

	}

	Img::RawImg Img::loadRaw(const std::string& filename)
	{

	}

	Img::Img(RawImg& raw)
	{

	}
}
