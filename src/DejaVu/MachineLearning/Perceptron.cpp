#include <DejaVu/DejaVu.hpp>

namespace djv
{
	Perceptron::Perceptron(uint64_t inputSize, float learningRate) :
		_weights(inputSize),
		_bias(0),
		_learningRate(learningRate)
	{
		for (uint64_t i(0); i < inputSize; i++)
			_weights[i] = static_cast<float>(std::rand()) / RAND_MAX - 0.5f;

		_bias = static_cast<float>(std::rand()) / RAND_MAX - 0.5f;
	}

	float Perceptron::operator()(const scp::Vec<float>& x) const
	{
		return f(scp::dot(x, _weights) + _bias);
	}

	void Perceptron::train(const scp::Vec<float>& x, float y)
	{
		float z = scp::dot(x, _weights) + _bias;
		float a = f(z);
		float c = _learningRate*(a - y)*df(a, z);
		for (uint64_t i(0); i < _weights.n; i++)
			_weights[i] -= c*x[i];
		_bias -= c;
	}

	const scp::Vec<float>& Perceptron::getWeights() const
	{
		return _weights;
	}

	void Perceptron::setWeights(const scp::Vec<float>& weights)
	{
		_weights = weights;
	}

	float Perceptron::getBias() const
	{
		return _bias;
	}

	void Perceptron::setBias(float bias)
	{
		_bias = bias;
	}


	SigmoidPerceptron::SigmoidPerceptron(uint64_t inputSize, float learningRate) :
		Perceptron(inputSize, learningRate)
	{
	}

	float SigmoidPerceptron::f(float z) const
	{
		return 1.f / (1.f + std::exp(-z));
	}

	float SigmoidPerceptron::df(float a, float z) const
	{
		return a*(1 - a);
	}
}