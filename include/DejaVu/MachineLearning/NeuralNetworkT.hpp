#pragma once

#include <DejaVu/MachineLearning/NeuralNetwork.hpp>

namespace djv
{
	template<typename TPerceptron>
	void NeuralNetwork::setInputLayer(uint64_t inputSize, uint64_t layerSize)
	{
		assert(_neurons.size() == 0);

		_neurons.push_back(std::vector<std::unique_ptr<Perceptron>>(layerSize));
		for (uint64_t i(0); i < layerSize; i++)
			_neurons[0][i] = std::unique_ptr<Perceptron>(new TPerceptron(inputSize));
	}

	template<typename TPerceptron>
	void NeuralNetwork::appendLayer(uint64_t layerSize)
	{
		assert(_neurons.size() != 0);

		_neurons.push_back(std::vector<std::unique_ptr<Perceptron>>(layerSize));
		for (uint64_t i(0); i < layerSize; i++)
			_neurons[_neurons.size() - 1][i] = std::unique_ptr<Perceptron>(new TPerceptron(_neurons[_neurons.size() - 2].size()));
	}
}
