#include <DejaVu/DejaVu.hpp>

int main()
{
	djv::Img image("examples/assets/Lena.png");

	// Kernels

	/*
	djv::Img(scp::convolve(image.getComponent(djv::ColorComponent::R), djv::sobelKernel()[0])).saveToFile("build/sobelKernelX.png");
	djv::Img(scp::convolve(image.getComponent(djv::ColorComponent::R), djv::sobelKernel()[1])).saveToFile("build/sobelKernelY.png");
	djv::Img(scp::convolve(image.getComponent(djv::ColorComponent::R), djv::derivativeOfGaussianKernel()[0])*3.f).saveToFile("build/derivativeOfGaussianKernelX.png");
	djv::Img(scp::convolve(image.getComponent(djv::ColorComponent::R), djv::derivativeOfGaussianKernel()[1])*3.f).saveToFile("build/derivativeOfGaussianKernelY.png");

	djv::Img(scp::convolve(image.getComponent(djv::ColorComponent::R), djv::laplacianOfGaussianKernel())*10.f).saveToFile("build/LoGKernel.png");

	djv::Img(scp::convolve(image.getComponent(djv::ColorComponent::R), djv::gaussianKernel(5.f))).saveToFile("build/gaussianKernel.png");
	*/

	// Processings

	/*
	djv::Img(djv::gaussianBlur(image.getComponent(djv::ColorComponent::R), 5.f)).saveToFile("build/gaussianBlur.png");
	djv::Img((djv::gaussianBlur(image.getComponent(djv::ColorComponent::R), 1.f) - djv::gaussianBlur(image.getComponent(djv::ColorComponent::R), 2.f))*10.f).saveToFile("build/DoG.png");
	*/

	// Edge detectors

	djv::Img(djv::marrHildrethEdgeDetector(image.getComponent(djv::ColorComponent::R))).saveToFile("build/marrHildrethEdgeDetector.png");

	return 0;
}
