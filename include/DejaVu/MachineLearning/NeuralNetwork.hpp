#pragma once

#include <DejaVu/types.hpp>

namespace djv
{
	class NeuralNetwork
	{
		public:

			template<typename TPerceptron = SigmoidPerceptron>
			void setInputLayer(uint64_t inputSize, uint64_t layerSize);
			template<typename TPerceptron = SigmoidPerceptron>
			void appendLayer(uint64_t layerSize);

			scp::Vec<float> operator()(const scp::Vec<float>& x) const;

			void train(const scp::Vec<float>& x, const scp::Vec<float>& y, float learningRate = 0.01f);

		private:

			std::vector<std::vector<std::unique_ptr<Perceptron>>> _neurons;
	};
}

#include <DejaVu/MachineLearning/NeuralNetworkT.hpp>
