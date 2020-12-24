#include <DejaVu/DejaVu.hpp>

namespace djv
{
	Img::Img(const std::string& filename, bool transpose, bool flipHorizontally, bool flipVertically)
	{
		int w, h;
		uint8_t* image = stbi_load(filename.c_str(), &w, &h, nullptr, STBI_rgb_alpha);
		if (image == nullptr)
			throw std::runtime_error("Couldn't open '" + filename + "'.");

		if (transpose)
		{
			_width = h;
			_height = w;
		}
		else
		{
			_width = w;
			_height = h;
		}

		_r = std::make_unique<scp::Mat<float>>(_width, _height);
		_g = std::make_unique<scp::Mat<float>>(_width, _height);
		_b = std::make_unique<scp::Mat<float>>(_width, _height);
		_a = std::make_unique<scp::Mat<float>>(_width, _height);

		for (uint64_t i(0); i < w; i++)
		{
			for (uint64_t j(0); j < h; j++)
			{
				uint64_t x = flipHorizontally && !transpose || flipVertically && transpose ? w - i - 1 : i;
				uint64_t y = flipVertically && !transpose || flipHorizontally && transpose ? h - j - 1 : j;
				uint64_t coord = 4 * (x + w * y);

				if (transpose)
				{
					(*_r)[j][i] = static_cast<float>(image[coord]) / 255.f;
					(*_g)[j][i] = static_cast<float>(image[coord + 1]) / 255.f;
					(*_b)[j][i] = static_cast<float>(image[coord + 2]) / 255.f;
					(*_a)[j][j] = static_cast<float>(image[coord + 3]) / 255.f;
				}
				else
				{
					(*_r)[i][j] = static_cast<float>(image[coord]) / 255.f;
					(*_g)[i][j] = static_cast<float>(image[coord + 1]) / 255.f;
					(*_b)[i][j] = static_cast<float>(image[coord + 2]) / 255.f;
					(*_a)[i][j] = static_cast<float>(image[coord + 3]) / 255.f;
				}
			}
		}

		stbi_image_free(image);
	}

	Img::Img(uint64_t width, uint64_t height) :
		_width(width),
		_height(height),
		_r(std::make_unique<scp::Mat<float>>(width, height)),
		_g(std::make_unique<scp::Mat<float>>(width, height)),
		_b(std::make_unique<scp::Mat<float>>(width, height)),
		_a(std::make_unique<scp::Mat<float>>(width, height, 1.f))
	{
		assert(width != 0 && height != 0);
	}

	Img::Img(const scp::Mat<float>& grayScale) :
		_width(grayScale.m),
		_height(grayScale.n),
		_r(std::make_unique<scp::Mat<float>>(grayScale)),
		_g(std::make_unique<scp::Mat<float>>(grayScale)),
		_b(std::make_unique<scp::Mat<float>>(grayScale)),
		_a(std::make_unique<scp::Mat<float>>(grayScale.m, grayScale.n, 1.f))
	{
	}

	Img::Img(const Img& image) :
		_width(image._width),
		_height(image._height),
		_r(std::make_unique<scp::Mat<float>>(*image._r)),
		_g(std::make_unique<scp::Mat<float>>(*image._g)),
		_b(std::make_unique<scp::Mat<float>>(*image._b)),
		_a(std::make_unique<scp::Mat<float>>(*image._a))
	{
	}


	Img& Img::operator=(const Img& image)
	{
		_width = image._width;
		_height = image._height;
		_r = std::make_unique<scp::Mat<float>>(*image._r);
		_g = std::make_unique<scp::Mat<float>>(*image._g);
		_b = std::make_unique<scp::Mat<float>>(*image._b);
		_a = std::make_unique<scp::Mat<float>>(*image._a);

		return *this;
	}


	float& Img::operator()(uint64_t x, uint64_t y, ColorComponent component)
	{
		assert(x < _width && y < _height);
		return getComponent(component)[x][y];
	}

	const float& Img::operator()(uint64_t x, uint64_t y, ColorComponent component) const
	{
		assert(x < _width && y < _height);
		return getComponent(component)[x][y];
	}

	float& Img::operator()(uint64_t x, uint64_t y, uint8_t component)
	{
		return (*this)(x, y, static_cast<ColorComponent>(component));
	}

