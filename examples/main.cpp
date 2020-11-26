#include <DejaVu/DejaVu.hpp>

int main()
{
	djv::Img image("examples/assets/Lena.png");

	// Kernels

	// djv::Img(scp::convolve(image.getComponent(djv::ColorComponent::R), djv::sobelKernel()[0])).saveToFile("build/sobelX.png");
	// djv::Img(scp::convolve(image.getComponent(djv::ColorComponent::R), djv::sobelKernel()[1])).saveToFile("build/sobelY.png");
	// djv::Img(scp::convolve(image.getComponent(djv::ColorComponent::R), djv::derivativeOfGaussianKernel()[0])*3.f).saveToFile("build/derivativeOfGaussianX.png");
	// djv::Img(scp::convolve(image.getComponent(djv::ColorComponent::R), djv::derivativeOfGaussianKernel()[1])*3.f).saveToFile("build/derivativeOfGaussianY.png");

	// djv::Img(scp::convolve(image.getComponent(djv::ColorComponent::R), djv::laplacianOfGaussianKernel())*10.f).saveToFile("build/LoG.png");
	// djv::Img((scp::convolve(image.getComponent(djv::ColorComponent::R), djv::gaussianKernel()) - scp::convolve(image.getComponent(djv::ColorComponent::R), djv::gaussianKernel(2.f)))*10.f).saveToFile("build/DoG.png");

	// djv::Img(scp::convolve(image.getComponent(djv::ColorComponent::R), djv::gaussianKernel(20.f))).saveToFile("build/gaussian.png");

	// Processings

	// djv::Img(djv::gaussianBlur(image.getComponent(djv::ColorComponent::R), 20.f)).saveToFile("build/gaussianBlur.png");

	return 0;
}
