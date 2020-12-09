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

	std::vector<std::vector<scp::Vec<float>>> mnist_training(10), mnist_testing(10);
	for (uint64_t n(0); n < 10; n++)
	{
		mnist_training[n].resize(5000, scp::Vec<float>(784));
		for (uint64_t i(0); i < mnist_training[n].size(); i++)
		{
			djv::Img nImg("examples/assets/MNIST/training/" + std::to_string(n) + "/" + std::to_string(i) + ".png");
			for (uint64_t k(0); k < 28; k++)
				for (uint64_t l(0); l < 28; l++)
					mnist_training[n][i][28*k + l] = nImg(k, l, 0);
		}

		mnist_testing[n].resize(500, scp::Vec<float>(784));
		for (uint64_t i(0); i < mnist_testing[n].size(); i++)
		{
			djv::Img nImg("examples/assets/MNIST/testing/" + std::to_string(n) + "/" + std::to_string(i) + ".png");
			for (uint64_t k(0); k < 28; k++)
				for (uint64_t l(0); l < 28; l++)
					mnist_testing[n][i][28*k + l] = nImg(k, l, 0);
		}
	}

	djv::NeuralNetwork net;
	net.setInputLayer(784, 64);
	net.appendLayer(32);
	net.appendLayer(10);

	for (uint64_t i(0); true; i++)
	{
		uint64_t j = std::rand() % mnist_training.size();
		uint64_t n = std::rand() % 10;

		scp::Vec<float> y(10);
		y[n] = 1.f;

		net.train(mnist_training[n][j], y);

		if (i % 1000 == 0)
		{
			float count(0);
			for (uint64_t k(0); k < 1000; k++)
			{
				uint64_t l = std::rand() % 500;
				uint64_t m = std::rand() % 10;
				scp::Vec<float> prediction = net(mnist_testing[m][l]);
				if (scp::maxElement(prediction) == prediction[m])
					count++;
			}

			std::cout << count / 1000.f << std::endl;
		}
	}

	system("pause");

	return 0;
}
