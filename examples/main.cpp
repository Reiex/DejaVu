#include <DejaVu/DejaVu.hpp>

#include <time.h>

int main()
{
	djv::Img image("examples/assets/Cathedrale.jpg");
	scp::Mat<float> grayScaleImage = image.grayScale();

	// Processings

	
	djv::Img(djv::operators::sobel(grayScaleImage)[0]).saveToFile("build/SobelX.png");
	djv::Img(djv::operators::sobel(grayScaleImage)[1]).saveToFile("build/SobelY.png");
	djv::Img(djv::operators::prewitt(grayScaleImage)[0]).saveToFile("build/PrewittX.png");
	djv::Img(djv::operators::prewitt(grayScaleImage)[1]).saveToFile("build/PrewittY.png");
	djv::Img(djv::operators::derivativeOfGaussian(grayScaleImage)[0]).saveToFile("build/DerivativeOfGaussianX.png");
	djv::Img(djv::operators::derivativeOfGaussian(grayScaleImage)[1]).saveToFile("build/DerivativeOfGaussianY.png");

	djv::Img(djv::operators::laplacianOfGaussian(grayScaleImage)).saveToFile("build/LaplacianOfGaussian.png");

	djv::Img(djv::operators::gaussianBlur(grayScaleImage, 10.f)).saveToFile("build/GaussianBlur.png");
	

	// Edge detectors


	djv::Img(djv::edgeDetector::sobel(grayScaleImage)).saveToFile("build/sobelEdgeDetector.png");
	djv::Img(djv::edgeDetector::prewitt(grayScaleImage)).saveToFile("build/prewittEdgeDetector.png");
	djv::Img(djv::edgeDetector::marrHildreth(grayScaleImage)).saveToFile("build/marrHildrethEdgeDetector.png");
	djv::Img(djv::edgeDetector::canny(grayScaleImage)).saveToFile("build/cannyEdgeDetector.png");


	// Line extractors

	{
		scp::Mat<float> edges = djv::edgeDetector::marrHildreth(grayScaleImage);
		std::vector<djv::Line> lines = djv::lineExtractor::hough(edges, 0.4);
		djv::Img result = image;
		for (uint64_t i(0); i < lines.size(); i++)
		{
			lines[i].color = { 1, 0, 0, 1 };
			result.draw(lines[i]);
		}
		result.saveToFile("build/houghLineExtractor.png");
	}

	// Neural network

	
	time_t t(time(nullptr));
	std::srand(t);
	std::cout << "Seed: " << t << std::endl;

	std::vector<std::vector<scp::Vec<float>>> mnist_training(10), mnist_testing(10);
	for (uint64_t n(0); n < 10; n++)
	{
		std::cout << "Loading number " << n << " of MNIST." << std::endl;
		mnist_training[n].resize(5000, scp::Vec<float>(784));
		int64_t i, k, l;
		djv::Img nImg;

		#pragma omp parallel for private(i, k, l, nImg) shared(mnist_training)
		for (i = 0; i < mnist_training[n].size(); i++)
		{
			nImg = djv::Img("examples/assets/MNIST/training/" + std::to_string(n) + "/" + std::to_string(i) + ".png");
			for (k = 0; k < 28; k++)
				for (l = 0; l < 28; l++)
					mnist_training[n][i][28*k + l] = nImg(k, l, 0);
		}

		mnist_testing[n].resize(500, scp::Vec<float>(784));

		#pragma omp parallel for private(i, k, l, nImg) shared(mnist_testing)
		for (int64_t i = 0; i < mnist_testing[n].size(); i++)
		{
			nImg = djv::Img("examples/assets/MNIST/testing/" + std::to_string(n) + "/" + std::to_string(i) + ".png");
			for (k = 0; k < 28; k++)
				for (l = 0; l < 28; l++)
					mnist_testing[n][i][28*k + l] = nImg(k, l, 0);
		}
	}

	djv::NeuralNetwork net;
	net.setInputLayer(784, 10);

	for (uint64_t i(0); true; i++)
	{
		std::vector<scp::Vec<float>> x, y;
		for (uint64_t j(0); j < 10; j++)
		{
			uint64_t n = std::rand() % 10;
			uint64_t k = std::rand() % mnist_training.size();
			x.push_back(mnist_training[n][k]);

			y.push_back(scp::Vec<float>(10));
			y.back()[n] = 1.f;
		}

		net.batchTrain(x, y, 0.01f);

		if (i % 1000 == 0)
		{
			float count[10];
			for (uint64_t m(0); m < 10; m++)
				count[m] = 0;

			#pragma omp parallel for shared(count, mnist_testing)
			for (int64_t m = 0; m < 10; m++)
			{
				for (int64_t l = 0; l < 500; l++)
				{
					scp::Vec<float> prediction = net(mnist_testing[m][l]);
					if (scp::maxElement(prediction) == prediction[m])
						#pragma omp atomic
						count[m]++;
				}
			}

			for (uint64_t m(0); m < 10; m++)
				std::cout << count[m] << " ";
			std::cout << std::endl;
		}
	}

	return 0;
}
