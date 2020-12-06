#pragma once

#include <DejaVu/types.hpp>

namespace djv
{
	namespace Perceptron
	{
		class Perceptron
		{
			public:

				Perceptron(uint64_t inputSize);

				float operator()(const scp::Vec<float>& x) const;

				void train(const scp::Vec<float>& x, float y, float learningRate = 0.005f);
				float nntrain(const scp::Vec<float>& x, float weightedDelta, float a, float z, float learningRate);

				virtual float f(float z) const = 0;
				virtual float df(float a, float z) const = 0;

				const scp::Vec<float>& getWeights() const;
				void setWeights(const scp::Vec<float>& weights);
				float getBias() const;
				void setBias(float bias);

			protected:

				scp::Vec<float> _weights;
				float _bias;
		};

		class BinaryStep : public Perceptron
		{
			public:

				BinaryStep(uint64_t inputSize);

				float f(float z) const;
				float df(float a, float z) const;
		};

		class LeakyReLU : public Perceptron
		{
			public:

				LeakyReLU(uint64_t inputSize);

				float f(float z) const;
				float df(float a, float z) const;
		};

		class Linear : public Perceptron
		{
			public:

				Linear(uint64_t inputSize);

				float f(float z) const;
				float df(float a, float z) const;
		};

		class ReLU : public Perceptron
		{
			public:

				ReLU(uint64_t inputSize);

				float f(float z) const;
				float df(float a, float z) const;
		};

		class Sigmoid: public Perceptron
		{
			public:

				Sigmoid(uint64_t inputSize);

				float f(float z) const;
				float df(float a, float z) const;
		};

		class Tanh : public Perceptron
		{
			public:

				Tanh(uint64_t inputSize);

				float f(float z) const;
				float df(float a, float z) const;
		};
	}
}
