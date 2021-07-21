#pragma once

#include <DejaVu/DejaVu.hpp>

namespace djv
{
	template<uint64_t N>
	PixelBase<N>::PixelBase(float x)
	{
		std::fill<float*, float>(components, components + N, x);
	}

	template<uint64_t N>
	PixelBase<N>::PixelBase(float r, float g, float b, float a)
	{
		if constexpr (N >= 1)
		{
			components[0] = r;
		}
		if constexpr (N >= 2)
		{
			components[1] = g;
		}
		if constexpr (N >= 3)
		{
			components[2] = b;
		}
		if constexpr (N >= 4)
		{
			components[3] = a;
		}
		if constexpr (N >= 5)
		{
			std::fill<float*, float>(components + 4, components + N, 0.f);
		}
	}

	template<uint64_t N>
	float& PixelBase<N>::operator[](uint64_t i)
	{
		assert(i < N);
		return components[i];
	}

	template<uint64_t N>
	const float& PixelBase<N>::operator[](uint64_t i) const
	{
		assert(i < N);
		return components[i];
	}

	template<uint64_t N>
	PixelBase<N>& PixelBase<N>::operator+=(const PixelBase<N>& pixel)
	{
		for (uint64_t i = 0; i < N; i++)
		{
			components[i] += pixel.components[i];
		}

		return *this;
	}

	template<uint64_t N>
	PixelBase<N>& PixelBase<N>::operator-=(const PixelBase<N>& pixel)
	{
		for (uint64_t i = 0; i < N; i++)
		{
			components[i] -= pixel.components[i];
		}

		return *this;
	}

	template<uint64_t N>
	PixelBase<N>& PixelBase<N>::operator*=(const PixelBase<N>& pixel)
	{
		for (uint64_t i = 0; i < N; i++)
		{
			components[i] *= pixel.components[i];
		}

		return *this;
	}

	template<uint64_t N>
	PixelBase<N>& PixelBase<N>::operator/=(const PixelBase<N>& pixel)
	{
		for (uint64_t i = 0; i < N; i++)
		{
			components[i] /= pixel.components[i];
		}

		return *this;
	}

	template<uint64_t N>
	PixelBase<N>& PixelBase<N>::operator%=(const PixelBase<N>& pixel)
	{
		for (uint64_t i = 0; i < N; i++)
		{
			components[i] %= pixel.components[i];
		}

		return *this;
	}

	template<uint64_t N>
	PixelBase<N>& PixelBase<N>::operator+=(float x)
	{
		for (uint64_t i = 0; i < N; i++)
		{
			components[i] += x;
		}

		return *this;
	}

	template<uint64_t N>
	PixelBase<N>& PixelBase<N>::operator-=(float x)
	{
		for (uint64_t i = 0; i < N; i++)
		{
			components[i] -= x;
		}

		return *this;
	}

	template<uint64_t N>
	PixelBase<N>& PixelBase<N>::operator*=(float x)
	{
		for (uint64_t i = 0; i < N; i++)
		{
			components[i] *= x;
		}

		return *this;
	}

	template<uint64_t N>
	PixelBase<N>& PixelBase<N>::operator/=(float x)
	{
		for (uint64_t i = 0; i < N; i++)
		{
			components[i] /= x;
		}

		return *this;
	}

	template<uint64_t N>
	PixelBase<N>& PixelBase<N>::operator%=(float x)
	{
		for (uint64_t i = 0; i < N; i++)
		{
			components[i] %= x;
		}

		return *this;
	}

	template<uint64_t N>
	void PixelBase<N>::getRGBA(float& red, float& green, float& blue, float& alpha) const
	{
		if constexpr (N >= 1)
		{
			red = components[0];
		}
		else
		{
			red = 0.f;
		}

		if constexpr (N >= 2)
		{
			green = components[1];
		}
		else
		{
			green = red;
		}

		if constexpr (N >= 3)
		{
			blue = components[2];
		}
		else
		{
			blue = green;
		}

		if constexpr (N >= 4)
		{
			alpha = components[3];
		}
		else
		{
			alpha = 1.f;
		}
	}

