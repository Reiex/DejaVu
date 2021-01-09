#pragma once

#include <DejaVu/types.hpp>

namespace djv
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \addtogroup MachineLearning
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	namespace perceptrons
	{
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \addtogroup perceptrons
		/// \ingroup MachineLearning
		/// \{
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \brief Abstract cass, base model for a perceptron.
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class PerceptronBase
		{
			public:

				PerceptronBase() = delete;
				PerceptronBase(uint64_t inputSize);  ///< Common constructor for every perceptron.
				PerceptronBase(const PerceptronBase& p) = default;
				PerceptronBase(PerceptronBase&& p) = default;

				PerceptronBase& operator=(const PerceptronBase& p) = default;
				PerceptronBase& operator=(PerceptronBase&& p) = default;

				///////////////////////////////////////////////////////////////////////////////////////////////////////
				/// \brief Applies the perceptron to a feature.
				///////////////////////////////////////////////////////////////////////////////////////////////////////
				float operator()(const scp::Vec<float>& x) const;

				///////////////////////////////////////////////////////////////////////////////////////////////////////
				/// \brief Applies the perceptron to a feature but save every intermediary step to avoid recomputation.
				/// 
				/// For an input feature `x`, this function compute the activation `a` and the dot product of `x` with
				/// the weights plus the bias in `z`.
				///////////////////////////////////////////////////////////////////////////////////////////////////////
				void goThrough(const scp::Vec<float>& x, float& a, float& z) const;
				///////////////////////////////////////////////////////////////////////////////////////////////////////
				/// \brief Trains the perceptron considering an input `x` and a desired output `y`.
				/// 
				/// This function is meant to be used for training of individual perceptrons and not for perceptrons
				/// inside a neural network.
				///////////////////////////////////////////////////////////////////////////////////////////////////////
				void train(const scp::Vec<float>& x, float y, float learningRate = 0.005f);
				///////////////////////////////////////////////////////////////////////////////////////////////////////
				/// \brief Computes the correction to be applied and the error for back propagation.
				/// 
				/// For an input feature `x`, a computed output error `err`, a computed activation `a`, a computed
				/// intermediary result `z` and a learning rate `learningRate`, this function compute the back
				/// propagation error term and a vector of corrections of size `inputSize + 1`.
				///////////////////////////////////////////////////////////////////////////////////////////////////////
				void computeCorrection(const scp::Vec<float>& x, float err, float a, float z, float learningRate, float& nextErr, scp::Vec<float>& correction) const;
				///////////////////////////////////////////////////////////////////////////////////////////////////////
				/// \brief Applies the correction computed by computeCorrection.
				///////////////////////////////////////////////////////////////////////////////////////////////////////
				void applyCorrection(const scp::Vec<float>& correction);

				///////////////////////////////////////////////////////////////////////////////////////////////////////
				/// \brief Returns the activation function evaluated in `z`.
				///////////////////////////////////////////////////////////////////////////////////////////////////////
				virtual float f(float z) const = 0;
				///////////////////////////////////////////////////////////////////////////////////////////////////////
				/// \brief Returns the derivative of the activation function evaluated in `z`.
				/// 
				/// Most commons activation functions have derivative that can be expressed easily using the image of
				/// the function. That is why in addition to the value the derivative of the activation function must
				/// be computed (`z`), the result of the activation function in `z` is also given: `a`.
				///////////////////////////////////////////////////////////////////////////////////////////////////////
				virtual float df(float a, float z) const = 0;

				const scp::Vec<float>& getWeights() const;        ///< Returns the weights of the perceptron.
				void setWeights(const scp::Vec<float>& weights);  ///< Sets the weights of the perceptron.
				float getBias() const;                            ///< Returns the bias of the perceptron.
				void setBias(float bias);                         ///< Sets the bias of the perceptron.

				~PerceptronBase() = default;

			protected:

				scp::Vec<float> _weights;
				float _bias;
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \brief Perceptron with Binary Step activation function.
		/// 
		/// The activation is: \f$a(z) = 0\f$ if \f$z < 0\f$ else \f$1\f$.
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class BinaryStep : public PerceptronBase
		{
			public:

				BinaryStep() = delete;
				BinaryStep(uint64_t inputSize);
				BinaryStep(const BinaryStep& p) = default;
				BinaryStep(BinaryStep&& p) = default;

				BinaryStep& operator=(const BinaryStep& p) = default;
				BinaryStep& operator=(BinaryStep&& p) = default;

				float f(float z) const;
				float df(float a, float z) const;

				~BinaryStep() = default;
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \brief Perceptron with Leaky Rectified Linear Unit activation function.
		/// 
		/// The activation is: \f$a(z) = -0.1z\f$ if \f$z < 0\f$ else \f$z\f$.
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class LeakyReLU : public PerceptronBase
		{
			public:

				LeakyReLU() = delete;
				LeakyReLU(uint64_t inputSize);
				LeakyReLU(const LeakyReLU& p) = default;
				LeakyReLU(LeakyReLU&& p) = default;

				LeakyReLU& operator=(const LeakyReLU& p) = default;
				LeakyReLU& operator=(LeakyReLU&& p) = default;

				float f(float z) const;
				float df(float a, float z) const;

				~LeakyReLU() = default;
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \brief Perceptron with Linear activation function.
		/// 
		/// The activation is: \f$a(z) = z\f$.
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class Linear : public PerceptronBase
		{
			public:

				Linear() = delete;
				Linear(uint64_t inputSize);
				Linear(const Linear& p) = default;
				Linear(Linear&& p) = default;

				Linear& operator=(const Linear& p) = default;
				Linear& operator=(Linear&& p) = default;

				float f(float z) const;
				float df(float a, float z) const;

				~Linear() = default;
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \brief Perceptron with Rectified Linear Unit activation function.
		/// 
		/// The activation is: \f$a(z) = 0\f$ if \f$z < 0\f$ else \f$z\f$.
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class ReLU : public PerceptronBase
		{
			public:

				ReLU() = delete;
				ReLU(uint64_t inputSize);
				ReLU(const ReLU& p) = default;
				ReLU(ReLU&& p) = default;

				ReLU& operator=(const ReLU& p) = default;
				ReLU& operator=(ReLU&& p) = default;

				float f(float z) const;
				float df(float a, float z) const;

				~ReLU() = default;
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \brief Perceptron with Sigmoid activation function.
		/// 
		/// The activation is: \f$a(z) = \frac{1}{1 + e^{-z}}\f$.
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class Sigmoid: public PerceptronBase
		{
			public:

				Sigmoid() = delete;
				Sigmoid(uint64_t inputSize);
				Sigmoid(const Sigmoid& p) = default;
				Sigmoid(Sigmoid&& p) = default;

				Sigmoid& operator=(const Sigmoid& p) = default;
				Sigmoid& operator=(Sigmoid&& p) = default;

				float f(float z) const;
				float df(float a, float z) const;

				~Sigmoid() = default;
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \brief Perceptron with Sigmoid activation function.
		/// 
		/// The activation is: \f$a(z) = tanh(z)\f$.
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class Tanh : public PerceptronBase
		{
			public:

				Tanh() = delete;
				Tanh(uint64_t inputSize);
				Tanh(const Tanh& p) = default;
				Tanh(Tanh&& p) = default;

				Tanh& operator=(const Tanh& p) = default;
				Tanh& operator=(Tanh&& p) = default;

				float f(float z) const;
				float df(float a, float z) const;

				~Tanh() = default;
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
}
