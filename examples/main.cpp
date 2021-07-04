#include <DejaVu/DejaVu.hpp>

#include <ctime>

int main()
{
	djv::ColorImg colorImage("examples/assets/Lena.jpg");
	colorImage.saveToFile("build/Original.png");

	djv::GrayScaleImg grayScaleImage("examples/assets/Lena.jpg"), result(grayScaleImage.width(), grayScaleImage.height());
	grayScaleImage.saveToFile("build/GrayScaleOriginal.png");

	const scp::Mat<float>& matGrayscale = grayScaleImage.getComponent(0);

	// Operators

	djv::GrayScaleImg(djv::operators::sobel(matGrayscale)[0]).saveToFile("build/SobelX.png");
	djv::GrayScaleImg(djv::operators::sobel(matGrayscale)[1]).saveToFile("build/SobelY.png");
	djv::GrayScaleImg(djv::operators::prewitt(matGrayscale)[0]).saveToFile("build/PrewittX.png");
	djv::GrayScaleImg(djv::operators::prewitt(matGrayscale)[1]).saveToFile("build/PrewittY.png");
	djv::GrayScaleImg(djv::operators::simpleGradient(matGrayscale)[0]).saveToFile("build/SimpleGradientX.png");
	djv::GrayScaleImg(djv::operators::simpleGradient(matGrayscale)[1]).saveToFile("build/SimpleGradientY.png");

	djv::GrayScaleImg(djv::operators::simpleHessian(matGrayscale)[0]).saveToFile("build/SimpleHessianXX.png");
	djv::GrayScaleImg(djv::operators::simpleHessian(matGrayscale)[1]).saveToFile("build/SimpleHessianXY.png");
	djv::GrayScaleImg(djv::operators::simpleHessian(matGrayscale)[2]).saveToFile("build/SimpleHessianYY.png");

	djv::GrayScaleImg(djv::operators::simpleLaplacian(matGrayscale)).saveToFile("build/SimpleLaplacian.png");
	

	// Blurs

	djv::GrayScaleImg(djv::blur::gaussian(matGrayscale, 10.f)).saveToFile("build/GaussianBlur.png");
	djv::GrayScaleImg(djv::blur::gaussianBilateral(matGrayscale, 2.f)).saveToFile("build/GaussianBilateralBlur.png");
	djv::GrayScaleImg(djv::blur::median(matGrayscale, 2)).saveToFile("build/MedianBlur.png");
	djv::GrayScaleImg(djv::blur::mean(matGrayscale, 10)).saveToFile("build/MeanBlur.png");
	

	// Image segmentation

	// result.applySegmentationColor(djv::segmentation::kMeans(image, 5)); result.saveToFile("build/kMeans.png");
	// result.applySegmentationColor(djv::segmentation::slic(image, 400)); result.saveToFile("build/SLIC.png");
	

	// Edge detectors
	
	djv::GrayScaleImg(djv::edgeDetectors::marrHildreth(matGrayscale)).saveToFile("build/MarrHildrethEdgeDetector.png");
	djv::GrayScaleImg(djv::edgeDetectors::canny(matGrayscale)).saveToFile("build/CannyEdgeDetector.png");
	

	// Corner detectors

	djv::GrayScaleImg(djv::cornerDetectors::harris(matGrayscale)).saveToFile("build/HarrisCornerDetector.png");


	// Line extractors

	/*{
		scp::Mat<float> edges = djv::edgeDetectors::marrHildreth(matGrayscale);
		std::vector<djv::Line> lines = djv::lineExtractors::hough(edges);
		djv::Img result = image;
		for (uint64_t i(0); i < lines.size(); i++)
		{
			lines[i].color = { 1, 0, 0, 1 };
			result.draw(lines[i]);
		}
		result.saveToFile("build/houghLineExtractor.png");
	}*/

	return 0;
}
