#include <DejaVu/DejaVu.hpp>

namespace djv
{
	namespace layers
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
	

		SoftMax::SoftMax(uint64_t size) :
			LayerBase(size, size)
		{
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
	
	
		Convolution2D::Convolution2D(uint64_t inputWidth, uint64_t inputHeight, uint64_t inputCount, uint64_t kernelWidth, uint64_t kernelHeight, const std::vector<uint64_t>& kernelCount) :
			_inputWidth(inputWidth),
			_inputHeight(inputHeight),
			_inputCount(inputCount),
			_kernelWidth(kernelWidth),
			_kernelHeight(kernelHeight),
			_kernelCount(kernelCount)
		{
			_inputSize = _inputWidth * _inputHeight * _inputCount;

			uint64_t outputCount = 0;
			for (uint64_t i(0); i < _kernelCount.size(); i++)
				outputCount += _kernelCount[i];

			_outputSize = _inputWidth * _inputHeight * outputCount;

			assert(kernelWidth & 1 == 1 && kernelHeight & 1 == 1);
			_kernels.resize(outputCount, scp::Mat<float>(_kernelWidth, _kernelHeight));

			for (uint64_t k(0); k < outputCount; k++)
				for (uint64_t i(0); i < _kernelWidth; i++)
					for (uint64_t j(0); j < kernelHeight; j++)
						_kernels[k][i][j] = (static_cast<float>(std::rand()) / RAND_MAX)/10.f - 0.05f;
		}

		scp::Vec<float> Convolution2D::operator()(const scp::Vec<float>& x) const
		{
			scp::Vec<float> result(_outputSize);

			uint64_t kernelIndex(0);
			for (int64_t k = 0; k < _inputCount; k++)
			{
				for (int64_t l = 0; l < _kernelCount[k]; l++)
				{
					for (int64_t i = 0; i < _inputWidth; i++)
					{
						for (int64_t j = 0; j < _inputHeight; j++)
						{
							uint64_t outputIndex = kernelIndex*(_inputWidth*_inputHeight) + i*(_inputHeight) + j;

							for (int64_t p = 0; p < _kernelWidth; p++)
							{
								for (int64_t q = 0; q < _kernelHeight; q++)
								{
									int64_t r = std::min(std::max(i + p - static_cast<int64_t>(_kernelWidth/2), int64_t(0)), static_cast<int64_t>(_inputWidth) - 1);
									int64_t s = std::min(std::max(j + q - static_cast<int64_t>(_kernelHeight/2), int64_t(0)), static_cast<int64_t>(_inputHeight) - 1);
									int64_t inputIndex = k*(_inputWidth*_inputHeight) + r*(_inputHeight) + s;

									result[outputIndex] += x[inputIndex] * _kernels[kernelIndex][p][q];
								}
							}
						}
					}

					kernelIndex++;
				}
			}

			return result;
		}

		void Convolution2D::goThrough(const scp::Vec<float>& x, scp::Vec<float>& a, scp::Vec<float>& z) const
		{
			uint64_t kernelIndex(0);
			for (int64_t k = 0; k < _inputCount; k++)
			{
				for (int64_t l = 0; l < _kernelCount[k]; l++)
				{
					for (int64_t i = 0; i < _inputWidth; i++)
					{
						for (int64_t j = 0; j < _inputHeight; j++)
						{
							uint64_t outputIndex = kernelIndex*(_inputWidth*_inputHeight) + i*(_inputHeight) + j;

							for (int64_t p = 0; p < _kernelWidth; p++)
							{
								for (int64_t q = 0; q < _kernelHeight; q++)
								{
									int64_t r = std::min(std::max(i + p - static_cast<int64_t>(_kernelWidth/2), int64_t(0)), static_cast<int64_t>(_inputWidth) - 1);
									int64_t s = std::min(std::max(j + q - static_cast<int64_t>(_kernelHeight/2), int64_t(0)), static_cast<int64_t>(_inputHeight) - 1);
									int64_t inputIndex = k*(_inputWidth*_inputHeight) + r*(_inputHeight) + s;

									a[outputIndex] += x[inputIndex] * _kernels[kernelIndex][p][q];
								}
							}
						}
					}

					kernelIndex++;
				}
			}
		}

		void Convolution2D::computeCorrection(const scp::Vec<float>& x, const scp::Vec<float>& err, const scp::Vec<float>& a, const scp::Vec<float>& z, float learningRate, scp::Vec<float>& nextErr, scp::Mat<float>& correction) const
		{
			uint64_t kernelIndex(0);
			for (int64_t k = 0; k < _inputCount; k++)
			{
				for (int64_t l = 0; l < _kernelCount[k]; l++)
				{
					for (int64_t i = 0; i < _inputWidth; i++)
					{
						for (int64_t j = 0; j < _inputHeight; j++)
						{
							for (int64_t p = 0; p < _kernelWidth; p++)
							{
								for (int64_t q = 0; q < _kernelHeight; q++)
								{
									int64_t r = std::min(std::max(i - p + static_cast<int64_t>(_kernelWidth/2), int64_t(0)), static_cast<int64_t>(_inputWidth) - 1);
									int64_t s = std::min(std::max(j - q + static_cast<int64_t>(_kernelHeight/2), int64_t(0)), static_cast<int64_t>(_inputHeight) - 1);
									_kernels[kernelIndex][p][q];
									err[kernelIndex*(_inputWidth*_inputHeight) + r*(_inputHeight) + s];
									nextErr[k*(_inputWidth*_inputHeight) + i*(_inputHeight) + j] += _kernels[kernelIndex][p][q] * err[kernelIndex*(_inputWidth*_inputHeight) + r*(_inputHeight) + s];
								}
							}
						}
					}

					kernelIndex++;
				}
			}

			kernelIndex = 0;
			for (int64_t k = 0; k < _inputCount; k++)
			{
				for (int64_t l = 0; l < _kernelCount[k]; l++)
				{
					for (int64_t p = 0; p < _kernelWidth; p++)
					{
						for (int64_t q = 0; q < _kernelHeight; q++)
						{
							for (int64_t i = 0; i < _inputWidth; i++)
							{
								for (int64_t j = 0; j < _inputHeight; j++)
								{
									int64_t r = std::min(std::max(i + p - static_cast<int64_t>(_kernelWidth/2), int64_t(0)), static_cast<int64_t>(_inputWidth) - 1);
									int64_t s = std::min(std::max(j + q - static_cast<int64_t>(_kernelHeight/2), int64_t(0)), static_cast<int64_t>(_inputHeight) - 1);
									correction[kernelIndex][p*_kernelHeight + q] += x[k*(_inputWidth*_inputHeight) + r*(_inputHeight) + s]*err[kernelIndex*(_inputWidth*_inputHeight) + i*(_inputHeight) + j]*learningRate/(_inputWidth*_inputHeight);
								}
							}
						}
					}

					kernelIndex++;
				}
			}
		}

		void Convolution2D::applyCorrection(const scp::Mat<float>& correction)
		{
			uint64_t kernelIndex = 0;
			for (int64_t k = 0; k < _inputCount; k++)
			{
				for (int64_t l = 0; l < _kernelCount[k]; l++)
				{
					for (int64_t p = 0; p < _kernelWidth; p++)
					{
						for (int64_t q = 0; q < _kernelHeight; q++)
						{
							_kernels[kernelIndex][p][q] -= correction[kernelIndex][p*_kernelHeight + q];
						}
					}

					kernelIndex++;
				}
			}
		}
	}


