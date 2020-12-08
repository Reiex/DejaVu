#pragma once

#include <DejaVu/types.hpp>

namespace djv
{
	class NeuralNetwork
	{
		public:

			template<typename TPerceptron = Perceptron::Sigmoid>
			void setInputLayer(uint64_t inputSize, uint64_t layerSize);
			template<typename TPerceptron = Perceptron::Sigmoid>
			void appendLayer(uint64_t layerSize);

			scp::Vec<float> operator()(const scp::Vec<float>& x) const;

			void train(const scp::Vec<float>& x, const scp::Vec<float>& y, float learningRate = 0.01f);

		private:

			std::vector<std::vector<std::unique_ptr<Perceptron::PerceptronBase>>> _neurons;
	};
}

#include <DejaVu/MachineLearning/NeuralNetworkT.hpp>
