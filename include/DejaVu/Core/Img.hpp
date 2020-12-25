#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief Functions and classes for simple image manipulations.
/// \author Reiex
/// 
/// For more complex image manipulations, see the image processing module.
/// For a more detailed description, see class Img.
/// 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <DejaVu/types.hpp>

namespace djv
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \brief Permits selection a single color component from an RGBA image.
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	enum class ColorComponent
	{
		R = 0,
		G = 1,
		B = 2,
		A = 3
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \brief Class representing an RGBA image that can then be manipulated globally component per component.
	/// 
	/// A pixel component in class Img should always be between 0.0 and 1.0. However, values over a larger range can be
	/// needed for certain image processing algorithms. Thus, any value is accepted.
	/// 
	/// The position `(0, 0)` refer to the pixel at the top-left corner of the image.
	/// x is greater when going to the right and y is greater when going bottom.
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class Img
	{
		public:

			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \brief Init the image from a file.
			/// 
			/// DejaVu internally uses STB to load and save images, thus supported formats are formats supported by the
			/// STB. For more informations, see https://github.com/nothings/stb.
			/// 
			/// Depending on the format, you may need to use `transpose`, `flipHorizontally` and `flipVertically`.
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			Img(const std::string& filename, bool transpose = false, bool flipHorizontally = false, bool flipVertically = false);
			Img(uint64_t width = 1, uint64_t height = 1);  ///< Default constructor. Init the image at the defined size and black `(0.0, 0.0, 0.0, 1.0)`.
			Img(const scp::Mat<float>& grayScale);         ///< Init the red, green and blue components with grayScale and the alpha component with 1.0.
			Img(const Img& image);
			Img(Img&& image) = default;

			Img& operator=(const Img& image);
			Img& operator=(Img&& image) = default;

			float& operator()(uint64_t x, uint64_t y, ColorComponent component);              ///< Returns the desired component of the pixel at position `(x, y)`.
			const float& operator()(uint64_t x, uint64_t y, ColorComponent component) const;  ///< Returns the desired component of the pixel at position `(x, y)`.
			float& operator()(uint64_t x, uint64_t y, uint8_t component);                     ///< Returns the desired component of the pixel at position `(x, y)`.
			const float& operator()(uint64_t x, uint64_t y, uint8_t component) const;         ///< Returns the desired component of the pixel at position `(x, y)`.
			Color operator()(uint64_t x, uint64_t y) const;									  ///< Returns the color of the pixel at position `(x, y)`.
			void setPixel(uint64_t x, uint64_t y, const Color& color);						  ///< Sets the color of the pixel at position `(x, y)`.
			const scp::Mat<float>& getComponent(ColorComponent component) const;			  ///< Returns a constant reference to the desired component of the image.
			scp::Mat<float>& getComponent(ColorComponent component);						  ///< Returns a reference the desired component of the image.
			void setComponent(ColorComponent component, const scp::Mat<float> m);			  ///< Sets the desired component of the image.

			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \brief Save the image with the desired format (specified as the extension of the filename)
			/// 
			/// Pixel components with values higher than 1.0 or lower than 0.0 are clamped.
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			void saveToFile(const std::string& filename) const;
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \brief Crop the image to the specified rectangular area.
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			Img subRect(uint64_t left, uint64_t top, uint64_t width, uint64_t height) const;
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \brief Draw a shape on the image. For more information, see class Shape and it's derivatives.
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			void draw(const Shape& shape);
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \brief Set the color of each pixel to the one of the corresponding group in the segmentation.
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			void applySegmentationColor(const segmentation::ImageSegmentation& seg);
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \brief Returns a grayscale matrix of the image computed using the specified factors.
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			scp::Mat<float> grayScale(float redFactor = 0.2126f, float greenFactor = 0.7152f, float blueFactor = 0.0722f) const;

			uint64_t width() const;												   ///< Returns the image width in pixels.
			uint64_t height() const;											   ///< Returns the image height in pixels.

			~Img() = default;

		private:

			uint64_t _width;
			uint64_t _height;

			std::unique_ptr<scp::Mat<float>> _r;
			std::unique_ptr<scp::Mat<float>> _g;
			std::unique_ptr<scp::Mat<float>> _b;
			std::unique_ptr<scp::Mat<float>> _a;
	};
}