	template<uint64_t N>
	PixelBase<N> operator+(const PixelBase<N>& pixelA, const PixelBase<N>& pixelB)
	{
		PixelBase<N> pixelC(pixelA);
		pixelC += pixelB;

		return pixelC;
	}

	template<uint64_t N>
	PixelBase<N>&& operator+(PixelBase<N>&& pixelA, const PixelBase<N>& pixelB)
	{
		pixelA += pixelB;
		return std::move(pixelA);
	}

	template<uint64_t N>
	PixelBase<N>&& operator+(const PixelBase<N>& pixelA, PixelBase<N>&& pixelB)
	{
		pixelB += pixelA;
		return std::move(pixelB);
	}

	template<uint64_t N>
	PixelBase<N>&& operator+(PixelBase<N>&& pixelA, PixelBase<N>&& pixelB)
	{
		pixelA += pixelB;
		return std::move(pixelA);
	}

	template<uint64_t N>
	PixelBase<N> operator-(const PixelBase<N>& pixelA, const PixelBase<N>& pixelB)
	{
		PixelBase<N> pixelC(pixelA);
		pixelC -= pixelB;

		return pixelC;
	}

	template<uint64_t N>
	PixelBase<N>&& operator-(PixelBase<N>&& pixelA, const PixelBase<N>& pixelB)
	{
		pixelA -= pixelB;
		return std::move(pixelA);
	}

	template<uint64_t N>
	PixelBase<N> operator*(const PixelBase<N>& pixelA, const PixelBase<N>& pixelB)
	{
		PixelBase<N> pixelC(pixelA);
		pixelC *= pixelB;

		return pixelC;
	}

	template<uint64_t N>
	PixelBase<N>&& operator*(PixelBase<N>&& pixelA, const PixelBase<N>& pixelB)
	{
		pixelA *= pixelB;
		return std::move(pixelA);
	}

	template<uint64_t N>
	PixelBase<N>&& operator*(const PixelBase<N>& pixelA, PixelBase<N>&& pixelB)
	{
		pixelB *= pixelA;
		return std::move(pixelB);
	}

	template<uint64_t N>
	PixelBase<N>&& operator*(PixelBase<N>&& pixelA, PixelBase<N>&& pixelB)
	{
		pixelA *= pixelB;
		return std::move(pixelA);
	}

	template<uint64_t N>
	PixelBase<N> operator/(const PixelBase<N>& pixelA, const PixelBase<N>& pixelB)
	{
		PixelBase<N> pixelC(pixelA);
		pixelC /= pixelB;

		return pixelC;
	}

	template<uint64_t N>
	PixelBase<N>&& operator/(PixelBase<N>&& pixelA, const PixelBase<N>& pixelB)
	{
		pixelA /= pixelB;
		return std::move(pixelA);
	}

	template<uint64_t N>
	PixelBase<N> operator%(const PixelBase<N>& pixelA, const PixelBase<N>& pixelB)
	{
		PixelBase<N> pixelC(pixelA);
		pixelC %= pixelB;

		return pixelC;
	}

	template<uint64_t N>
	PixelBase<N>&& operator%(PixelBase<N>&& pixelA, const PixelBase<N>& pixelB)
	{
		pixelA %= pixelB;
		return std::move(pixelA);
	}

	template<uint64_t N>
	PixelBase<N> operator+(const PixelBase<N>& pixel, float x)
	{
		PixelBase<N> result(pixel);
		result += x;

		return result;
	}

	template<uint64_t N>
	PixelBase<N>&& operator+(PixelBase<N>&& pixel, float x)
	{
		pixel += x;
		return std::move(pixel);
	}

	template<uint64_t N>
	PixelBase<N> operator+(float x, const PixelBase<N>& pixel)
	{
		PixelBase<N> result(pixel);
		result += x;

		return result;
	}

	template<uint64_t N>
	PixelBase<N>&& operator+(float x, PixelBase<N>&& pixel)
	{
		pixel += x;
		return std::move(pixel);
	}

