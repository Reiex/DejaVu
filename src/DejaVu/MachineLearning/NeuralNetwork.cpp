#include <DejaVu/DejaVu.hpp>

namespace djv
{
	namespace layer
	{
		LayerBase::LayerBase(uint64_t inputSize, uint64_t layerSize) :
			_inputSize(inputSize),
			_layerSize(layerSize)
		{
		}

		uint64_t LayerBase::getInputSize() const
		{
			return _inputSize;
		}

		uint64_t LayerBase::getLayerSize() const
		{
			return _layerSize;
		}
	
		SoftMax::SoftMax(uint64_t inputSize, uint64_t layerSize) :
			LayerBase(inputSize, layerSize)
		{
			assert(inputSize == layerSize);
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
			float s = 0.f;
			for (uint64_t i(0); i < x.n; i++)
				s += std::exp(x[i]);

			for (uint64_t i(0); i < x.n; i++)
				a[i] = std::exp(x[i]) / s;
		}

		scp::Vec<float> SoftMax::train(const scp::Vec<float>& x, const scp::Vec<float>& weightedErrors, const scp::Vec<float>& a, const scp::Vec<float>& z, float learningRate)
		{
			scp::Vec<float> newWeightedErrors(_inputSize);

			for (uint64_t i(0); i < newWeightedErrors.n; i++)
				newWeightedErrors[i] = a[i] * (1.f - a[i]) * weightedErrors[i];

			return newWeightedErrors;
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
		std::vector<scp::Vec<float>> z, a, delta;
		z.push_back(scp::Vec<float>(0));
		a.push_back(x);
		for (uint64_t i(0); i < _layers.size(); i++)
		{
			a.push_back(scp::Vec<float>(_layers[i]->getLayerSize()));
			z.push_back(scp::Vec<float>(_layers[i]->getLayerSize()));
			delta.push_back(scp::Vec<float>(_layers[i]->getInputSize()));
			_layers[i]->goThrough(a[i], a[i+1], z[i+1]);
		}

		delta.push_back(a.back() - y);
		for (uint64_t i(_layers.size() - 1); i != UINT64_MAX; i--)
			delta[i] = _layers[i]->train(a[i], delta[i+1], a[i+1], z[i+1], learningRate);
	}
}