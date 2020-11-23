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
}
