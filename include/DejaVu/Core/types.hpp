#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <SciPP/SciPP.hpp>

#include <DejaVu/Core/stb/stb_image.hpp>
#include <DejaVu/Core/stb/stb_image_write.hpp>

namespace djv
{
	template<uint64_t N> class PixelBase;
	template<typename PixelType> class Img;
	struct GrayScalePixel;
	class GrayScaleImg;
	struct RGBAPixel;
	class RGBAImg;
}