	namespace
	{
		void flattenedMatrix(const scp::Mat<float>& m, scp::Vec<float>& v)
		{
			for (uint64_t i(0); i < m.m; i++)
				for (uint64_t j(0); j < m.n; j++)
					v[i*m.n + j] = m[i][j];
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

	scp::Vec<float> NeuralNetwork::operator()(const scp::Mat<float>& m) const
	{
		scp::Vec<float> x(m.m * m.n);
		flattenedMatrix(m, x);
		return (*this)(x);
	}


	void NeuralNetwork::train(const scp::Vec<float>& x, const scp::Vec<float>& y, float learningRate)
	{
		batchTrain({ x }, { y }, learningRate);
	}

	void NeuralNetwork::train(const scp::Mat<float>& m, const scp::Vec<float>& y, float learningRate)
	{
		scp::Vec<float> x(m.m * m.n);
		flattenedMatrix(m, x);
		return train(x, y, learningRate);
	}

	void NeuralNetwork::batchTrain(const std::vector<scp::Vec<float>>& x, const std::vector<scp::Vec<float>>& y, float learningRate)
	{
		assert(x.size() != 0);
		assert(x.size() == y.size());

		std::vector<scp::Mat<float>> corrections;
		for (uint64_t i(0); i < _layers.size(); i++)
			corrections.push_back(scp::Mat<float>(_layers[i]->getOutputSize(), _layers[i]->getInputSize() + 1));
		
		int64_t i, j;
		#pragma omp parallel for private(i, j) shared(x, learningRate, corrections)
		for (i = 0; i < x.size(); i++)
		{
			std::vector<scp::Vec<float>> z, a, delta;
			z.push_back(scp::Vec<float>(0));
			a.push_back(x[i]);
			for (j = 0; j < _layers.size(); j++)
			{
				a.push_back(scp::Vec<float>(_layers[j]->getOutputSize()));
				z.push_back(scp::Vec<float>(_layers[j]->getOutputSize()));
				delta.push_back(scp::Vec<float>(_layers[j]->getInputSize()));
				_layers[j]->goThrough(a[j], a[j+1], z[j+1]);
			}

			delta.push_back(a.back() - y[i]);
			for (j = _layers.size() - 1; j != UINT64_MAX; j--)
			{
				scp::Mat<float> correction(_layers[j]->getOutputSize(), _layers[j]->getInputSize() + 1);
				_layers[j]->computeCorrection(a[j], delta[j+1], a[j+1], z[j+1], learningRate, delta[j], correction);
				#pragma omp critical
				corrections[j] += correction;
			}
		}

		#pragma omp parallel for private(i) shared(corrections, x)
		for (i = 0; i < _layers.size(); i++)
			_layers[i]->applyCorrection(corrections[i] / static_cast<float>(x.size()));
	}

	void NeuralNetwork::batchTrain(const std::vector<scp::Mat<float>>& m, const std::vector<scp::Vec<float>>& y, float learningRate)
	{
		assert(m.size() != 0);

		std::vector<scp::Vec<float>> x(m.size(), scp::Vec<float>(m[0].m*m[0].n));
		for (uint64_t i(0); i < m.size(); i++)
			flattenedMatrix(m[i], x[i]);

		return batchTrain(x, y, learningRate);
	}
}