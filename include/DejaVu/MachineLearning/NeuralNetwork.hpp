#pragma once

#include <DejaVu/types.hpp>

namespace djv
{
	namespace layer
	{
		class LayerBase
		{
			public:

				LayerBase(uint64_t inputSize, uint64_t layerSize);

				virtual scp::Vec<float> operator()(const scp::Vec<float>& x) const = 0;

				virtual void goThrough(const scp::Vec<float>& x, scp::Vec<float>& a, scp::Vec<float>& z) const = 0;
				virtual scp::Vec<float> train(const scp::Vec<float>& x, const scp::Vec<float>& weightedErrors, const scp::Vec<float>& a, const scp::Vec<float>& z, float learningRate) = 0;

				virtual uint64_t getInputSize() const;
				virtual uint64_t getLayerSize() const;

			protected:

				uint64_t _inputSize;
				uint64_t _layerSize;
		};

		template<typename TPerceptron = perceptron::Sigmoid>
		class Perceptrons : public LayerBase
		{
			public:

				Perceptrons(uint64_t inputSize, uint64_t layerSize);

				scp::Vec<float> operator()(const scp::Vec<float>& x) const;

				void goThrough(const scp::Vec<float>& x, scp::Vec<float>& a, scp::Vec<float>& z) const;
				scp::Vec<float> train(const scp::Vec<float>& x, const scp::Vec<float>& weightedErrors, const scp::Vec<float>& a, const scp::Vec<float>& z, float learningRate);

			private:

				std::vector<TPerceptron> _neurons;
		};

		class SoftMax : public LayerBase
		{
			public:

				SoftMax(uint64_t inputSize, uint64_t layerSize);

				scp::Vec<float> operator()(const scp::Vec<float>& x) const;

				void goThrough(const scp::Vec<float>& x, scp::Vec<float>& a, scp::Vec<float>& z) const;
				scp::Vec<float> train(const scp::Vec<float>& x, const scp::Vec<float>& weightedErrors, const scp::Vec<float>& a, const scp::Vec<float>& z, float learningRate);
		};
	}

	class NeuralNetwork
	{
		public:

			template<typename TLayer = layer::Perceptrons<>>
			void setInputLayer(uint64_t inputSize, uint64_t layerSize);
			template<typename TLayer = layer::Perceptrons<>>
			void appendLayer(uint64_t layerSize);

			scp::Vec<float> operator()(const scp::Vec<float>& x) const;

			void train(const scp::Vec<float>& x, const scp::Vec<float>& y, float learningRate = 0.01f);

		private:

			std::vector<std::unique_ptr<layer::LayerBase>> _layers;
	};
}

#include <DejaVu/MachineLearning/NeuralNetworkT.hpp>
