#include <DejaVu/DejaVu.hpp>

#include <ctime>

int main()
{
	djv::ImgRGBA imageRGBA("examples/assets/Lena.jpg");
	imageRGBA.saveToFile("build/Original.png");

	djv::ImgGrayscale imageGrayscale("examples/assets/Lena.jpg"), result(imageGrayscale.width(), imageGrayscale.height());
	imageGrayscale.saveToFile("build/GrayScaleOriginal.png");

	const scp::Mat<float>& matGrayscale = imageGrayscale.getData();

	// Operators

	djv::ImgGrayscale(djv::operators::sobel(matGrayscale)[0]).saveToFile("build/SobelX.png");
	djv::ImgGrayscale(djv::operators::sobel(matGrayscale)[1]).saveToFile("build/SobelY.png");
	djv::ImgGrayscale(djv::operators::prewitt(matGrayscale)[0]).saveToFile("build/PrewittX.png");
	djv::ImgGrayscale(djv::operators::prewitt(matGrayscale)[1]).saveToFile("build/PrewittY.png");
	djv::ImgGrayscale(djv::operators::simpleGradient(matGrayscale)[0]).saveToFile("build/SimpleGradientX.png");
	djv::ImgGrayscale(djv::operators::simpleGradient(matGrayscale)[1]).saveToFile("build/SimpleGradientY.png");

	djv::ImgGrayscale(djv::operators::simpleHessian(matGrayscale)[0]).saveToFile("build/SimpleHessianXX.png");
	djv::ImgGrayscale(djv::operators::simpleHessian(matGrayscale)[1]).saveToFile("build/SimpleHessianXY.png");
	djv::ImgGrayscale(djv::operators::simpleHessian(matGrayscale)[2]).saveToFile("build/SimpleHessianYY.png");

	djv::ImgGrayscale(djv::operators::simpleLaplacian(matGrayscale)).saveToFile("build/SimpleLaplacian.png");
	

	// Blurs

	djv::ImgGrayscale(djv::blur::gaussian(matGrayscale, 10.f)).saveToFile("build/GaussianBlur.png");
	djv::ImgGrayscale(djv::blur::gaussianBilateral(matGrayscale, 2.f)).saveToFile("build/GaussianBilateralBlur.png");
	djv::ImgGrayscale(djv::blur::median(matGrayscale, 2)).saveToFile("build/MedianBlur.png");
	djv::ImgGrayscale(djv::blur::mean(matGrayscale, 10)).saveToFile("build/MeanBlur.png");
	

	// Image segmentation

	// result.applySegmentationColor(djv::segmentation::kMeans(image, 5)); result.saveToFile("build/kMeans.png");
	// result.applySegmentationColor(djv::segmentation::slic(image, 400)); result.saveToFile("build/SLIC.png");
	

	// Edge detectors
	
	djv::ImgGrayscale(djv::edgeDetectors::marrHildreth(matGrayscale)).saveToFile("build/MarrHildrethEdgeDetector.png");
	djv::ImgGrayscale(djv::edgeDetectors::canny(matGrayscale)).saveToFile("build/CannyEdgeDetector.png");
	

	// Corner detectors

	djv::ImgGrayscale(djv::cornerDetectors::harris(matGrayscale)).saveToFile("build/HarrisCornerDetector.png");


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
