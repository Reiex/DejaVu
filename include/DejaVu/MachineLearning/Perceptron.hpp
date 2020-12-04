#pragma once

#include <DejaVu/types.hpp>

namespace djv
{
	class Perceptron
	{
		public:

			Perceptron(uint64_t inputSize, float learningRate);

			float operator()(const scp::Vec<float>& x) const;

			void train(const scp::Vec<float>& x, float y);

			virtual float f(float z) const = 0;
			virtual float df(float a, float z) const = 0;

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

			float f(float z) const;
			float df(float a, float z) const;
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
