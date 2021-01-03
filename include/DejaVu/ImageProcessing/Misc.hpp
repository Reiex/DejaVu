#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief General purpose image processing functions.
/// \author Reiex
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <DejaVu/types.hpp>

namespace djv
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \addtogroup ImageProcessing
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	namespace operators
	{
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \addtogroup operators
		/// \ingroup ImageProcessing
		/// \brief List of common operators on images such as gradients, laplacians, blurs...
		/// \{
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \brief Returns an approximation of the gradient of a matrix.
		/// 
		/// This approximation is computed using the kernels `(-1 0 1)` and its transpose.
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		std::array<scp::Mat<float>, 2> simpleGradient(const scp::Mat<float>& m);
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \brief Returns the Sobel gradient of a matrix.
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		std::array<scp::Mat<float>, 2> sobel(const scp::Mat<float>& m);
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \brief Returns the Prewitt gradient of a matrix.
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		std::array<scp::Mat<float>, 2> prewitt(const scp::Mat<float>& m);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \brief Returns the second order derivatives of a matrix.
		/// 
		/// This function returns three matrices containing the second order derivatives
		/// \f$(\partial_{xx}m, \partial_{xy}m, \partial_{yy}m)\f$.
		/// These derivatives are computed using the kernels `(1 -2 1)` and its transpose and
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		std::array<scp::Mat<float>, 3> simpleHessian(const scp::Mat<float>& m);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \brief Returns an approximation of the laplacian of a matrix.
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		scp::Mat<float> simpleLaplacian(const scp::Mat<float>& m);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}

	namespace blur
	{
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \addtogroup blur
		/// \ingroup ImageProcessing
		/// \{
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \brief Returns the blurred matrix using a gaussian function.
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		scp::Mat<float> gaussian(const scp::Mat<float>& m, float sigma = 1.f);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \brief Returns the denoised matrix using bilateral filter.
		/// 
		/// The function used for the spatial component is a gaussian.
		/// The function used for the intensity component is a gaussian.
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		scp::Mat<float> gaussianBilateral(const scp::Mat<float>& m, float sigmaSpace = 1.f, float sigmaIntensity = 0.1f);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
}
