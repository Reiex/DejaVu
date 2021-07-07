#include <DejaVu/DejaVu.hpp>

#include <ctime>

int main()
{
	djv::RGBAImg rgbaImage("examples/assets/Lena.jpg");
	rgbaImage.saveToFile("build/Original.png");

	djv::GrayScaleImg grayScaleImage("examples/assets/Lena.jpg");
	grayScaleImage.saveToFile("build/GrayScaleOriginal.png");

	// Operators

	{
		std::array<djv::Img<djv::RGBAPixel>, 2> result = djv::operators::sobel(rgbaImage);
		result[0].setComponent(3, { rgbaImage.width(), rgbaImage.height(), 1.f });
		result[0].normalize();
		result[0].saveToFile("build/SobelX.png");
		result[1].setComponent(3, { rgbaImage.width(), rgbaImage.height(), 1.f });
		result[1].normalize();
		result[1].saveToFile("build/SobelY.png");
	}

	{
		std::array<djv::Img<djv::RGBAPixel>, 2> result = djv::operators::prewitt(rgbaImage);
		result[0].setComponent(3, { rgbaImage.width(), rgbaImage.height(), 1.f });
		result[0].normalize();
		result[0].saveToFile("build/PrewittX.png");
		result[1].setComponent(3, { rgbaImage.width(), rgbaImage.height(), 1.f });
		result[1].normalize();
		result[1].saveToFile("build/PrewittY.png");
	}

	{
		std::array<djv::Img<djv::RGBAPixel>, 2> result = djv::operators::simpleGradient(rgbaImage);
		result[0].setComponent(3, { rgbaImage.width(), rgbaImage.height(), 1.f });
		result[0].normalize();
		result[0].saveToFile("build/SimpleGradientX.png");
		result[1].setComponent(3, { rgbaImage.width(), rgbaImage.height(), 1.f });
		result[1].normalize();
		result[1].saveToFile("build/SimpleGradientY.png");
	}

	{
		std::array<djv::Img<djv::RGBAPixel>, 3> result = djv::operators::simpleHessian(rgbaImage);
		result[0].setComponent(3, { rgbaImage.width(), rgbaImage.height(), 1.f });
		result[0].normalize();
		result[0].saveToFile("build/SimpleHessianXX.png");
		result[1].setComponent(3, { rgbaImage.width(), rgbaImage.height(), 1.f });
		result[1].normalize();
		result[1].saveToFile("build/SimpleHessianXY.png");
		result[2].setComponent(3, { rgbaImage.width(), rgbaImage.height(), 1.f });
		result[2].normalize();
		result[2].saveToFile("build/SimpleHessianYY.png");
	}

	{
		djv::RGBAImg result = djv::operators::simpleLaplacian(rgbaImage);
		result.setComponent(3, { rgbaImage.width(), rgbaImage.height(), 1.f });
		result.normalize();
		result.saveToFile("build/SimpleLaplacian.png");
	}

	// Blurs

	djv::blur::gaussian(rgbaImage, 10.f).saveToFile("build/GaussianBlur.png");
	djv::blur::gaussianBilateral(rgbaImage, 2.f).saveToFile("build/GaussianBilateralBlur.png");
	djv::blur::median(rgbaImage, 2).saveToFile("build/MedianBlur.png");
	djv::blur::mean(rgbaImage, 10).saveToFile("build/MeanBlur.png");
	

	// Image segmentation

	// result.applySegmentationColor(djv::segmentation::kMeans(image, 5)); result.saveToFile("build/kMeans.png");
	// result.applySegmentationColor(djv::segmentation::slic(image, 400)); result.saveToFile("build/SLIC.png");
	

	// Edge detectors
	
	{
		djv::RGBAImg result = djv::edgeDetectors::marrHildreth(rgbaImage);
		result.setComponent(3, { rgbaImage.width(), rgbaImage.height(), 1.f });
		result.normalize();
		result.saveToFile("build/MarrHildrethEdgeDetector.png");
	}

	{
		djv::RGBAImg result = djv::edgeDetectors::canny(rgbaImage);
		result.setComponent(3, { rgbaImage.width(), rgbaImage.height(), 1.f });
		result.normalize();
		result.saveToFile("build/CannyEdgeDetector.png");
	}
	

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
