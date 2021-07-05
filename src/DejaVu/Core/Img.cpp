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

	GrayScalePixel& GrayScalePixel::operator+=(const GrayScalePixel& pixel)
	{
		value += pixel.value;
		return *this;
	}

	GrayScalePixel& GrayScalePixel::operator-=(const GrayScalePixel& pixel)
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

	GrayScaleImg::GrayScaleImg(const RGBAImg& image, float redWeight, float greenWeight, float blueWeight)
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
		return *this;
	}
}

namespace djv
{
	RGBAPixel::RGBAPixel(float x) :
		r(x),
		g(x),
		b(x),
		a(1.f)
	{
	}

	RGBAPixel::RGBAPixel(float red, float green, float blue, float alpha) :
		r(red),
		g(green),
		b(blue),
		a(alpha)
	{
	}

	RGBAPixel& RGBAPixel::operator+=(const RGBAPixel& pixel)
	{
		r += pixel.r;
		g += pixel.g;
		b += pixel.b;
		a += pixel.a;

		return *this;
	}

	RGBAPixel& RGBAPixel::operator-=(const RGBAPixel& pixel)
	{
		r -= pixel.r;
		g -= pixel.g;
		b -= pixel.b;
		a -= pixel.a;

		return *this;
	}

	RGBAPixel& RGBAPixel::operator*=(float x)
	{
		r *= x;
		g *= x;
		b *= x;
		a *= x;

		return *this;
	}

	RGBAPixel& RGBAPixel::operator/=(float x)
	{
		r /= x;
		g /= x;
		b /= x;
		a /= x;

		return *this;
	}

	float RGBAPixel::getComponent(uint64_t i) const
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

	void RGBAPixel::setComponent(uint64_t i, float x)
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

	void RGBAPixel::getRGBA(uint8_t& red, uint8_t& green, uint8_t& blue, uint8_t& alpha) const
	{
		red = 255 * std::min(std::max(r, 0.f), 1.f);
		green = 255 * std::min(std::max(g, 0.f), 1.f);
		blue = 255 * std::min(std::max(b, 0.f), 1.f);
		alpha = 255 * std::min(std::max(a, 0.f), 1.f);
		alpha = 255;
	}

	RGBAPixel operator+(const RGBAPixel& p, const RGBAPixel& q)
	{
		return { p.r + q.r, p.g + q.g, p.b + q.b, p.a + q.a };
	}

	RGBAPixel&& operator+(RGBAPixel&& p, const RGBAPixel& q)
	{
		p += q;
		return std::move(p);
	}

	RGBAPixel&& operator+(const RGBAPixel& p, RGBAPixel&& q)
	{
		q += p;
		return std::move(q);
	}

	RGBAPixel&& operator+(RGBAPixel&& p, RGBAPixel&& q)
	{
		p += q;
		return std::move(p);
	}

	RGBAPixel operator-(const RGBAPixel& p, const RGBAPixel& q)
	{
		return { p.r - q.r, p.g - q.g, p.b - q.b, p.a - q.a };
	}

	RGBAPixel&& operator-(RGBAPixel&& p, const RGBAPixel& q)
	{
		p -= q;
		return std::move(p);
	}

	RGBAPixel operator*(const RGBAPixel& p, float x)
	{
		return { p.r * x, p.g * x, p.b * x, p.a * x };
	}

	RGBAPixel&& operator*(RGBAPixel&& p, float x)
	{
		p *= x;
		return std::move(p);
	}

	RGBAPixel operator*(float x, const RGBAPixel& p)
	{
		return { p.r * x, p.g * x, p.b * x, p.a * x };
	}

	RGBAPixel&& operator*(float x, RGBAPixel&& p)
	{
		p *= x;
		return std::move(p);
	}

	RGBAPixel operator/(const RGBAPixel& p, float x)
	{
		return { p.r / x, p.g / x, p.b / x, p.a / x };
	}

	RGBAPixel&& operator/(RGBAPixel&& p, float x)
	{
		p /= x;
		return std::move(p);
	}
}

namespace std
{
	float norm(const djv::RGBAPixel& pixel)
	{
		return pixel.r * pixel.r + pixel.g * pixel.g + pixel.b * pixel.b + pixel.a * pixel.a;
	}

	float abs(const djv::RGBAPixel& pixel)
	{
		return sqrt(norm(pixel));
	}
}

namespace djv
{
	RGBAImg::RGBAImg(const Img<RGBAPixel>& image) : Img<RGBAPixel>(image)
	{
	}

	RGBAImg::RGBAImg(Img<RGBAPixel>&& image) : Img<RGBAPixel>(std::move(image))
	{
	}

	RGBAImg::RGBAImg(const GrayScaleImg& image)
	{
		_data = std::make_unique<scp::Mat<RGBAPixel>>(image.width(), image.height());

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

	RGBAImg::RGBAImg(const scp::Mat<float>& red, const scp::Mat<float>& green, const scp::Mat<float>& blue, const scp::Mat<float>& alpha)
	{
		assert(red.shape(0) == green.shape(0) && green.shape(0) == blue.shape(0) && blue.shape(0) == alpha.shape(0));
		assert(red.shape(1) == green.shape(1) && green.shape(1) == blue.shape(1) && blue.shape(1) == alpha.shape(1));

		_data = std::make_unique<scp::Mat<RGBAPixel>>(red.shape(0), red.shape(1));

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

	RGBAImg::RGBAImg(const RGBAImg& image) : Img<RGBAPixel>(image.getData())
	{
	}

	RGBAImg& RGBAImg::operator=(const RGBAImg& image)
	{
		_data = std::make_unique<scp::Mat<RGBAPixel>>(image.getData());
		return *this;
	}
}
