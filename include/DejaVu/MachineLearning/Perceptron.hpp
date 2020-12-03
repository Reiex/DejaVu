#pragma once

#include <DejaVu/types.hpp>

namespace djv
{
	class Perceptron
	{
		public:

			Perceptron(uint64_t n, const std::string& function);

			float operator()(scp::Vec<float> x);

			const scp::Vec<float>& getWeights() const;
			void setWeights(scp::Vec<float>& weights);
			float getBias() const;
			void setBias(float bias);

		private:

			scp::Vec<float> _weights;
			float _bias;

			// Store function ?
	};
}
