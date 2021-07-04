#include <DejaVu/DejaVu.hpp>

namespace djv
{
	GrayScalePixel::GrayScalePixel(float x) :
		value(x)
	{
	}

	GrayScalePixel::GrayScalePixel(float r, float g, float b, float a) :
		value(r * 0.299f + g * 0.587f + b * 0.114f)
	{
	}

	GrayScalePixel& GrayScalePixel::operator+=(const GrayScalePixel & pixel)
	{
		value += pixel.value;
		return *this;
	}

	GrayScalePixel& GrayScalePixel::operator-=(const GrayScalePixel & pixel)
	{
		value -= pixel.value;
		return *this;
	}

	GrayScalePixel& GrayScalePixel::operator*=(float x)
	{
		value *= x;
		return *this;
	}

	GrayScalePixel& GrayScalePixel::operator/=(float x)
	{
		value /= x;
		return *this;
	}

	uint64_t GrayScalePixel::componentCount() const
	{
		return 1;
	}

	float GrayScalePixel::getComponent(uint64_t i) const
	{
		assert(i == 0);
		return value;
	}

	void GrayScalePixel::setComponent(uint64_t i, float x)
	{
		assert(i == 0);
		value = x;
	}

	void GrayScalePixel::getRGBA(uint8_t& red, uint8_t& green, uint8_t& blue, uint8_t& alpha) const
	{
		uint8_t x = 255 * std::min(std::max(value, 0.f), 1.f);
		red = x;
		green = x;
		blue = x;
		alpha = 255;
	}

	GrayScalePixel operator+(const GrayScalePixel& p, const GrayScalePixel& q)
	{
		return p.value + q.value;
	}

	GrayScalePixel operator-(const GrayScalePixel& p, const GrayScalePixel& q)
	{
		return p.value - q.value;
	}

	GrayScalePixel operator*(const GrayScalePixel& p, float x)
	{
		return p.value * x;
	}

	GrayScalePixel operator*(float x, const GrayScalePixel& p)
	{
		return p.value * x;
	}

	GrayScalePixel operator/(const GrayScalePixel& p, float x)
	{
		return p.value / x;
	}
}

namespace std
{
	float norm(const djv::GrayScalePixel& pixel)
	{
		return std::norm(pixel.value);
	}

	float abs(const djv::GrayScalePixel& pixel)
	{
		return std::abs(pixel.value);
	}
}

namespace djv
{
	GrayScaleImg::GrayScaleImg(const Img<GrayScalePixel>& image) : Img<GrayScalePixel>(image)
	{
	}

	GrayScaleImg::GrayScaleImg(Img<GrayScalePixel>&& image) : Img<GrayScalePixel>(std::move(image))
	{
	}

	GrayScaleImg::GrayScaleImg(const ColorImg& image, float redWeight, float greenWeight, float blueWeight)
	{
		_data = std::make_unique<scp::Mat<GrayScalePixel>>(image.width(), image.height());

		for (uint64_t i = 0; i < _data->shape(0); ++i)
		{
			for (uint64_t j = 0; j < _data->shape(1); ++j)
			{
				(*_data)[i][j] = image[i][j].r * redWeight + image[i][j].g * greenWeight + image[i][j].b * blueWeight;
			}
		}
	}

	GrayScaleImg::GrayScaleImg(const scp::Mat<float>& image)
	{
		_data = std::make_unique<scp::Mat<GrayScalePixel>>(image.shape(0), image.shape(1));

		for (uint64_t i = 0; i < _data->shape(0); ++i)
		{
			for (uint64_t j = 0; j < _data->shape(1); ++j)
			{
				(*_data)[i][j].value = image[i][j];
			}
		}
	}

	GrayScaleImg::GrayScaleImg(const GrayScaleImg& image) : Img<GrayScalePixel>(image.getData())
	{
	}

	GrayScaleImg& GrayScaleImg::operator=(const GrayScaleImg& image)
	{
		_data = std::make_unique<scp::Mat<GrayScalePixel>>(image.getData());
	}
}

namespace djv
{
	ColorPixel::ColorPixel(float x) :
		r(x),
		g(x),
		b(x),
		a(x)
	{
	}

	ColorPixel::ColorPixel(float red, float green, float blue, float alpha) :
		r(red),
		g(green),
		b(blue),
		a(alpha)
	{
	}

	ColorPixel& ColorPixel::operator+=(const ColorPixel& pixel)
	{
		r += pixel.r;
		g += pixel.g;
		b += pixel.b;
		a += pixel.a;

		return *this;
	}

	ColorPixel& ColorPixel::operator-=(const ColorPixel& pixel)
	{
		r -= pixel.r;
		g -= pixel.g;
		b -= pixel.b;
		a -= pixel.a;

		return *this;
	}

	ColorPixel& ColorPixel::operator*=(float x)
	{
		r *= x;
		g *= x;
		b *= x;
		a *= x;

		return *this;
	}

	ColorPixel& ColorPixel::operator/=(float x)
	{
		r /= x;
		g /= x;
		b /= x;
		a /= x;

		return *this;
	}

	uint64_t ColorPixel::componentCount() const
	{
		return 4;
	}

