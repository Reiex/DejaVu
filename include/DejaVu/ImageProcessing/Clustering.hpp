#pragma once

#include <DejaVu/ImageProcessing/types.hpp>

namespace djv
{
	namespace clustering
	{
		struct Clustering
		{
			Clustering() = delete;
			Clustering(uint64_t w, uint64_t h);
			Clustering(const Clustering& seg) = default;
			Clustering(Clustering&& seg) = default;

			template<typename PixelType>
			Img<PixelType> appliedTo(const Img<PixelType>& image) const;

			scp::Mat<uint64_t> clusterMap;
		};

		template<typename PixelType>
		Clustering kMeans(const Img<PixelType>& m, uint64_t k, float epsilon = 1e-5f);

		template<typename PixelType>
		Clustering slic(const Img<PixelType>& m, uint64_t k, float posWeight = 40.f, float epsilon = -1.f);
	}
}

#include <DejaVu/ImageProcessing/ClusteringT.hpp>
