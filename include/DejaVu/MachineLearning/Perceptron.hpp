#pragma once

#include <DejaVu/types.hpp>

namespace djv
{
	namespace perceptron
	{
		class PerceptronBase
		{
			public:

				PerceptronBase(uint64_t inputSize);

				float operator()(const scp::Vec<float>& x) const;

				void goThrough(const scp::Vec<float>& x, float& a, float& z) const;
				void train(const scp::Vec<float>& x, float y, float learningRate = 0.005f);
				void computeCorrection(const scp::Vec<float>& x, float err, float a, float z, float learningRate, float& nextErr, scp::Vec<float>& correction) const;
				void applyCorrection(const scp::Vec<float>& correction);

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

		class BinaryStep : public PerceptronBase
		{
			public:

				BinaryStep(uint64_t inputSize);

				float f(float z) const;
				float df(float a, float z) const;
		};

		class LeakyReLU : public PerceptronBase
		{
			public:

				LeakyReLU(uint64_t inputSize);

				float f(float z) const;
				float df(float a, float z) const;
		};

		class Linear : public PerceptronBase
		{
			public:

				Linear(uint64_t inputSize);

				float f(float z) const;
				float df(float a, float z) const;
		};

		class ReLU : public PerceptronBase
		{
			public:

				ReLU(uint64_t inputSize);

				float f(float z) const;
				float df(float a, float z) const;
		};

		class Sigmoid: public PerceptronBase
		{
			public:

				Sigmoid(uint64_t inputSize);

				float f(float z) const;
				float df(float a, float z) const;
		};

		class Tanh : public PerceptronBase
		{
			public:

				Tanh(uint64_t inputSize);

				float f(float z) const;
				float df(float a, float z) const;
		};
	}
}
