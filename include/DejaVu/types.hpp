#pragma once

#include <cassert>
#include <array>
#include <cstdint>
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
	typedef scp::Mat<float> Mat;

	class Img;
}