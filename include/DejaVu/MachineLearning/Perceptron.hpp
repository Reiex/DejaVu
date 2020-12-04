#pragma once

#include <DejaVu/types.hpp>

namespace djv
{
	class Perceptron
	{
		public:

			Perceptron(uint64_t inputSize, float learningRate);

			virtual float operator()(const scp::Vec<float>& x) const = 0;

			virtual void train(const scp::Vec<float>& x, float y) = 0;

			const scp::Vec<float>& getWeights() const;
			void setWeights(const scp::Vec<float>& weights);
			float getBias() const;
			void setBias(float bias);

		protected:

			scp::Vec<float> _weights;
			float _bias;

			float _learningRate;
	};

	class SigmoidPerceptron: public Perceptron
	{
		public:

			SigmoidPerceptron(uint64_t inputSize, float learningRate = 0.005f);
			float operator()(const scp::Vec<float>& x) const;
			void train(const scp::Vec<float>& x, float y);
	};

	/*
	
	Allow for generalization of sigmoid (1/(1+exp(-lambda*x)))

	Future perceptrons:
		- CustomPerceptron
		- ReLU
		- Leaky ReLU
		- Linear
		- Smooth max
		- Softmax
		- tanh

	*/
}