	float ColorPixel::getComponent(uint64_t i) const
	{
		assert(i < 4);

		switch (i)
		{
		case 0:
			return r;
		case 1:
			return g;
		case 2:
			return b;
		default:
			return a;
		}
	}

	void ColorPixel::setComponent(uint64_t i, float x)
	{
		assert(i < 4);

		switch (i)
		{
		case 0:
			r = x;
			break;
		case 1:
			g = x;
			break;
		case 2:
			b = x;
			break;
		default:
			a = x;
			break;
		}
	}

	void ColorPixel::getRGBA(uint8_t& red, uint8_t& green, uint8_t& blue, uint8_t& alpha) const
	{
		red = 255 * std::min(std::max(r, 1.f), 0.f);
		green = 255 * std::min(std::max(g, 1.f), 0.f);
		blue = 255 * std::min(std::max(b, 1.f), 0.f);
		alpha = 255 * std::min(std::max(a, 1.f), 0.f);
	}

	ColorPixel operator+(const ColorPixel& p, const ColorPixel& q)
	{
		return { p.r + q.r, p.g + q.g, p.b + q.b, p.a + q.a };
	}

	ColorPixel&& operator+(ColorPixel&& p, const ColorPixel& q)
	{
		p += q;
		return std::move(p);
	}

	ColorPixel&& operator+(const ColorPixel& p, ColorPixel&& q)
	{
		q += p;
		return std::move(q);
	}

	ColorPixel&& operator+(ColorPixel&& p, ColorPixel&& q)
	{
		p += q;
		return std::move(p);
	}

	ColorPixel operator-(const ColorPixel& p, const ColorPixel& q)
	{
		return { p.r - q.r, p.g - q.g, p.b - q.b, p.a - q.a };
	}

	ColorPixel&& operator-(ColorPixel&& p, const ColorPixel& q)
	{
		p -= q;
		return std::move(p);
	}

	ColorPixel operator*(const ColorPixel& p, float x)
	{
		return { p.r * x, p.g * x, p.b * x, p.a * x };
	}

	ColorPixel&& operator*(ColorPixel&& p, float x)
	{
		p *= x;
		return std::move(p);
	}

	ColorPixel operator*(float x, const ColorPixel& p)
	{
		return { p.r * x, p.g * x, p.b * x, p.a * x };
	}

	ColorPixel&& operator*(float x, ColorPixel&& p)
	{
		p *= x;
		return std::move(p);
	}

	ColorPixel operator/(const ColorPixel& p, float x)
	{
		return { p.r / x, p.g / x, p.b / x, p.a / x };
	}

	ColorPixel&& operator/(ColorPixel&& p, float x)
	{
		p /= x;
		return std::move(p);
	}
}

namespace std
{
	float norm(const djv::ColorPixel& pixel)
	{
		return pixel.r * pixel.r + pixel.g * pixel.g + pixel.b * pixel.b + pixel.a * pixel.a;
	}

	float abs(const djv::ColorPixel& pixel)
	{
		return sqrt(norm(pixel));
	}
}

namespace djv
{
	ColorImg::ColorImg(const Img<ColorPixel>& image) : Img<ColorPixel>(image)
	{
	}

	ColorImg::ColorImg(Img<ColorPixel>&& image) : Img<ColorPixel>(std::move(image))
	{
	}

	ColorImg::ColorImg(const GrayScaleImg& image)
	{
		_data = std::make_unique<scp::Mat<ColorPixel>>(image.width(), image.height());

		for (uint64_t i = 0; i < _data->shape(0); ++i)
		{
			for (uint64_t j = 0; j < _data->shape(1); ++j)
			{
				(*_data)[i][j].r = image[i][j].value;
				(*_data)[i][j].g = image[i][j].value;
				(*_data)[i][j].b = image[i][j].value;
				(*_data)[i][j].a = 1.f;
			}
		}
	}

	ColorImg::ColorImg(const scp::Mat<float>& red, const scp::Mat<float>& green, const scp::Mat<float>& blue, const scp::Mat<float>& alpha)
	{
		assert(red.shape(0) == green.shape(0) && green.shape(0) == blue.shape(0) && blue.shape(0) == alpha.shape(0));
		assert(red.shape(1) == green.shape(1) && green.shape(1) == blue.shape(1) && blue.shape(1) == alpha.shape(1));

		_data = std::make_unique<scp::Mat<ColorPixel>>(red.shape(0), red.shape(1));

		for (uint64_t i = 0; i < _data->shape(0); ++i)
		{
			for (uint64_t j = 0; j < _data->shape(1); ++j)
			{
				(*_data)[i][j].r = red[i][j];
				(*_data)[i][j].g = green[i][j];
				(*_data)[i][j].b = blue[i][j];
				(*_data)[i][j].a = alpha[i][j];
			}
		}
	}

	ColorImg::ColorImg(const ColorImg& image) : Img<ColorPixel>(image.getData())
	{
	}

	ColorImg& ColorImg::operator=(const ColorImg& image)
	{
		_data = std::make_unique<scp::Mat<ColorPixel>>(image.getData());
	}
}
