#include <DejaVu/DejaVu.hpp>

namespace djv
{
	template<typename PixelType>
	Img<PixelType>::Img() :
		_data(nullptr)
	{
	}

	template<typename PixelType>
	Img<PixelType>::Img(uint64_t width, uint64_t height, PixelType pixelInit) :
		_data(std::make_unique<scp::Mat<float>>(width, height, pixelInit))
	{
	}

	template<typename PixelType>
	Img<PixelType>::Img(const scp::Mat<PixelType>& mat) :
		_data(std::make_unique<scp::Mat<float>>(mat))
	{
	}

	template<typename PixelType>
	Img<PixelType>::Img(scp::Mat<PixelType>&& mat) :
		_data(std::make_unique<scp::Mat<float>>(std::move(mat)))
	{
	}

	template<typename PixelType>
	Img<PixelType>::Img(const Img<PixelType>& image) :
		_data(std::make_unique<scp::Mat<PixelType>>(*image._data))
	{
	}

	template<typename PixelType>
	Img<PixelType>& Img<PixelType>::operator=(const Img<PixelType>& image)
	{
		_data = std::make_unique<scp::Mat<float>>(*image._data);
	}

	template<typename PixelType>
	scp::Vec<PixelType>& Img<PixelType>::operator[](uint64_t i)
	{
		return (*_data)[i];
	}

	template<typename PixelType>
	const scp::Vec<PixelType>& Img<PixelType>::operator[](uint64_t i) const
	{
		return (*_data)[i];
	}

	template<typename PixelType>
	uint64_t Img<PixelType>::width() const
	{
		return _data->shape(0);
	}

	template<typename PixelType>
	uint64_t Img<PixelType>::height() const
	{
		return _data->shape(1);
	}

	template<typename PixelType>
	const scp::Mat<PixelType>& Img<PixelType>::getData() const
	{
		return *_data;
	}

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

	void ImgGrayscale::saveToFile(const std::string& filename) const
	{
		uint64_t w = _data->shape(0), h = _data->shape(1);
		std::vector<uint8_t> image(3 * _data->globalLength());
		for (uint64_t i(0); i < w; i++)
		{
			for (uint64_t j(0); j < h; j++)
			{
				uint8_t color = 255.f * std::min(std::max((*_data)[i][j], 0.f), 1.f);
				image[3 * (i + w * j)] = color;
				image[3 * (i + w * j) + 1] = color;
				image[3 * (i + w * j) + 2] = color;
			}
		}

		assert(filename.size() > 4);

		if (filename.substr(filename.size() - 4, 4) == ".bmp")
		{
			stbi_write_bmp(filename.c_str(), w, h, 3, image.data());
			return;
		}
		else if (filename.substr(filename.size() - 4, 4) == ".jpg")
		{
			stbi_write_jpg(filename.c_str(), w, h, 3, image.data(), 95);
			return;
		}
		else if (filename.substr(filename.size() - 4, 4) == ".png")
		{
			stbi_write_png(filename.c_str(), w, h, 3, image.data(), 3 * w);
			return;
		}
		else if (filename.substr(filename.size() - 4, 4) == ".tga")
		{
			stbi_write_tga(filename.c_str(), w, h, 3, image.data());
			return;
		}

		assert(filename.size() > 5);

		if (filename.substr(filename.size() - 5, 5) == ".jpeg")
		{
			stbi_write_jpg(filename.c_str(), w, h, 3, image.data(), 95);
			return;
		}
		else
		{
			throw std::runtime_error("File extension '" + filename + "' unrecognized.");
		}
	}


	PixelRGBA::PixelRGBA(float x) :
		r(x),
		g(x),
		b(x),
		a(x)
	{
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

	void ImgRGBA::saveToFile(const std::string & filename) const
	{
		uint64_t w = _data->shape(0), h = _data->shape(1);
		std::vector<uint8_t> image(4 * _data->globalLength());
		for (uint64_t i(0); i < w; i++)
		{
			for (uint64_t j(0); j < h; j++)
			{
				image[4 * (i + w * j)] = 255.f * std::min(std::max((*_data)[i][j].r, 0.f), 1.f);
				image[4 * (i + w * j) + 1] = 255.f * std::min(std::max((*_data)[i][j].g, 0.f), 1.f);
				image[4 * (i + w * j) + 2] = 255.f * std::min(std::max((*_data)[i][j].b, 0.f), 1.f);
				image[4 * (i + w * j) + 3] = 255.f * std::min(std::max((*_data)[i][j].a, 0.f), 1.f);
			}
		}

		assert(filename.size() > 4);

		if (filename.substr(filename.size() - 4, 4) == ".bmp")
		{
			stbi_write_bmp(filename.c_str(), w, h, 4, image.data());
			return;
		}
		else if (filename.substr(filename.size() - 4, 4) == ".jpg")
		{
			stbi_write_jpg(filename.c_str(), w, h, 4, image.data(), 95);
			return;
		}
		else if (filename.substr(filename.size() - 4, 4) == ".png")
		{
			stbi_write_png(filename.c_str(), w, h, 4, image.data(), 4 * w);
			return;
		}
		else if (filename.substr(filename.size() - 4, 4) == ".tga")
		{
			stbi_write_tga(filename.c_str(), w, h, 4, image.data());
			return;
		}

		assert(filename.size() > 5);

		if (filename.substr(filename.size() - 5, 5) == ".jpeg")
		{
			stbi_write_jpg(filename.c_str(), w, h, 4, image.data(), 95);
			return;
		}
		else
		{
			throw std::runtime_error("File extension '" + filename + "' unrecognized.");
		}
	}
}