	template<uint64_t N>
	PixelBase<N> operator-(const PixelBase<N>& pixel, float x)
	{
		PixelBase<N> result(pixel);
		result -= x;

		return result;
	}

	template<uint64_t N>
	PixelBase<N>&& operator-(PixelBase<N>&& pixel, float x)
	{
		pixel -= x;
		return std::move(pixel);
	}

	template<uint64_t N>
	PixelBase<N> operator*(const PixelBase<N>& pixel, float x)
	{
		PixelBase<N> result(pixel);
		result *= x;

		return result;
	}

	template<uint64_t N>
	PixelBase<N>&& operator*(PixelBase<N>&& pixel, float x)
	{
		pixel *= x;
		return std::move(pixel);
	}

	template<uint64_t N>
	PixelBase<N> operator*(float x, const PixelBase<N>& pixel)
	{
		PixelBase<N> result(pixel);
		result *= x;

		return result;
	}

	template<uint64_t N>
	PixelBase<N>&& operator*(float x, PixelBase<N>&& pixel)
	{
		pixel *= x;
		return std::move(pixel);
	}

	template<uint64_t N>
	PixelBase<N> operator/(const PixelBase<N>& pixel, float x)
	{
		PixelBase<N> result(pixel);
		result /= x;

		return result;
	}

	template<uint64_t N>
	PixelBase<N>&& operator/(PixelBase<N>&& pixel, float x)
	{
		pixel /= x;
		return std::move(pixel);
	}

	template<uint64_t N>
	PixelBase<N> operator%(const PixelBase<N>& pixel, float x)
	{
		PixelBase<N> result(pixel);
		result %= x;

		return result;
	}

	template<uint64_t N>
	PixelBase<N>&& operator%(PixelBase<N>&& pixel, float x)
	{
		pixel %= x;
		return std::move(pixel);
	}
}

namespace std
{
	template<uint64_t N>
	float norm(const djv::PixelBase<N>& pixel)
	{
		float s = 0.f;
		for (uint64_t i = 0; i < N; ++i)
		{
			s += pixel[i] * pixel[i];
		}

		return s;
	}

	template<uint64_t N>
	float abs(const djv::PixelBase<N>& pixel)
	{
		return sqrt(norm(pixel));
	}
}

namespace djv
{
	template<typename PixelType>
	Img<PixelType>::Img() :
		_data(nullptr)
	{
	}

	template<typename PixelType>
	Img<PixelType>::Img(uint64_t width, uint64_t height, PixelType pixelInit) :
		_data(std::make_unique<scp::Mat<PixelType>>(width, height, pixelInit))
	{
	}

	template<typename PixelType>
	Img<PixelType>::Img(const scp::Mat<PixelType>& mat) :
		_data(std::make_unique<scp::Mat<PixelType>>(mat))
	{
	}

	template<typename PixelType>
	Img<PixelType>::Img(scp::Mat<PixelType>&& mat) :
		_data(std::make_unique<scp::Mat<PixelType>>(std::move(mat)))
	{
	}

	template<typename PixelType>
	Img<PixelType>::Img(const std::string& filename, bool transpose, bool flipHorizontally, bool flipVertically) : Img<PixelType>()
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

		_data = std::make_unique<scp::Mat<PixelType>>(imageWidth, imageHeight);

		for (uint64_t i = 0; i < w; ++i)
		{
			for (uint64_t j = 0; j < h; ++j)
			{
				uint64_t x = flipHorizontally && !transpose || flipVertically && transpose ? w - i - 1 : i;
				uint64_t y = flipVertically && !transpose || flipHorizontally && transpose ? h - j - 1 : j;
				uint8_t* it = image + 4 * (x + w * y);

				float r = *it / 255.f;
				float g = *(++it) / 255.f;
				float b = *(++it) / 255.f;
				float a = *(++it) / 255.f;

				if (transpose)
				{
					(*_data)[j][i] = PixelType(r, g, b, a);
				}
				else
				{
					(*_data)[i][j] = PixelType(r, g, b, a);
				}
			}
		}

