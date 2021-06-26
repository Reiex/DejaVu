#include <DejaVu/DejaVu.hpp>

#include <ctime>

int main()
{
	/*
	// IMAGE PROCESSING

	djv::Img image("examples/assets/Lena.jpg"), result(image.width(), image.height());
	scp::Mat<float> grayScaleImage = image.grayScale();
	image.saveToFile("build/Original.png");
	djv::Img(grayScaleImage).saveToFile("build/GrayScaleOriginal.png");

	// Operators

	
	djv::Img(djv::operators::sobel(grayScaleImage)[0]).saveToFile("build/SobelX.png");
	djv::Img(djv::operators::sobel(grayScaleImage)[1]).saveToFile("build/SobelY.png");
	djv::Img(djv::operators::prewitt(grayScaleImage)[0]).saveToFile("build/PrewittX.png");
	djv::Img(djv::operators::prewitt(grayScaleImage)[1]).saveToFile("build/PrewittY.png");
	djv::Img(djv::operators::simpleGradient(grayScaleImage)[0]).saveToFile("build/SimpleGradientX.png");
	djv::Img(djv::operators::simpleGradient(grayScaleImage)[1]).saveToFile("build/SimpleGradientY.png");

	djv::Img(djv::operators::simpleHessian(grayScaleImage)[0]).saveToFile("build/SimpleHessianXX.png");
	djv::Img(djv::operators::simpleHessian(grayScaleImage)[1]).saveToFile("build/SimpleHessianXY.png");
	djv::Img(djv::operators::simpleHessian(grayScaleImage)[2]).saveToFile("build/SimpleHessianYY.png");

	djv::Img(djv::operators::simpleLaplacian(grayScaleImage)).saveToFile("build/SimpleLaplacian.png");
	

	// Blurs

	
	djv::Img(djv::blur::gaussian(grayScaleImage, 10.f)).saveToFile("build/GaussianBlur.png");
	djv::Img(djv::blur::gaussianBilateral(grayScaleImage, 2.f)).saveToFile("build/GaussianBilateralBlur.png");
	djv::Img(djv::blur::median(grayScaleImage, 2)).saveToFile("build/MedianBlur.png");
	djv::Img(djv::blur::mean(grayScaleImage, 10)).saveToFile("build/MeanBlur.png");
	

	// Image segmentation

	
	result.applySegmentationColor(djv::segmentation::kMeans(image, 5)); result.saveToFile("build/kMeans.png");
	result.applySegmentationColor(djv::segmentation::slic(image, 400)); result.saveToFile("build/SLIC.png");
	

	// Edge detectors

	
	djv::Img(djv::edgeDetectors::marrHildreth(grayScaleImage)).saveToFile("build/MarrHildrethEdgeDetector.png");
	djv::Img(djv::edgeDetectors::canny(grayScaleImage)).saveToFile("build/CannyEdgeDetector.png");
	

	// Corner detectors


	djv::Img(djv::cornerDetectors::harris(grayScaleImage)).saveToFile("build/HarrisCornerDetector.png");


	// Line extractors

	
	{
		scp::Mat<float> edges = djv::edgeDetectors::marrHildreth(grayScaleImage);
		std::vector<djv::Line> lines = djv::lineExtractors::hough(edges);
		djv::Img result = image;
		for (uint64_t i(0); i < lines.size(); i++)
		{
			lines[i].color = { 1, 0, 0, 1 };
			result.draw(lines[i]);
		}
		result.saveToFile("build/houghLineExtractor.png");
	}
	*/

	return 0;
}
