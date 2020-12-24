#include <DejaVu/DejaVu.hpp>

#include <time.h>

int main()
{
	djv::Img image("examples/assets/Marius.jpg");
	scp::Mat<float> grayScaleImage = image.grayScale();

	// Processings

	/*
	djv::Img(djv::operators::sobel(grayScaleImage)[0]).saveToFile("build/SobelX.png");
	djv::Img(djv::operators::sobel(grayScaleImage)[1]).saveToFile("build/SobelY.png");
	djv::Img(djv::operators::prewitt(grayScaleImage)[0]).saveToFile("build/PrewittX.png");
	djv::Img(djv::operators::prewitt(grayScaleImage)[1]).saveToFile("build/PrewittY.png");
	djv::Img(djv::operators::simpleGradient(grayScaleImage)[0]).saveToFile("build/SimpleGradientX.png");
	djv::Img(djv::operators::simpleGradient(grayScaleImage)[1]).saveToFile("build/SimpleGradientY.png");

	djv::Img(djv::operators::simpleHessian(grayScaleImage)[0]).saveToFile("build/SimpleHessianXX.png");
	djv::Img(djv::operators::simpleHessian(grayScaleImage)[1]).saveToFile("build/SimpleHessianXY.png");
	djv::Img(djv::operators::simpleHessian(grayScaleImage)[2]).saveToFile("build/SimpleHessianYY.png");

	djv::Img(djv::operators::simpleLaplacian(grayScaleImage)).saveToFile("build/SimpleLaplacian.png");

	djv::Img(djv::operators::gaussianBlur(grayScaleImage, 10.f)).saveToFile("build/GaussianBlur.png");
	*/

	// Image segmentation

	/*
	{
		djv::segmentation::ImageSegmentation seg = djv::segmentation::kMeans(image, 5);
		djv::Img segmented(image.width(), image.height());
		for (uint64_t i(0); i < image.width(); i++)
			for (uint64_t j(0); j < image.height(); j++)
				segmented.setPixel(i, j, seg.groupColors[seg.groups[i][j]]);
		segmented.saveToFile("build/kMeans.png");
	}
	*/

	{
		djv::segmentation::ImageSegmentation seg = djv::segmentation::slic(image, 400);
		djv::Img segmented(image.width(), image.height());
		for (uint64_t i(0); i < image.width(); i++)
			for (uint64_t j(0); j < image.height(); j++)
				segmented.setPixel(i, j, seg.groupColors[seg.groups[i][j]]);
		segmented.saveToFile("build/SLIC.png");
	}


	// Edge detectors

	/*
	djv::Img(djv::edgeDetectors::marrHildreth(grayScaleImage)).saveToFile("build/marrHildrethEdgeDetector.png");
	djv::Img(djv::edgeDetectors::canny(grayScaleImage)).saveToFile("build/cannyEdgeDetector.png");
	*/

	// Line extractors

	/*
	{
		scp::Mat<float> edges = djv::edgeDetectors::marrHildreth(grayScaleImage);
		std::vector<djv::Line> lines = djv::lineExtractors::hough(edges, 0.4);
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

	/*
	scp::Mat<float> face(28, 28);
	std::vector<std::vector<scp::Vec<float>>> training(2), testing(2);
	for (uint64_t i(0); i < 4500; i++)
	{
		training[0].push_back(scp::Vec<float>(784));
		face = djv::Img("datasets/FDDB/positive/" + std::to_string(i) + ".png").grayScale();
		for (uint64_t j(0); j < 28; j++)
			for (uint64_t k(0); k < 28; k++)
				training[0][i][j*28 + k] = face[j][k];

		training[1].push_back(scp::Vec<float>(784));
		face = djv::Img("datasets/FDDB/negative/" + std::to_string(i) + ".png").grayScale();
		for (uint64_t j(0); j < 28; j++)
			for (uint64_t k(0); k < 28; k++)
				training[1][i][j*28 + k] = face[j][k];
	}

	for (uint64_t i(4500); i < 5171; i++)
	{
		testing[0].push_back(scp::Vec<float>(784));
		face = djv::Img("datasets/FDDB/positive/" + std::to_string(i) + ".png").grayScale();
		for (uint64_t j(0); j < 28; j++)
			for (uint64_t k(0); k < 28; k++)
				testing[0][i - 4500][j*28 + k] = face[j][k];

		testing[1].push_back(scp::Vec<float>(784));
		face = djv::Img("datasets/FDDB/negative/" + std::to_string(i) + ".png").grayScale();
		for (uint64_t j(0); j < 28; j++)
			for (uint64_t k(0); k < 28; k++)
				testing[1][i - 4500][j*28 + k] = face[j][k];
	}


	djv::NeuralNetwork net;
	net.setInputLayer(784, 8);
	net.appendLayer(1);

	for (uint64_t i(0); true; i++)
	{
		std::vector<scp::Vec<float>> x, y;
		for (uint64_t j(0); j < 10; j++)
		{
			uint64_t n = std::rand() % 2;
			uint64_t k = std::rand() % training[0].size();
			x.push_back(training[n][k]);

			y.push_back(scp::Vec<float>(1, static_cast<float>(n)));
		}

		net.batchTrain(x, y, 0.1f);

		if (i % 5000 == 0)
		{
			float count[2] = { 0, 0 };

			#pragma omp parallel for shared(count, testing)
			for (int64_t m = 0; m < 2; m++)
			{
				for (int64_t l = 0; l < 671; l++)
				{
					scp::Vec<float> prediction = net(testing[m][l]);
					if (prediction[0] > 0.5 && m || prediction[0] < 0.5 && !m)
						#pragma omp atomic
						count[m]++;
				}
			}

			std::cout << count[0] << " " << count[1] << " " << (count[0] + count[1]) / 1342.f << std::endl;
		}
	}
	*/

	return 0;
}
