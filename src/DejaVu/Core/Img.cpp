#include <DejaVu/DejaVu.hpp>

namespace djv
{
	ImgGrayscale::ImgGrayscale(const Img<float>& image) : Img<float>(image)
	{
	}

	ImgGrayscale::ImgGrayscale(Img<float>&& image) : Img<float>(std::move(image))
	{
	}

	ImgGrayscale::ImgGrayscale(const std::string& filename, bool transpose, bool flipHorizontally, bool flipVertically, float redWeight, float greenWeight, float blueWeight)
	{
		int w, h;
		uint8_t* image = stbi_load(filename.c_str(), &w, &h, nullptr, STBI_rgb);
		if (image == nullptr)
		{
			throw std::runtime_error("Couldn't open '" + filename + "'.");
		}

		uint64_t imageWidth = w, imageHeight = h;
		if (transpose)
		{
			std::swap(w, h);
		}

		_data = std::make_unique<scp::Mat<float>>(imageWidth, imageHeight);

		for (uint64_t i = 0; i < w; ++i)
		{
			for (uint64_t j = 0; j < h; ++j)
			{
				uint64_t x = flipHorizontally && !transpose || flipVertically && transpose ? w - i - 1 : i;
				uint64_t y = flipVertically && !transpose || flipHorizontally && transpose ? h - j - 1 : j;
				uint8_t* it = image + 3 * (x + w * y);

				float s = redWeight * *it;
				s += greenWeight * *(++it);
				s += blueWeight * *(++it);
				s /= 255.f;

				if (transpose)
				{
					(*_data)[j][i] = s;
				}
				else
				{
					(*_data)[i][j] = s;
				}
			}
		}

		stbi_image_free(image);
	}

	ImgGrayscale::ImgGrayscale(const ImgRGBA& image, float redWeight, float greenWeight, float blueWeight)
	{
		_data = std::make_unique<scp::Mat<float>>(image.width(), image.height());

		for (uint64_t i = 0; i < _data->shape(0); ++i)
		{
			for (uint64_t j = 0; j < _data->shape(1); ++j)
			{
				(*_data)[i][j] = image[i][j].r * redWeight + image[i][j].g * greenWeight + image[i][j].b * blueWeight;
			}
		}
	}

	void ImgGrayscale::saveToFile(const std::string& filename) const
	{
		uint64_t w = _data->shape(0), h = _data->shape(1);
		std::vector<uint8_t> image(4 * _data->globalLength());
		std::vector<uint8_t>::iterator it = --image.begin();

		for (uint64_t j(0); j < h; j++)
		{
			for (uint64_t i(0); i < w; i++)
			{
				uint8_t color = 255.f * std::min(std::max((*_data)[i][j], 0.f), 1.f);
				*(++it) = color;
				*(++it) = color;
				*(++it) = color;
				*(++it) = 255;
			}
		}

		saveRawToFile(filename, image);
	}


	PixelRGBA::PixelRGBA(float x) :
		r(x),
		g(x),
		b(x),
		a(x)
	{
	}

	PixelRGBA::PixelRGBA(float red, float green, float blue, float alpha) :
		r(red),
		g(green),
		b(blue),
		a(alpha)

	{
	}

	float& PixelRGBA::operator[](uint8_t i)
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

	const float& PixelRGBA::operator[](uint8_t i) const
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

	float& PixelRGBA::getComponent(Component component)
	{
		switch (component)
		{
		case Component::RED:
			return r;
		case Component::GREEN:
			return g;
		case Component::BLUE:
			return b;
		default:
			return a;
		}
	}

	const float& PixelRGBA::getComponent(Component component) const
	{
		switch (component)
		{
		case Component::RED:
			return r;
		case Component::GREEN:
			return g;
		case Component::BLUE:
			return b;
		default:
			return a;
		}
	}

	PixelRGBA& PixelRGBA::operator+=(const PixelRGBA& p)
	{
		r += p.r;
		g += p.g;
		b += p.b;
		a += p.a;

		return *this;
	}

	PixelRGBA& PixelRGBA::operator-=(const PixelRGBA& p)
	{
		r -= p.r;
		g -= p.g;
		b -= p.b;
		a -= p.a;

		return *this;
	}

	PixelRGBA& PixelRGBA::operator*=(float x)
	{
		r *= x;
		g *= x;
		b *= x;
		a *= x;

		return *this;
	}

	PixelRGBA& PixelRGBA::operator/=(float x)
	{
		r /= x;
		g /= x;
		b /= x;
		a /= x;

		return *this;
	}

	PixelRGBA operator+(const PixelRGBA& p, const PixelRGBA& q)
	{
		PixelRGBA r(p);
		r += q;

		return r;
	}

	PixelRGBA&& operator+(PixelRGBA&& p, const PixelRGBA& q)
	{
		p += q;
		return std::move(p);
	}

	PixelRGBA&& operator+(const PixelRGBA& p, PixelRGBA&& q)
	{
		q += p;
		return std::move(q);
	}

	PixelRGBA&& operator+(PixelRGBA&& p, PixelRGBA&& q)
	{
		p += q;
		return std::move(p);
	}

	PixelRGBA operator-(const PixelRGBA& p, const PixelRGBA& q)
	{
		PixelRGBA r(p);
		r -= q;

		return r;
	}

	PixelRGBA&& operator-(PixelRGBA&& p, const PixelRGBA& q)
	{
		p -= q;
		return std::move(p);
	}

