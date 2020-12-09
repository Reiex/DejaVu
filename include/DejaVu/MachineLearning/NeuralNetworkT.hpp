#pragma once

#include <DejaVu/MachineLearning/NeuralNetwork.hpp>

namespace djv
{
	namespace layer
	{
		template<typename TPerceptron>
		Perceptrons<TPerceptron>::Perceptrons(uint64_t inputSize, uint64_t layerSize) :
			LayerBase(inputSize, layerSize),
			_neurons(layerSize, TPerceptron(inputSize))
		{
		}

		template<typename TPerceptron>
		scp::Vec<float> Perceptrons<TPerceptron>::operator()(const scp::Vec<float>& x) const
		{
			scp::Vec<float> result(_layerSize);

			for (uint64_t i(0); i < _layerSize; i++)
				result[i] = _neurons[i](x);
			
			return result;
		}

		template<typename TPerceptron>
		void Perceptrons<TPerceptron>::goThrough(const scp::Vec<float>& x, scp::Vec<float>& a, scp::Vec<float>& z) const
		{
			for (uint64_t i(0); i < _layerSize; i++)
				_neurons[i].goThrough(x, a[i], z[i]);
		}

		template<typename TPerceptron>
		scp::Vec<float> Perceptrons<TPerceptron>::train(const scp::Vec<float>& x, const scp::Vec<float>& weightedErrors, const scp::Vec<float>& a, const scp::Vec<float>& z, float learningRate)
		{
			scp::Vec<float> newWeightedErrors(_inputSize);

			for (uint64_t i(0); i < _layerSize; i++)
			{
				scp::Vec<float> w = _neurons[i].getWeights();
				float error = _neurons[i].nntrain(x, weightedErrors[i], a[i], z[i], learningRate);
				for (uint64_t j(0); j < w.n; j++)
					newWeightedErrors[j] += w[j] * error;
			}

			return newWeightedErrors;
		}
	}

	template<typename TLayer>
	void NeuralNetwork::setInputLayer(uint64_t inputSize, uint64_t layerSize)
	{
		assert(_layers.size() == 0);
		_layers.push_back(std::make_unique<TLayer>(inputSize, layerSize));
	}

	template<typename TLayer>
	void NeuralNetwork::appendLayer(uint64_t layerSize)
	{
		assert(_layers.size() != 0);
		_layers.push_back(std::make_unique<TLayer>(_layers.back()->getLayerSize(), layerSize));
	}
}
