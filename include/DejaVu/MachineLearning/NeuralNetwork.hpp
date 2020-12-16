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
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class LayerBase
		{
			public:

				LayerBase(uint64_t inputSize, uint64_t outputSize);

				virtual scp::Vec<float> operator()(const scp::Vec<float>& x) const = 0;

				virtual void goThrough(const scp::Vec<float>& x, scp::Vec<float>& a, scp::Vec<float>& z) const = 0;
				virtual void computeCorrection(const scp::Vec<float>& x, const scp::Vec<float>& err, const scp::Vec<float>& a, const scp::Vec<float>& z, float learningRate, scp::Vec<float>& nextErr, scp::Mat<float>& correction) const = 0;
				virtual void applyCorrection(const scp::Mat<float>& correction) = 0;

				virtual uint64_t getInputSize() const;
				virtual uint64_t getOutputSize() const;

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

				Perceptrons(uint64_t inputSize, uint64_t outputSize);

				scp::Vec<float> operator()(const scp::Vec<float>& x) const;

				void goThrough(const scp::Vec<float>& x, scp::Vec<float>& a, scp::Vec<float>& z) const;
				void computeCorrection(const scp::Vec<float>& x, const scp::Vec<float>& err, const scp::Vec<float>& a, const scp::Vec<float>& z, float learningRate, scp::Vec<float>& nextErr, scp::Mat<float>& correction) const;
				void applyCorrection(const scp::Mat<float>& correction);

			private:

				std::vector<TPerceptron> _neurons;
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \brief Class representing a neural network's softmax layer.
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class SoftMax : public LayerBase
		{
			public:

				SoftMax(uint64_t inputSize, uint64_t outputSize);

				scp::Vec<float> operator()(const scp::Vec<float>& x) const;

				void goThrough(const scp::Vec<float>& x, scp::Vec<float>& a, scp::Vec<float>& z) const;
				void computeCorrection(const scp::Vec<float>& x, const scp::Vec<float>& err, const scp::Vec<float>& a, const scp::Vec<float>& z, float learningRate, scp::Vec<float>& nextErr, scp::Mat<float>& correction) const;
				void applyCorrection(const scp::Mat<float>& correction);
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

			template<typename TLayer = layers::Perceptrons<perceptrons::Sigmoid>>
			void setInputLayer(uint64_t inputSize, uint64_t outputSize);
			template<typename TLayer = layers::Perceptrons<perceptrons::Sigmoid>>
			void appendLayer(uint64_t outputSize);

			scp::Vec<float> operator()(const scp::Vec<float>& x) const;

			void train(const scp::Vec<float>& x, const scp::Vec<float>& y, float learningRate = 0.01f);
			void batchTrain(const std::vector<scp::Vec<float>>& x, const std::vector<scp::Vec<float>>& y, float learningRate = 0.01f);

		private:

			std::vector<std::unique_ptr<layers::LayerBase>> _layers;
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

#include <DejaVu/MachineLearning/NeuralNetworkT.hpp>
