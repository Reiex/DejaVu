///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Pélégrin Marius
//! \copyright The MIT License (MIT)
//! \date 2020-2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <DejaVu/DejaVu.hpp>

/*

TODO:
	Image:
		- Find something to do when read/write to file fails... (at least return a bool when write fail and put in invalid state when read fail ?)
		- Replace 'swizzling' by some 'PixelConversion<TPixelFrom, TPixelTo>' with default value...
		- Change width and height from uint64_t to uint32_t

	ImageProcessing:
		- Corner detection etc...

*/

int main()
{
	djv::Image_rgba_u8 image("examples/assets/Lena.pnm");
	uint64_t w = image.getWidth(), h = image.getHeight();

	djv::Image_rgba_u8 resized(image, 1024, 768, scp::InterpolationMethod::Linear);
	djv::proc::PrImage_rgb_f32 processing(resized, [](const djv::Pixel_rgba_u8& x, djv::Pixel_rgb_f32& y) { y.set(0, x[0]); y.set(1, x[1]); y.set(2, x[2]); });


	// Image manipulation
	
	image.saveToFile("build/Original.pnm");
	resized.saveToFile("build/Resized.pnm");
	
	{
		uint8_t gsSwizzling[4] = { 0, 0, 0, -1 };
		djv::Image_gs_u8 grayscale(resized, [](const djv::Pixel_rgba_u8& x, djv::Pixel_gs_u8& y) { y[0] = 0.299 * x[0] + 0.587 * x[1] + 0.114 * x[2]; });
		grayscale.saveToFile("build/Grayscale.pnm", gsSwizzling);
	}
	{
		djv::Image_rgba_u8 result(image, w * 0.1, h * 0.2, w * 0.4, h * 0.7);
		result.saveToFile("build/Cropped.pnm");
	}
	{
		djv::Image_rgba_u8 result(resized);
		result.transpose();
		result.saveToFile("build/Transposed.pnm");
	}
	{
		djv::Image_rgba_u8 result(resized);
		result.rotate<scp::InterpolationMethod::Linear, scp::BorderBehaviour::Zero>(0.5);
		result.saveToFile("build/Rotated.pnm");
	}
	{
		djv::Image_rgba_u8 result(resized);
		result.flip<true, true>();
		result.saveToFile("build/Flipped.pnm");
	}
	{
		djv::ShapeCrown crown(256, 256, 256, 64);
		djv::ShapeThickRect rect(100, 50, 100, 51, 7);
		djv::ShapeLine line(257, 100, 200, 400);
	
		djv::Image_rgba_u8 result(512, 512, { 0, 0, 0, 0 });
		result.draw(crown, image);
		result.draw(rect, { 0, 255, 255, 255 });
		result.draw(line, { 255, 0, 0, 255 });
		result.saveToFile("build/Shapes.pnm");
	}
	
	
	// Blurs
	
	{
		djv::Image_rgba_u8 result(resized);
		result.blurGaussian<scp::BorderBehaviour::Continuous>(10.f);
		result.saveToFile("build/BlurGaussian.pnm");
	}
	{
		djv::Image_rgba_u8 result(resized);
		result.blurMean<scp::BorderBehaviour::Continuous>(10);
		result.saveToFile("build/BlurMean.pnm");
	}
	{
		djv::Image_rgba_u8 result(resized);
		result.blurMedian<scp::BorderBehaviour::Continuous>(10);
		result.saveToFile("build/BlurMedian.pnm");
	}
	{
		djv::Image_rgba_u8 result(image);
		result.filterGaussianBilateral<scp::BorderBehaviour::Continuous>(5, 0.01);
		result.saveToFile("build/BlurGaussianBilateral.pnm");
	}
	
	
	// Image Processing
	
	{
		djv::proc::PrImage_rgb_f32 result(processing);
		result.fft();
		result.normalize(-1.f, 100.f);
		result.saveToFile("build/FourierTransform.pnm");
	}
	{
		djv::proc::PrImage_rgb_f32 result(processing);
		result.gradientX<scp::BorderBehaviour::Continuous, djv::proc::GradientMethod::Scharr>();
		result.normalize();
		result.saveToFile("build/GradientX.pnm");
	}
	{
		djv::proc::PrImage_rgb_f32 result(processing);
		result.gradientY<scp::BorderBehaviour::Continuous, djv::proc::GradientMethod::Scharr>();
		result.normalize();
		result.saveToFile("build/GradientY.pnm");
	}
	{
		djv::proc::PrImage_rgb_f32 result(processing);
		result.laplacian<scp::BorderBehaviour::Continuous, djv::proc::LaplacianMethod::Naive>();
		result.normalize();
		result.saveToFile("build/Laplacian.pnm");
	}
	
	
	// Feature detection
	
	{
		djv::proc::PrImage_rgb_f32 result(processing);
		result.canny<scp::BorderBehaviour::Continuous>();
		result.normalize();
		result.saveToFile("build/Canny.pnm");
	}
	{
		djv::proc::PrImage_rgb_f32 result(processing);
		result.blurGaussian<scp::BorderBehaviour::Continuous>(2);
		result.marrHildreth<scp::BorderBehaviour::Continuous>();
		result.normalize();
		result.saveToFile("build/MarrHildreth.pnm");
	}
	
	
	// Clustering
	
	{
		djv::proc::Clustering clustering = processing.kMeans(10);
		djv::proc::PrImage_rgb_f32 result(processing);
		clustering.applyTo(result);
		result.saveToFile("build/kMeans.pnm");
	}
	{
		djv::proc::Clustering clustering = processing.slicSuperpixels(1000, 0.2f);
		djv::proc::PrImage_rgb_f32 result(processing);
		clustering.applyTo(result);
		result.saveToFile("build/SLIC.pnm");
	}

	return 0;
}
