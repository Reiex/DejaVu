#include <DejaVu/DejaVu.hpp>

namespace djv
{
	namespace Perceptron
	{
		PerceptronBase::PerceptronBase(uint64_t inputSize) :
			_weights(inputSize),
			_bias(0)
		{
			for (uint64_t i(0); i < inputSize; i++)
				_weights[i] = static_cast<float>(std::rand()) / RAND_MAX - 0.5f;

			_bias = static_cast<float>(std::rand()) / RAND_MAX - 0.5f;
		}

		float PerceptronBase::operator()(const scp::Vec<float>& x) const
		{
			return f(scp::dot(x, _weights) + _bias);
		}

		void PerceptronBase::train(const scp::Vec<float>& x, float y, float learningRate)
		{
			float z = scp::dot(x, _weights) + _bias;
			float a = f(z);
			float c = learningRate*(a - y)*df(a, z);
			for (uint64_t i(0); i < _weights.n; i++)
				_weights[i] -= c*x[i];
			_bias -= c;
		}

		float PerceptronBase::nntrain(const scp::Vec<float>& x, float weightedError, float a, float z, float learningRate)
		{
			weightedError *= df(a, z);
			float c = learningRate * weightedError;
			for (uint64_t i(0); i < _weights.n; i++)
				_weights[i] -= c * x[i];
			_bias -= c;

			return weightedError;
		}

		const scp::Vec<float>& PerceptronBase::getWeights() const
		{
			return _weights;
		}

		void PerceptronBase::setWeights(const scp::Vec<float>& weights)
		{
			_weights = weights;
		}

		float PerceptronBase::getBias() const
		{
			return _bias;
		}

		void PerceptronBase::setBias(float bias)
		{
			_bias = bias;
		}


		BinaryStep::BinaryStep(uint64_t inputSize) :
			Perceptron(inputSize)
		{
		}

		float BinaryStep::f(float z) const
		{
			return z >= 0.f;
		}

		float BinaryStep::df(float a, float z) const
		{
			return 0.f;
		}


		LeakyReLU::LeakyReLU(uint64_t inputSize) :
			Perceptron(inputSize)
		{
		}

		float LeakyReLU::f(float z) const
		{
			return z * (1.f - 0.99f * (z < 0.f));
		}

		float LeakyReLU::df(float a, float z) const
		{
			return 1.f - 0.99f * (z < 0.f);
		}


		Linear::Linear(uint64_t inputSize) :
			Perceptron(inputSize)
		{
		}

		float Linear::f(float z) const
		{
			return z;
		}

		float Linear::df(float a, float z) const
		{
			return 1.f;
		}


		ReLU::ReLU(uint64_t inputSize) :
			Perceptron(inputSize)
		{
		}

		float ReLU::f(float z) const
		{
			return z * (z > 0.f);
		}

		float ReLU::df(float a, float z) const
		{
			return z > 0.f;
		}


		Sigmoid::Sigmoid(uint64_t inputSize) :
			Perceptron(inputSize)
		{
		}

		float Sigmoid::f(float z) const
		{
			return 1.f / (1.f + std::exp(-z));
		}

		float Sigmoid::df(float a, float z) const
		{
			return a*(1 - a);
		}
	

		Tanh::Tanh(uint64_t inputSize) :
			Perceptron(inputSize)
		{
		}

		float Tanh::f(float z) const
		{
			return std::tanh(z);
		}

		float Tanh::df(float a, float z) const
		{
			return 1.f - a * a;
		}
	}
}