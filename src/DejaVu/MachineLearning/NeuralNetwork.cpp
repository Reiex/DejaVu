#include <DejaVu/DejaVu.hpp>

namespace djv
{
	namespace layer
	{
		LayerBase::LayerBase(uint64_t inputSize, uint64_t outputSize) :
			_inputSize(inputSize),
			_outputSize(outputSize)
		{
		}

		uint64_t LayerBase::getInputSize() const
		{
			return _inputSize;
		}

		uint64_t LayerBase::getOutputSize() const
		{
			return _outputSize;
		}
	
		SoftMax::SoftMax(uint64_t inputSize, uint64_t outputSize) :
			LayerBase(inputSize, outputSize)
		{
			assert(inputSize == outputSize);
		}

		scp::Vec<float> SoftMax::operator()(const scp::Vec<float>& x) const
		{
			float s = 0.f;
			for (uint64_t i(0); i < x.n; i++)
				s += std::exp(x[i]);

			scp::Vec<float> result(x.n);
			for (uint64_t i(0); i < x.n; i++)
				result[i] = std::exp(x[i]) / s;

			return result;
		}

		void SoftMax::goThrough(const scp::Vec<float>& x, scp::Vec<float>& a, scp::Vec<float>& z) const
		{
			for (uint64_t i(0); i < x.n; i++)
				z[0] += std::exp(x[i]);

			for (uint64_t i(0); i < x.n; i++)
				a[i] = std::exp(x[i]) / z[0];
		}

		void SoftMax::computeCorrection(const scp::Vec<float>& x, const scp::Vec<float>& err, const scp::Vec<float>& a, const scp::Vec<float>& z, float learningRate, scp::Vec<float>& nextErr, scp::Mat<float>& correction) const
		{
			for (uint64_t i(0); i < _inputSize; i++)
				nextErr[i] = a[i]*(1.f - a[i])*err[i];
		}

		void SoftMax::applyCorrection(const scp::Mat<float>& correction)
		{
		}
	}

	scp::Vec<float> NeuralNetwork::operator()(const scp::Vec<float>& x) const
	{
		std::vector<scp::Vec<float>> a;
		a.push_back(x);
		for (uint64_t i(0); i < _layers.size(); i++)
			a.push_back((*_layers[i])(a.back()));

		return a.back();
	}

	void NeuralNetwork::train(const scp::Vec<float>& x, const scp::Vec<float>& y, float learningRate)
	{
		batchTrain({ x }, { y }, learningRate);
	}

	void NeuralNetwork::batchTrain(const std::vector<scp::Vec<float>>& x, const std::vector<scp::Vec<float>>& y, float learningRate)
	{
		assert(x.size() == y.size());

		std::vector<scp::Mat<float>> corrections;
		for (uint64_t i(0); i < _layers.size(); i++)
			corrections.push_back(scp::Mat<float>(_layers[i]->getOutputSize(), _layers[i]->getInputSize() + 1));

		for (uint64_t i(0); i < x.size(); i++)
		{
			std::vector<scp::Vec<float>> z, a, delta;
			z.push_back(scp::Vec<float>(0));
			a.push_back(x[i]);
			for (uint64_t j(0); j < _layers.size(); j++)
			{
				a.push_back(scp::Vec<float>(_layers[j]->getOutputSize()));
				z.push_back(scp::Vec<float>(_layers[j]->getOutputSize()));
				delta.push_back(scp::Vec<float>(_layers[j]->getInputSize()));
				_layers[j]->goThrough(a[j], a[j+1], z[j+1]);
			}

			delta.push_back(a.back() - y[i]);
			for (uint64_t j(_layers.size() - 1); j != UINT64_MAX; j--)
			{
				scp::Mat<float> correction(_layers[j]->getOutputSize(), _layers[j]->getInputSize() + 1);
				_layers[j]->computeCorrection(a[j], delta[j+1], a[j+1], z[j+1], learningRate, delta[j], correction);
				corrections[j] += correction;
			}
		}

		for (uint64_t i(0); i < _layers.size(); i++)
			_layers[i]->applyCorrection(corrections[i] / static_cast<float>(x.size()));
	}
}