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
	djv::Img(djv::edgeDetector::marrHildreth((image.getComponent(djv::ColorComponent::R) + image.getComponent(djv::ColorComponent::G) + image.getComponent(djv::ColorComponent::B)) / 3.f)).saveToFile("build/marrHildrethEdgeDetector.png");
	djv::Img(djv::edgeDetector::canny((image.getComponent(djv::ColorComponent::R) + image.getComponent(djv::ColorComponent::G) + image.getComponent(djv::ColorComponent::B)) / 3.f)).saveToFile("build/cannyEdgeDetector.png");
	*/

	// Line extractors

	/*
	{
		scp::Mat<float> edges = djv::edgeDetector::marrHildreth((image.getComponent(djv::ColorComponent::R) + image.getComponent(djv::ColorComponent::G) + image.getComponent(djv::ColorComponent::B)) / 3.f);
		std::vector<djv::Line> lines = djv::lineExtractor::hough(edges, 0.6f, 0.1f, 5);
		djv::Img result = image;
		for (uint64_t i(0); i < lines.size(); i++)
		{
			lines[i].color = { 1, 0, 0, 1 };
			result.draw(lines[i]);
		}
		result.saveToFile("build/houghLineExtractor.png");
	}
	*/

	// Neural network

	djv::SigmoidPerceptron p(784);

	for (uint64_t i(0); i < 10000; i++)
	{
		uint64_t j = std::rand() % 5000;
		uint64_t n = std::rand() % 10; while (n == 5) n = std::rand() % 10;

		djv::Img zeroImg("examples/assets/MNIST/training/5/" + std::to_string(j) + ".png");
		djv::Img oneImg("examples/assets/MNIST/training/" + std::to_string(n) + "/" + std::to_string(j) + ".png");

		scp::Vec<float> zero(784), one(784);
		for (uint64_t k(0); k < 28; k++)
		{
			for (uint64_t l(0); l < 28; l++)
			{
				zero[28*k + l] = zeroImg.getComponent(djv::ColorComponent::R)[k][l];
				one[28*k + l] = oneImg.getComponent(djv::ColorComponent::R)[k][l];
			}
		}

		p.train(zero, 0);
		p.train(one, 1);
	}
	
	float count(0);
	for (uint64_t i(0); i < 500; i++)
	{
		uint64_t n = std::rand() % 10; while (n == 5) n = std::rand() % 10;
		djv::Img zeroImg("examples/assets/MNIST/testing/5/" + std::to_string(i) + ".png");
		djv::Img oneImg("examples/assets/MNIST/testing/" + std::to_string(n) + "/" + std::to_string(i) + ".png");

		scp::Vec<float> zero(784), one(784);
		for (uint64_t k(0); k < 28; k++)
		{
			for (uint64_t l(0); l < 28; l++)
			{
				zero[28 * k + l] = zeroImg.getComponent(djv::ColorComponent::R)[k][l];
				one[28 * k + l] = oneImg.getComponent(djv::ColorComponent::R)[k][l];
			}
		}

		if (p(zero) < 0.5)
			count++;
		if (p(one) > 0.5)
			count++;
	}

	std::cout << count / 1000.f << std::endl;

	system("pause");

	return 0;
}
