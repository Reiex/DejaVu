#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief Functions and classes for feature detection.
/// \author Reiex
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <DejaVu/types.hpp>

namespace djv
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \addtogroup ImageProcessing
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	namespace edgeDetectors
	{
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \addtogroup edgeDetectors
		/// \ingroup ImageProcessing
		/// \{
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \brief Returns the square of the magnitude of the Sobel gradient.
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		scp::Mat<float> sobel(const scp::Mat<float>& m);
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \brief Returns the square of the magnitude of the Prewitt gradient.
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		scp::Mat<float> prewitt(const scp::Mat<float>& m);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \brief Returns the result of the Marr-Hildreth edge detection algorithm.
		/// 
		/// The laplacian computation method used is the laplacian of gaussian (LoG).
		/// The gradient computation method used it the derivative of gaussian.
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		scp::Mat<float> marrHildreth(const scp::Mat<float>& m, float sigma = 1.5f);
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \brief Returns the result of the Canny edge detection algorithm.
		/// 
		/// The gradient computation method used it the derivative of gaussian.
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		scp::Mat<float> canny(const scp::Mat<float>& m, float sigma = 1.5f);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}

	namespace lineExtractors
	{
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \addtogroup lineExtractors
		/// \ingroup ImageProcessing
		/// \{
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \brief Returns a list of lines detected using Hough transform.
		/// 
		/// The Hough transform is first computed using a discretization of the Hough space using dTheta and dRho.
		/// Then the matrix containing the Hough transform is normalized.
		/// Finaly, a line is computed for every coefficient of the matrix greater than `threshold`.
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		std::vector<Line> hough(const scp::Mat<float>& m, float threshold = 0.5f, float dTheta = 0.1f, float dRho = 5.f);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}

}
