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
#include <thread>
#include <vector>

#include <SciPP/SciPP.hpp>

#include <DejaVu/Core/stb/stb_image.hpp>
#include <DejaVu/Core/stb/stb_image_write.hpp>

namespace djv
{
	struct Color;
	struct Point;
	struct Shape;
	class Line;
	class Rect;

	class Img;


	namespace perceptron
	{
		class PerceptronBase;

		class BinaryStep;
		class LeakyReLU;
		class Linear;
		class ReLU;
		class Sigmoid;
		class Tanh;
	}

	namespace layer
	{
		class LayerBase;
		
		template<typename TPerceptron> class Perceptrons;
		class SoftMax;
	}

	class NeuralNetwork;
}