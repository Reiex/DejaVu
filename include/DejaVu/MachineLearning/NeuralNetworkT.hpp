#pragma once

#include <DejaVu/MachineLearning/NeuralNetwork.hpp>

namespace djv
{
	namespace layers
	{
		template<typename TPerceptron>
		Perceptrons<TPerceptron>::Perceptrons(uint64_t inputSize, uint64_t outputSize) :
			LayerBase(inputSize, outputSize),
			_neurons(outputSize, TPerceptron(inputSize))
		{
		}

		template<typename TPerceptron>
		scp::Vec<float> Perceptrons<TPerceptron>::operator()(const scp::Vec<float>& x) const
		{
			scp::Vec<float> result(_outputSize);

			for (uint64_t i(0); i < _outputSize; i++)
				result[i] = _neurons[i](x);
			
			return result;
		}

		template<typename TPerceptron>
		void Perceptrons<TPerceptron>::goThrough(const scp::Vec<float>& x, scp::Vec<float>& a, scp::Vec<float>& z) const
		{
			for (uint64_t i(0); i < _outputSize; i++)
				_neurons[i].goThrough(x, a[i], z[i]);
		}

		template<typename TPerceptron>
		void Perceptrons<TPerceptron>::computeCorrection(const scp::Vec<float>& x, const scp::Vec<float>& err, const scp::Vec<float>& a, const scp::Vec<float>& z, float learningRate, scp::Vec<float>& nextErr, scp::Mat<float>& correction) const
		{
			for (uint64_t i(0); i < _outputSize; i++)
			{
				float computedError;
				_neurons[i].computeCorrection(x, err[i], a[i], z[i], learningRate, computedError, correction[i]);

				const scp::Vec<float>& w = _neurons[i].getWeights();
				for (uint64_t j(0); j < nextErr.n; j++)
					nextErr[j] += computedError * w[j];
			}
		}

		template<typename TPerceptron>
		void Perceptrons<TPerceptron>::applyCorrection(const scp::Mat<float>& correction)
		{
			for (uint64_t i(0); i < _outputSize; i++)
				_neurons[i].applyCorrection(correction[i]);
		}
	}

	template<typename TLayer>
	void NeuralNetwork::setInputLayer(uint64_t inputSize, uint64_t outputSize)
	{
		assert(_layers.size() == 0);
		_layers.push_back(std::unique_ptr<layers::LayerBase>(new TLayer(inputSize, outputSize)));
	}

	template<typename TLayer>
	void NeuralNetwork::appendLayer(uint64_t outputSize)
	{
		assert(_layers.size() != 0);
		_layers.push_back(std::unique_ptr<layers::LayerBase>(new TLayer(_layers.back()->getOutputSize(), outputSize)));
	}
}