		stbi_image_free(image);
	}

	template<typename PixelType>
	Img<PixelType>::Img(const Img<PixelType>& image) :
		_data(std::make_unique<scp::Mat<PixelType>>(*image._data))
	{
	}

	template<typename PixelType>
	Img<PixelType>& Img<PixelType>::operator=(const Img<PixelType>& image)
	{
		_data = std::make_unique<scp::Mat<PixelType>>(*image._data);
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
	scp::Mat<float> Img<PixelType>::getComponent(uint64_t i) const
	{
		scp::Mat<float> r(_data->shape(0), _data->shape(1));

		for (uint64_t i = 0; i < r.shape(0); ++i)
		{
			for (uint64_t j = 0; j < r.shape(1); ++j)
			{
				r[i][j] = (*_data)[i][j][i];
			}
		}

		return r;
	}

	template<typename PixelType>
	void Img<PixelType>::setComponent(uint64_t i, const scp::Mat<float>& comp)
	{
		assert(comp.shape(0) == _data->shape(0) && comp.shape(1) == _data->shape(1));
		assert(i < PixelType::componentCount);

		for (uint64_t x = 0; x < comp.shape(0); ++x)
		{
			for (uint64_t y = 0; y < comp.shape(1); ++y)
			{
				(*_data)[x][y][i] = comp[x][y];
			}
		}
	}

	template<typename PixelType>
	void Img<PixelType>::normalize(PixelType min, PixelType max)
	{
		PixelType currentMin = (*_data)[0][0], currentMax = (*_data)[0][0];
		for (uint64_t i = 0; i < _data->shape(0); ++i)
		{
			for (uint64_t j = 0; j < _data->shape(1); ++j)
			{
				for (uint64_t k = 0; k < PixelType::componentCount; ++k)
				{
					if ((*_data)[i][j][k] > currentMax[k])
					{
						currentMax[k] = (*_data)[i][j][k];
					}
					if ((*_data)[i][j][k] < currentMin[k])
					{
						currentMin[k] = (*_data)[i][j][k];
					}
				}
			}
		}

		for (uint64_t k = 0; k < PixelType::componentCount; ++k)
		{
			for (uint64_t i = 0; i < _data->shape(0); ++i)
			{
				for (uint64_t j = 0; j < _data->shape(1); ++j)
				{
					if (currentMin[k] == currentMax[k])
					{
						(*_data)[i][j][k] = currentMin[k] < min[k] ? min[k] : max[k];
					}
					else
					{
						(*_data)[i][j][k] = min[k] + (max[k] - min[k])*((*_data)[i][j][k] - currentMin[k])/(currentMax[k] - currentMin[k]);
					}
				}
			}
		}
	}

	template<typename PixelType>
	template<typename T>
	Img<T> Img<PixelType>::castToPixelType() const
	{
		scp::Mat<T> mat(_data->shape(0), _data->shape(1));
		for (uint64_t i = 0; i < _data->shape(0); ++i)
		{
			for (uint64_t j = 0; j < _data->shape(1); ++j)
			{
				float r, g, b, a;
				(*_data)[i][j].getRGBA(r, g, b, a);

				mat[i][j] = T(r, g, b, a);
			}
		}

		return Img<T>(mat);
	}

	template<typename PixelType>
	void Img<PixelType>::saveToFile(const std::string& filename) const
	{
		assert(filename.size() > 4);

		uint64_t w = _data->shape(0), h = _data->shape(1);
		std::vector<uint8_t> image(4 * _data->globalLength());
		std::vector<uint8_t>::iterator it = image.begin();

		for (uint64_t j(0); j < h; ++j)
		{
			for (uint64_t i(0); i < w; ++i)
			{
				float r, g, b, a;
				(*_data)[i][j].getRGBA(r, g, b, a);
				*it = 255 * std::max(std::min(r, 1.f), 0.f);
				*(++it) = 255 * std::max(std::min(g, 1.f), 0.f);
				*(++it) = 255 * std::max(std::min(b, 1.f), 0.f);
				*(++it) = 255 * std::max(std::min(a, 1.f), 0.f);
				++it;
			}
		}

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
}
