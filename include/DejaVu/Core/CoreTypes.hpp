///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Pélégrin Marius
//! \copyright The MIT License (MIT)
//! \date 2020-2023
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <deque>
#include <filesystem>

#include <SciPP/SciPPTypes.hpp>
#include <Diskon/DiskonTypes.hpp>

namespace djv
{
	template<typename T> concept CShape = (
		requires {
			typename T::Generator;
		}
		&& requires (T a, int64_t x, int64_t y) {
			{ a.getGenerator() } -> std::same_as<typename T::Generator>;
			{ a.getGenerator().getNextPixel(x, y) } -> std::same_as<bool>;
		}
		&& std::movable<T>
		&& std::copyable<T>
		&& std::movable<typename T::Generator>
		&& std::copyable<typename T::Generator>
	);

	class ShapeThickRect;
	class ShapeCrown;
	class ShapeLine;
	class ShapeFilledRect;
	class ShapeRect;
	class ShapeDisc;
	class ShapeCircle;

	template<typename TComponent, uint8_t ComponentCount> class Pixel;
	template<typename T> concept CPixel = requires { typename T::ComponentType; T::componentCount; } && std::derived_from<T, Pixel<typename T::ComponentType, T::componentCount>>;
	namespace colors
	{
		template<typename TComponent, uint8_t ComponentCount> static constexpr Pixel<TComponent, ComponentCount> black = std::integral<TComponent> ? std::numeric_limits<TComponent>::min() : -1.0;
		template<typename TComponent, uint8_t ComponentCount> static constexpr Pixel<TComponent, ComponentCount> white = std::integral<TComponent> ? std::numeric_limits<TComponent>::max() : 1.0;
	}

	enum class ImageFormat;
	template<CPixel TPixel> class Image;
	template<typename T> concept CImage = requires { typename T::PixelType; } && CPixel<typename T::PixelType> && std::derived_from<T, Image<typename T::PixelType>>;

}
