#include <DejaVu/DejaVu.hpp>

namespace djv
{
	GrayScalePixel::GrayScalePixel(const PixelBase<1>& pixel) : PixelBase<1>(pixel)
	{
	}

	GrayScalePixel::GrayScalePixel(PixelBase<1>&& pixel) : PixelBase<1>(std::move(pixel))
	{
	}

	GrayScalePixel::GrayScalePixel(float r, float g, float b, float a)
	{
		components[0] = r * 0.299f + g * 0.587f + b * 0.114f;
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
				(*_data)[i][j] = image[i][j][0] * redWeight + image[i][j][1] * greenWeight + image[i][j][2] * blueWeight;
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
				(*_data)[i][j][0] = image[i][j];
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
	RGBAPixel::RGBAPixel(float x) : PixelBase<4>(x)
	{
		components[3] = 1.f;
	}

	RGBAPixel::RGBAPixel(const PixelBase<4>& pixel) : PixelBase<4>(pixel)
	{
	}

	RGBAPixel::RGBAPixel(PixelBase<4>&& pixel) : PixelBase<4>(std::move(pixel))
	{
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
				(*_data)[i][j][0] = image[i][j][0];
				(*_data)[i][j][1] = image[i][j][0];
				(*_data)[i][j][2] = image[i][j][0];
				(*_data)[i][j][3] = 1.f;
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
				(*_data)[i][j][0] = red[i][j];
				(*_data)[i][j][1] = green[i][j];
				(*_data)[i][j][2] = blue[i][j];
				(*_data)[i][j][3] = alpha[i][j];
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
