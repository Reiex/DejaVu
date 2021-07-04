#pragma once

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
	void Img<PixelType>::saveToFile(const std::string& filename) const
	{
		assert(filename.size() > 4);

		uint64_t w = _data->shape(0), h = _data->shape(1);
		std::vector<uint8_t> image(4 * _data->globalLength());
		std::vector<uint8_t>::iterator it = --image.begin();

		for (uint64_t j(0); j < h; ++j)
		{
			for (uint64_t i(0); i < w; ++i)
			{
				uint8_t r, g, b, a;
				(*_data)[i][j].getRGBA(r, g, b, a);
				*(++it) = r;
				*(++it) = g;
				*(++it) = b;
				*(++it) = a;
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
	scp::Mat<float> Img<PixelType>::getComponent(uint64_t comp) const
	{
		scp::Mat<float> r(_data->shape(0), _data->shape(1));

		for (uint64_t i = 0; i < r.shape(0); ++i)
		{
			for (uint64_t j = 0; j < r.shape(1); ++j)
			{
				r[i][j] = (*_data)[i][j].getComponent(comp);
			}
		}

		return r;
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
