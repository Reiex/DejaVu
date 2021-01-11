#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief Functions and classes for neural networks creation and manipulation.
/// \author Reiex
/// 
/// For a more detailed description, see class NeuralNetwork.
/// 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <DejaVu/types.hpp>

namespace djv
{
	namespace layers
	{
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \addtogroup layers
		/// \ingroup MachineLearning
		/// \{
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \brief Abstract class, base model for a NeuralNetwork layer.
		/// 
		/// The data that pass from a layer to another is always a vector. For convolutionnal layers, the data is
		/// flattened in column major order.
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class LayerBase
		{
			public:

				LayerBase(uint64_t inputSize = 0, uint64_t outputSize = 0);  ///< Specify the input and output vector sizes.
				LayerBase(const LayerBase& layer) = default;
				LayerBase(LayerBase&& layer) = default;

				LayerBase& operator=(const LayerBase& layer) = default;
				LayerBase& operator=(LayerBase&& layer) = default;

				///////////////////////////////////////////////////////////////////////////////////////////////////////
				/// \brief Applies the layer to a feature.
				/// 
				/// The returned vector must be of size `outputSize`.
				///////////////////////////////////////////////////////////////////////////////////////////////////////
				virtual scp::Vec<float> operator()(const scp::Vec<float>& x) const = 0;

				///////////////////////////////////////////////////////////////////////////////////////////////////////
				/// \brief Applies the layer to a feature but save every intermediary step to avoid recomputation.
				/// 
				/// For an input feature `x`, this function compute the output `a` and an intermediary vector of size
				/// `outputSize`: `z`.
				/// For instance, for a perceptrons layer, `z` correspond to the dot product of weights and `x` for
				/// each perceptron (plus the biases) and `a` to the activation function applied to each element of
				/// `z`.
				///////////////////////////////////////////////////////////////////////////////////////////////////////
				virtual void goThrough(const scp::Vec<float>& x, scp::Vec<float>& a, scp::Vec<float>& z) const = 0;
				///////////////////////////////////////////////////////////////////////////////////////////////////////
				/// \brief Computes the correction to be applied and the error of the layer for back propagation.
				/// 
				/// For an input feature `x`, a computed output error `err`, a computed output `a`, a computed
				/// intermediary result `z` and a learning rate `learningRate`, this function compute the output error
				/// vector for the precedent layer and a matrix of corrections of size `outputSize*(inputSize + 1)`.
				///////////////////////////////////////////////////////////////////////////////////////////////////////
				virtual void computeCorrection(const scp::Vec<float>& x, const scp::Vec<float>& err, const scp::Vec<float>& a, const scp::Vec<float>& z, float learningRate, scp::Vec<float>& nextErr, scp::Mat<float>& correction) const = 0;
				///////////////////////////////////////////////////////////////////////////////////////////////////////
				/// \brief Applies the correction computed by computeCorrection.
				///////////////////////////////////////////////////////////////////////////////////////////////////////
				virtual void applyCorrection(const scp::Mat<float>& correction) = 0;

				uint64_t getInputSize() const;   ///< Returns the layer's input size.
				uint64_t getOutputSize() const;  ///< Returns the layer's output size.

				~LayerBase() = default;

			protected:

				uint64_t _inputSize;
				uint64_t _outputSize;
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \brief Template class representing a neural network's layer of perceptrons.
		/// 
		/// Classes that can be used as TPerceptron are classes derived from djv::perceptrons::PerceptronBase
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		template<typename TPerceptron = perceptrons::Sigmoid>
		class Perceptrons : public LayerBase
		{
			public:


				Perceptrons() = delete;
				Perceptrons(uint64_t inputSize, uint64_t outputSize);
				Perceptrons(const Perceptrons& layer) = default;
				Perceptrons(Perceptrons&& layer) = default;

				Perceptrons& operator=(const Perceptrons& layer) = default;
				Perceptrons& operator=(Perceptrons&& layer) = default;

				scp::Vec<float> operator()(const scp::Vec<float>& x) const;

				void goThrough(const scp::Vec<float>& x, scp::Vec<float>& a, scp::Vec<float>& z) const;
				void computeCorrection(const scp::Vec<float>& x, const scp::Vec<float>& err, const scp::Vec<float>& a, const scp::Vec<float>& z, float learningRate, scp::Vec<float>& nextErr, scp::Mat<float>& correction) const;
				void applyCorrection(const scp::Mat<float>& correction);

				~Perceptrons() = default;

			private:

				std::vector<TPerceptron> _neurons;
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \brief Class representing a neural network's softmax layer.
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class SoftMax : public LayerBase
		{
			public:

				SoftMax() = delete;
				SoftMax(uint64_t size);
				SoftMax(const SoftMax& layer) = default;
				SoftMax(SoftMax&& layer) = default;

				SoftMax& operator=(const SoftMax& layer) = default;
				SoftMax& operator=(SoftMax&& layer) = default;

				scp::Vec<float> operator()(const scp::Vec<float>& x) const;

				void goThrough(const scp::Vec<float>& x, scp::Vec<float>& a, scp::Vec<float>& z) const;
				void computeCorrection(const scp::Vec<float>& x, const scp::Vec<float>& err, const scp::Vec<float>& a, const scp::Vec<float>& z, float learningRate, scp::Vec<float>& nextErr, scp::Mat<float>& correction) const;
				void applyCorrection(const scp::Mat<float>& correction);

				~SoftMax() = default;
		};

		class Convolution2D : public LayerBase
		{
			public:

				Convolution2D(uint64_t inputWidth, uint64_t inputHeight, uint64_t inputCount, uint64_t kernelWidth, uint64_t kernelHeight, const std::vector<uint64_t>& kernelCount);

				scp::Vec<float> operator()(const scp::Vec<float>& x) const;

				void goThrough(const scp::Vec<float>& x, scp::Vec<float>& a, scp::Vec<float>& z) const;
				void computeCorrection(const scp::Vec<float>& x, const scp::Vec<float>& err, const scp::Vec<float>& a, const scp::Vec<float>& z, float learningRate, scp::Vec<float>& nextErr, scp::Mat<float>& correction) const;
				void applyCorrection(const scp::Mat<float>& correction);

			private:

				uint64_t _inputWidth;
				uint64_t _inputHeight;
				uint64_t _inputCount;
				uint64_t _kernelWidth;
				uint64_t _kernelHeight;
				std::vector<uint64_t> _kernelCount;
				std::vector<scp::Mat<float>> _kernels;
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \addtogroup MachineLearning
	/// \{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \brief Class representing a full neural network.
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class NeuralNetwork
	{
		public:

			NeuralNetwork() = default;
			NeuralNetwork(const NeuralNetwork& net) = delete;
			NeuralNetwork(NeuralNetwork&& net) = default;

			NeuralNetwork& operator=(const NeuralNetwork& net) = delete;
			NeuralNetwork& operator=(NeuralNetwork&& net) = default;

			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \brief Add a layer to the NeuralNetwork.
			/// 
			/// The layer class must be derived from layers::LayerBase.
			/// The layer instance is copied, thus a unique instance of layer can be added multiple times to the neural
			/// network (if the input and output are compatible).
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			template<typename TLayer>
			void addLayer(const TLayer& layer);

			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \brief Compute the output of the neural network for an input vector `x`.
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			scp::Vec<float> operator()(const scp::Vec<float>& x) const;
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \brief Compute the output of the neural network for an input matrix.
			/// 
			/// This function simply flatten the matrix to a vector and call the neural network on this vector.
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			scp::Vec<float> operator()(const scp::Mat<float>& m) const;

			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \brief Trains the neural network on a single training vector, the expected output and a learning rate.
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			void train(const scp::Vec<float>& x, const scp::Vec<float>& y, float learningRate = 0.01f);
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \brief Trains the neural network on a single training matrix, the expected output and a learning rate.
			/// 
			/// This function simply flatten the matrix to a vector and train the neural network on this vector.
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			void train(const scp::Mat<float>& m, const scp::Vec<float>& y, float learningRate = 0.01f);
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \brief Trains the neural network on a batch of training vectors and expected outputs.
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			void batchTrain(const std::vector<scp::Vec<float>>& x, const std::vector<scp::Vec<float>>& y, float learningRate = 0.01f);
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \brief Trains the neural network on a batch of training matrices and expected outputs.
			/// 
			/// This function simply flatten the matrices to vectors and train the neural network on these vectors.
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			void batchTrain(const std::vector<scp::Mat<float>>& m, const std::vector<scp::Vec<float>>& y, float learningRate = 0.01f);

			~NeuralNetwork() = default;

		private:

			std::vector<std::unique_ptr<layers::LayerBase>> _layers;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

#include <DejaVu/MachineLearning/NeuralNetworkT.hpp>
