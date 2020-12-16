#include <DejaVu/DejaVu.hpp>

namespace djv
{
	namespace perceptrons
	{
		PerceptronBase::PerceptronBase(uint64_t inputSize) :
			_weights(inputSize),
			_bias(0)
		{
			for (uint64_t i(0); i < inputSize; i++)
				_weights[i] = (static_cast<float>(std::rand()) / RAND_MAX)/10.f - 0.05f;

			_bias = (static_cast<float>(std::rand()) / RAND_MAX)/10.f - 0.05f;
		}

		float PerceptronBase::operator()(const scp::Vec<float>& x) const
		{
			return f(scp::dot(x, _weights) + _bias);
		}

		void PerceptronBase::goThrough(const scp::Vec<float>& x, float& a, float& z) const
		{
			z = scp::dot(x, _weights) + _bias;
			a = f(z);
		}

		void PerceptronBase::train(const scp::Vec<float>& x, float y, float learningRate)
		{
			float a, z;
			goThrough(x, a, z);

			float c = learningRate*(a - y)*df(a, z);
			for (uint64_t i(0); i < _weights.n; i++)
				_weights[i] -= c*x[i];
			_bias -= c;
		}

		void PerceptronBase::computeCorrection(const scp::Vec<float>& x, float err, float a, float z, float learningRate, float& nextErr, scp::Vec<float>& correction) const
		{
			nextErr = df(a, z) * err;
			float c = learningRate * nextErr;
			for (uint64_t i(0); i < _weights.n; i++)
				correction[i] = c*x[i];
			correction[correction.n - 1] = c;
		}

		void PerceptronBase::applyCorrection(const scp::Vec<float>& correction)
		{
			for (uint64_t i(0); i < _weights.n; i++)
				_weights[i] -= correction[i];
			_bias -= correction[correction.n - 1];
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
			PerceptronBase(inputSize)
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
			PerceptronBase(inputSize)
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
			PerceptronBase(inputSize)
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
			PerceptronBase(inputSize)
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
			PerceptronBase(inputSize)
		{
		}

		float Sigmoid::f(float z) const
		{
			return 1.f / (1.f + std::exp(-z));
		}

		float Sigmoid::df(float a, float z) const
		{
			return a*(1.f - a);
		}
	

		Tanh::Tanh(uint64_t inputSize) :
			PerceptronBase(inputSize)
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