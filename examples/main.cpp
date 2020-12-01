#include <DejaVu/DejaVu.hpp>

int main()
{

	djv::Img image("examples/assets/Cathedrale.jpg");

	// Kernels

	/*
	djv::Img(scp::convolve(image.getComponent(djv::ColorComponent::R), djv::kernel::sobel()[0])).saveToFile("build/sobelKernelX.png");
	djv::Img(scp::convolve(image.getComponent(djv::ColorComponent::R), djv::kernel::sobel()[1])).saveToFile("build/sobelKernelY.png");
	djv::Img(scp::convolve(image.getComponent(djv::ColorComponent::R), djv::kernel::prewitt()[0])).saveToFile("build/prewittKernelX.png");
	djv::Img(scp::convolve(image.getComponent(djv::ColorComponent::R), djv::kernel::prewitt()[1])).saveToFile("build/prewittKernelY.png");
	djv::Img(scp::convolve(image.getComponent(djv::ColorComponent::R), djv::kernel::derivativeOfGaussian()[0])*3.f).saveToFile("build/derivativeOfGaussianKernelX.png");
	djv::Img(scp::convolve(image.getComponent(djv::ColorComponent::R), djv::kernel::derivativeOfGaussian()[1])*3.f).saveToFile("build/derivativeOfGaussianKernelY.png");

	djv::Img(scp::convolve(image.getComponent(djv::ColorComponent::R), djv::kernel::laplacianOfGaussian())*10.f).saveToFile("build/LoGKernel.png");

	djv::Img(scp::convolve(image.getComponent(djv::ColorComponent::R), djv::kernel::gaussian(5.f))).saveToFile("build/gaussianKernel.png");
	*/

	// Processings

	/*
	djv::Img(djv::gaussianBlur(image.getComponent(djv::ColorComponent::R), 5.f)).saveToFile("build/gaussianBlur.png");
	djv::Img((djv::gaussianBlur(image.getComponent(djv::ColorComponent::R), 1.f) - djv::gaussianBlur(image.getComponent(djv::ColorComponent::R), 2.f))*10.f).saveToFile("build/DoG.png");
	*/

	// Edge detectors

	/*
	djv::Img(djv::edgeDetector::sobel((image.getComponent(djv::ColorComponent::R) + image.getComponent(djv::ColorComponent::G) + image.getComponent(djv::ColorComponent::B)) / 3.f)).saveToFile("build/sobelEdgeDetector.png");
	djv::Img(djv::edgeDetector::prewitt((image.getComponent(djv::ColorComponent::R) + image.getComponent(djv::ColorComponent::G) + image.getComponent(djv::ColorComponent::B)) / 3.f)).saveToFile("build/prewittEdgeDetector.png");
	djv::Img(djv::edgeDetector::marrHildreth((image.getComponent(djv::ColorComponent::R) + image.getComponent(djv::ColorComponent::G) + image.getComponent(djv::ColorComponent::B))/3.f)).saveToFile("build/marrHildrethEdgeDetector.png");
	*/
	djv::Img(djv::edgeDetector::canny((image.getComponent(djv::ColorComponent::R) + image.getComponent(djv::ColorComponent::G) + image.getComponent(djv::ColorComponent::B)) / 3.f)).saveToFile("build/cannyEdgeDetector.png");
	
	// Line extractors

	/*
	{
		scp::Mat<float> edges = djv::edgeDetector::marrHildreth((image.getComponent(djv::ColorComponent::R) + image.getComponent(djv::ColorComponent::G) + image.getComponent(djv::ColorComponent::B)) / 3.f);
		std::vector<djv::Line> lines = djv::lineExtractor::hough(edges, 0.7f, 0.1f, 5);
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
