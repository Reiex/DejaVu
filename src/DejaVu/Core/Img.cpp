#include <DejaVu/DejaVu.hpp>

namespace djv
{
	ColorChannels operator|(ColorChannels a, ColorChannels b)
	{
		return static_cast<ColorChannels>(static_cast<int>(a) | static_cast<int>(b));
	}

	ColorChannels operator&(ColorChannels a, ColorChannels b)
	{
		return static_cast<ColorChannels>(static_cast<int>(a) & static_cast<int>(b));
	}

	Img::Img(uint64_t width, uint64_t height) :
		_width(width),
		_height(height),
		_r(std::make_unique<scp::Mat<float>>(width, height)),
		_g(std::make_unique<scp::Mat<float>>(width, height)),
		_b(std::make_unique<scp::Mat<float>>(width, height)),
		_a(std::make_unique<scp::Mat<float>>(width, height))
	{
		assert(width != 0 && height != 0);
	}

	Img::Img(const std::string& filename)
	{
		int x, y;
		uint8_t* image = stbi_load(filename.c_str(), &x, &y, nullptr, STBI_rgb_alpha);
		_width = x;
		_height = y;

		_r = std::make_unique<scp::Mat<float>>(_width, _height);
		_g = std::make_unique<scp::Mat<float>>(_width, _height);
		_b = std::make_unique<scp::Mat<float>>(_width, _height);
		_a = std::make_unique<scp::Mat<float>>(_width, _height);

		for (uint64_t i(0); i < _width; i++)
		{
			for (uint64_t j(0); j < _height; j++)
			{
				(*_r)[i][j] = static_cast<float>(image[4*(j + _width * i)])/255.f;
				(*_g)[i][j] = static_cast<float>(image[4*(j + _width * i) + 1])/255.f;
				(*_b)[i][j] = static_cast<float>(image[4*(j + _width * i) + 2])/255.f;
				(*_a)[i][j] = static_cast<float>(image[4*(j + _width * i) + 3])/255.f;
			}
		}

		stbi_image_free(image);
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
		assert(x > 0 && x <= _width && y > 0 && y < _height);
		return getComponent(component)[x][y];
	}

	const float& Img::operator()(uint64_t x, uint64_t y, ColorComponent component) const
	{
		assert(x > 0 && x <= _width && y > 0 && y < _height);
		return getComponent(component)[x][y];
	}

	uint64_t Img::width() const
	{
		return _width;
	}

	uint64_t Img::height() const
	{
		return _height;
	}

	float& Img::operator()(uint64_t x, uint64_t y, uint8_t component)
	{
		return (*this)(x, y, static_cast<ColorComponent>(component));
	}

	const float& Img::operator()(uint64_t x, uint64_t y, uint8_t component) const
	{
		return (*this)(x, y, static_cast<ColorComponent>(component));
	}

	void Img::saveToFile(const std::string& filename) const
	{
		std::vector<uint8_t> image(4 * _width * _height);
		for (uint64_t i(0); i < _width; i++)
		{
			for (uint64_t j(0); j < _height; j++)
			{
				image[4*(j + _width * i)] = 255.f * std::min(std::max((*_r)[i][j], 0.f), 1.f);
				image[4*(j + _width * i) + 1] = 255.f * std::min(std::max((*_g)[i][j], 0.f), 1.f);
				image[4*(j + _width * i) + 2] = 255.f * std::min(std::max((*_b)[i][j], 0.f), 1.f);
				image[4*(j + _width * i) + 3] = 255.f * std::min(std::max((*_a)[i][j], 0.f), 1.f);
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
}