	const float& Img::operator()(uint64_t x, uint64_t y, uint8_t component) const
	{
		return (*this)(x, y, static_cast<ColorComponent>(component));
	}

	Color Img::operator()(uint64_t x, uint64_t y) const
	{
		return { (*_r)[x][y], (*_g)[x][y], (*_b)[x][y], (*_a)[x][y] };
	}


	void Img::saveToFile(const std::string& filename) const
	{
		std::vector<uint8_t> image(4 * _width * _height);
		for (uint64_t i(0); i < _width; i++)
		{
			for (uint64_t j(0); j < _height; j++)
			{
				image[4*(i + _width * j)] = 255.f * std::min(std::max((*_r)[i][j], 0.f), 1.f);
				image[4*(i + _width * j) + 1] = 255.f * std::min(std::max((*_g)[i][j], 0.f), 1.f);
				image[4*(i + _width * j) + 2] = 255.f * std::min(std::max((*_b)[i][j], 0.f), 1.f);
				image[4*(i + _width * j) + 3] = 255.f * std::min(std::max((*_a)[i][j], 0.f), 1.f);
			}
		}

		assert(filename.size() > 4);

		if (filename.substr(filename.size() - 4, 4) == ".bmp")
		{
			stbi_write_bmp(filename.c_str(), _width, _height, 4, image.data());
			return;
		}
		else if (filename.substr(filename.size() - 4, 4) == ".jpg")
		{
			stbi_write_jpg(filename.c_str(), _width, _height, 4, image.data(), 95);
			return;
		}
		else if (filename.substr(filename.size() - 4, 4) == ".png")
		{
			stbi_write_png(filename.c_str(), _width, _height, 4, image.data(), 4 * _width);
			return;
		}
		else if (filename.substr(filename.size() - 4, 4) == ".tga")
		{
			stbi_write_tga(filename.c_str(), _width, _height, 4, image.data());
			return;
		}

		assert(filename.size() > 5);

		if (filename.substr(filename.size() - 5, 5) == ".jpeg")
		{
			stbi_write_jpg(filename.c_str(), _width, _height, 4, image.data(), 95);
			return;
		}
		else
			throw std::runtime_error("Image forscp::Mat<float> of '" + filename + "' unrecognized.");
	}

	Img Img::subRect(uint64_t left, uint64_t top, uint64_t width, uint64_t height) const
	{
		Img image(width, height);

		assert(left + width < _width && top + height < _height);

		for (uint64_t i(0); i < width; i++)
		{
			for (uint64_t j(0); j < height; j++)
			{
				(*image._r)[i][j] = (*_r)[left + i][top + j];
				(*image._g)[i][j] = (*_g)[left + i][top + j];
				(*image._b)[i][j] = (*_b)[left + i][top + j];
				(*image._a)[i][j] = (*_a)[left + i][top + j];
			}
		}

		return image;
	}

	void Img::draw(const Shape& shape)
	{
		shape.draw(*this);
	}


	scp::Mat<float> Img::grayScale(float redFactor, float greenFactor, float blueFactor) const
	{
		return redFactor*(*_r) + greenFactor*(*_g) + blueFactor*(*_b);
	}

	scp::Mat<float>& Img::getComponent(ColorComponent component)
	{
		switch (component)
		{
		case ColorComponent::R:
			return *_r;
		case ColorComponent::G:
			return *_g;
		case ColorComponent::B:
			return *_b;
		case ColorComponent::A:
			return *_a;
		default:
			throw std::runtime_error("Cannot get component " + std::to_string(static_cast<int>(component)) + " of Img.");
		}
	}

	const scp::Mat<float>& Img::getComponent(ColorComponent component) const
	{
		switch (component)
		{
			case ColorComponent::R:
				return *_r;
			case ColorComponent::G:
				return *_g;
			case ColorComponent::B:
				return *_b;
			case ColorComponent::A:
				return *_a;
			default:
				throw std::runtime_error("Cannot get component " + std::to_string(static_cast<int>(component)) + " of Img.");
		}
	}

	void Img::setPixel(uint64_t x, uint64_t y, const Color& color)
	{
		(*_r)[x][y] = color.r;
		(*_g)[x][y] = color.g;
		(*_b)[x][y] = color.b;
		(*_a)[x][y] = color.a;
	}

	uint64_t Img::width() const
	{
		return _width;
	}

	uint64_t Img::height() const
	{
		return _height;
	}
}
