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
	struct Color;
	struct Point;
	class Shape;
	class Line;
	class Rect;

	class Img;


	namespace segmentation
	{
		struct ImageSegmentation;
	}
}