	PixelRGBA operator*(const PixelRGBA& p, float x)
	{
		PixelRGBA r(p);
		r *= x;

		return r;
	}

	PixelRGBA&& operator*(PixelRGBA&& p, float x)
	{
		p *= x;
		return std::move(p);
	}

	PixelRGBA operator*(float x, const PixelRGBA& p)
	{
		PixelRGBA r(p);
		r *= x;

		return r;
	}

	PixelRGBA&& operator*(float x, PixelRGBA&& p)
	{
		p *= x;
		return std::move(p);
	}

	PixelRGBA operator/(const PixelRGBA& p, float x)
	{
		PixelRGBA r(p);
		r /= x;

		return r;
	}

	PixelRGBA&& operator/(PixelRGBA&& p, float x)
	{
		p /= x;
		return std::move(p);
	}
}

namespace std
{
	float norm(const djv::PixelRGBA& p)
	{
		return p.r*p.r + p.g*p.g + p.b*p.b + p.a*p.a;
	}

	float abs(const djv::PixelRGBA& p)
	{
		return sqrt(norm(p));
	}
}

namespace djv
{
	ImgRGBA::ImgRGBA(const Img<PixelRGBA>& image) : Img(image)
	{
	}

	ImgRGBA::ImgRGBA(Img<PixelRGBA>&& image) : Img(std::move(image))
	{
	}

	ImgRGBA::ImgRGBA(const std::string& filename, bool transpose, bool flipHorizontally, bool flipVertically)
	{
		int w, h;
		uint8_t* image = stbi_load(filename.c_str(), &w, &h, nullptr, STBI_rgb_alpha);
		if (image == nullptr)
		{
			throw std::runtime_error("Couldn't open '" + filename + "'.");
		}

		uint64_t imageWidth = w, imageHeight = h;
		if (transpose)
		{
			std::swap(w, h);
		}

		_data = std::make_unique<scp::Mat<PixelRGBA>>(imageWidth, imageHeight);

		for (uint64_t i = 0; i < w; ++i)
		{
			for (uint64_t j = 0; j < h; ++j)
			{
				uint64_t x = flipHorizontally && !transpose || flipVertically && transpose ? w - i - 1 : i;
				uint64_t y = flipVertically && !transpose || flipHorizontally && transpose ? h - j - 1 : j;
				uint8_t* it = image + 4 * (x + w * y);

				if (transpose)
				{
					(*_data)[j][i].r = *it / 255.f;
					(*_data)[j][i].g = *++it / 255.f;
					(*_data)[j][i].b = *++it / 255.f;
					(*_data)[j][i].a = *++it / 255.f;
				}
				else
				{
					(*_data)[i][j].r = *it / 255.f;
					(*_data)[i][j].g = *++it / 255.f;
					(*_data)[i][j].b = *++it / 255.f;
					(*_data)[i][j].a = *++it / 255.f;
				}
			}
		}

		stbi_image_free(image);
	}

	ImgRGBA::ImgRGBA(const ImgGrayscale& image)
	{
		_data = std::make_unique<scp::Mat<PixelRGBA>>(image.width(), image.height());

		for (uint64_t i = 0; i < _data->shape(0); ++i)
		{
			for (uint64_t j = 0; j < _data->shape(1); ++j)
			{
				(*_data)[i][j].r = image[i][j];
				(*_data)[i][j].g = image[i][j];
				(*_data)[i][j].b = image[i][j];
				(*_data)[i][j].a = 1.f;
			}
		}
	}

	ImgRGBA::ImgRGBA(const scp::Mat<float>& red, const scp::Mat<float>& green, const scp::Mat<float>& blue, const scp::Mat<float>& alpha)
	{
		assert(red.shape(0) == green.shape(0) && green.shape(0) == blue.shape(0) && blue.shape(0) == alpha.shape(0));
		assert(red.shape(1) == green.shape(1) && green.shape(1) == blue.shape(1) && blue.shape(1) == alpha.shape(1));

		_data = std::make_unique<scp::Mat<PixelRGBA>>(red.shape(0), red.shape(1));

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

	scp::Mat<float> ImgRGBA::getComponent(PixelRGBA::Component component) const
	{
		scp::Mat<float> r(_data->shape(0), _data->shape(1));

		for (uint64_t i = 0; i < r.shape(0); i++)
		{
			for (uint64_t j = 0; j < r.shape(1); j++)
			{
				r[i][j] = (*_data)[i][j].getComponent(component);
			}
		}

		return r;
	}

	void ImgRGBA::saveToFile(const std::string & filename) const
	{
		uint64_t w = _data->shape(0), h = _data->shape(1);
		std::vector<uint8_t> image(4 * _data->globalLength());
		std::vector<uint8_t>::iterator it = --image.begin();

		for (uint64_t j(0); j < h; j++)
		{
			for (uint64_t i(0); i < w; i++)
			{
				*(++it) = 255.f * std::min(std::max((*_data)[i][j].r, 0.f), 1.f);
				*(++it) = 255.f * std::min(std::max((*_data)[i][j].g, 0.f), 1.f);
				*(++it) = 255.f * std::min(std::max((*_data)[i][j].b, 0.f), 1.f);
				*(++it) = 255.f * std::min(std::max((*_data)[i][j].a, 0.f), 1.f);
			}
		}

		saveRawToFile(filename, image);
	}
}
