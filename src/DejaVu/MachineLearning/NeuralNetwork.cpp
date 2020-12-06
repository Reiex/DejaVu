#include <DejaVu/DejaVu.hpp>

namespace djv
{
	scp::Vec<float> NeuralNetwork::operator()(const scp::Vec<float>& x) const
	{
		std::vector<scp::Vec<float>> a;
		a.push_back(x);
		for (uint64_t i(0); i < _neurons.size(); i++)
		{
			const std::vector<std::unique_ptr<Perceptron::Perceptron>>& layer = _neurons[i];
			a.push_back(scp::Vec<float>(layer.size()));
			for (uint64_t j(0); j < layer.size(); j++)
				a[i + 1][j] = (*layer[j])(a[i]);
		}

		return a[a.size() - 1];
	}

	void NeuralNetwork::train(const scp::Vec<float>& x, const scp::Vec<float>& y, float learningRate)
	{
		std::vector<scp::Vec<float>> z, a, delta;
		z.push_back(scp::Vec<float>(0));
		a.push_back(x);
		delta.push_back(scp::Vec<float>(0));
		for (uint64_t i(0); i < _neurons.size(); i++)
		{
			const std::vector<std::unique_ptr<Perceptron::Perceptron>>& layer = _neurons[i];

			z.push_back(scp::Vec<float>(layer.size()));
			a.push_back(scp::Vec<float>(layer.size()));
			delta.push_back(scp::Vec<float>(layer.size()));
			for (uint64_t j(0); j < layer.size(); j++)
			{
				z[i + 1][j] = scp::dot(layer[j]->getWeights(), a[i]) + layer[j]->getBias();
				a[i + 1][j] = layer[j]->f(z[i + 1][j]);
			}
		}

		uint64_t lastLayer = delta.size() - 1;
		for (uint64_t i(0); i < delta[lastLayer].n; i++)
			delta[lastLayer][i] = _neurons[lastLayer - 1][i]->nntrain(x, a[lastLayer][i] - y[i], a[lastLayer][i], z[lastLayer][i], learningRate);

		for (uint64_t i(delta.size() - 2); i > 0; i--)
		{
			for (uint64_t j(0); j < delta[i].n; j++)
			{
				scp::Vec<float> weights(_neurons[i].size());
				for (uint64_t k(0); k < weights.n; k++)
					weights[k] = _neurons[i][k]->getWeights()[j];

				delta[i][j] = _neurons[i - 1][j]->nntrain(x, scp::dot(weights, delta[i+1]), a[i][j], z[i][j], learningRate);
			}
		}
	}
}