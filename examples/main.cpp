#include <DejaVu/DejaVu.hpp>

#include <ctime>

int main()
{
	djv::RGBAImg rgbaImage("examples/assets/Lena.jpg");
	rgbaImage.saveToFile("build/Original.png");

	djv::GrayScaleImg grayScaleImage("examples/assets/Lena.jpg"), result(grayScaleImage.width(), grayScaleImage.height());
	grayScaleImage.saveToFile("build/GrayScaleOriginal.png");

	// Operators

	djv::operators::sobel(rgbaImage)[0].saveToFile("build/SobelX.png");
	djv::operators::sobel(rgbaImage)[1].saveToFile("build/SobelY.png");
	djv::operators::prewitt(rgbaImage)[0].saveToFile("build/PrewittX.png");
	djv::operators::prewitt(rgbaImage)[1].saveToFile("build/PrewittY.png");
	djv::operators::simpleGradient(rgbaImage)[0].saveToFile("build/SimpleGradientX.png");
	djv::operators::simpleGradient(rgbaImage)[1].saveToFile("build/SimpleGradientY.png");

	djv::operators::simpleHessian(rgbaImage)[0].saveToFile("build/SimpleHessianXX.png");
	djv::operators::simpleHessian(rgbaImage)[1].saveToFile("build/SimpleHessianXY.png");
	djv::operators::simpleHessian(rgbaImage)[2].saveToFile("build/SimpleHessianYY.png");

	djv::operators::simpleLaplacian(rgbaImage).saveToFile("build/SimpleLaplacian.png");
	

	// Blurs

	djv::blur::gaussian(rgbaImage, 10.f).saveToFile("build/GaussianBlur.png");
	djv::blur::gaussianBilateral(rgbaImage, 2.f).saveToFile("build/GaussianBilateralBlur.png");
	djv::blur::median(rgbaImage, 2).saveToFile("build/MedianBlur.png");
	djv::blur::mean(rgbaImage, 10).saveToFile("build/MeanBlur.png");
	

	// Image segmentation

	// result.applySegmentationColor(djv::segmentation::kMeans(image, 5)); result.saveToFile("build/kMeans.png");
	// result.applySegmentationColor(djv::segmentation::slic(image, 400)); result.saveToFile("build/SLIC.png");
	

	// Edge detectors
	
	djv::edgeDetectors::marrHildreth(rgbaImage).saveToFile("build/MarrHildrethEdgeDetector.png");
	djv::edgeDetectors::canny(rgbaImage).saveToFile("build/CannyEdgeDetector.png");
	

	// Corner detectors

	// djv::cornerDetectors::harris(rgbaImage).saveToFile("build/HarrisCornerDetector.png");


	// Line extractors

	/*{
		scp::Mat<float> edges = djv::edgeDetectors::marrHildreth(rgbaImage);
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
