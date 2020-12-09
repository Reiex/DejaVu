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