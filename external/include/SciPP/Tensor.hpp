#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file Tensor.hpp
/// \brief Functions and classes for tensors manipulations.
/// \author Reiex
/// 
/// For a more detailed description, see class Tensor.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <SciPP/types.hpp>

namespace scp
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \struct TensorPosition
	/// \brief Struct representing an index in a tensor.
	/// 
	/// A tensor in SciPP is both a contiguous array and a 'tree' of subtensors. Thus you can access an element by a
	/// set of N indices or by a global index giving its position in the contiguous array.
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<uint64_t N>
	struct TensorPosition
	{
		uint64_t indices[N];	///< Array of N indices with `0 <= indices[i] < shape[i]`.
		uint64_t globalIndex;	///< Global index of the element (see Tensor::get).
	};

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \class TensorIterator
	/// \brief Class for iterating over the possible indices of a given Tensor.
	/// 
	/// Iterates over a Tensor in a row major order. Dereferencing this class returns a TensorPosition instance.
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<uint64_t N>
	class TensorIterator
	{
		public:

			TensorIterator() = delete;
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \brief Constructs an iterator directly from a tensor.
			/// \param end If true, the iterator references the position "right after" the last position of the tensor.
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			template<typename T> TensorIterator(const Tensor<T, N>& a, bool end = false);
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \brief Constructs a tensor from a shape, indices and the global index associated.
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			TensorIterator(const uint64_t* shape, const uint64_t* indices, uint64_t globalIndex);
			TensorIterator(const TensorIterator<N>& iterator) = default;
			TensorIterator(TensorIterator<N>&& iterator) = default;

			TensorIterator<N>& operator=(const TensorIterator<N>& iterator) = default;
			TensorIterator<N>& operator=(TensorIterator<N>&& iterator) = default;

			const TensorPosition<N>& operator*() const;
			const TensorPosition<N>* operator->() const;

			TensorIterator<N>& operator++();	///< Iterates over a Tensor in a row major order.
			TensorIterator<N> operator++(int);  ///< Iterates over a Tensor in a row major order.

			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \brief There is equality iff the shapes of the tensors and indices of the iterators are the same.
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			bool operator==(const TensorIterator<N>& iterator) const;
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \brief There is inequality iff the shapes of the tensors or indices of the iterators are different.
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			bool operator!=(const TensorIterator<N>& iterator) const;

			~TensorIterator() = default;

		private:

			uint64_t _shape[N];		///< Shape of the underlying tensor.
			TensorPosition<N> _pos;	///< Current index of the iterator.
	};

	template<typename T, uint64_t N> struct _TensorTypeEncapsulator { using Type = Tensor<T, N>; };
	template<typename T> struct _TensorTypeEncapsulator<T, 0> { using Type = T; };

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \class Tensor
	/// \brief Class representing a mathematical tensor of order N.
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename T, uint64_t N>
	class Tensor
	{
		public:

			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \brief Alias for type of "sub-tensor", a tensor of order `N-1` (which is `T` for `N = 1`).
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			using SubTensor = typename _TensorTypeEncapsulator<T, N - 1>::Type;

			Tensor(const uint64_t* shape, const T& x);					///< Construct the tensor with x everywhere.
			Tensor(const uint64_t* shape, int64_t x = 0);				///< Construct the tensor with T(x) everywhere.
			Tensor(const std::vector<uint64_t>& shape, const T& x);		///< Construct the tensor with x everywhere.
			Tensor(const std::vector<uint64_t>& shape, int64_t x = 0);	///< Construct the tensor with T(x) everywhere.
			Tensor(const Tensor<T, N>& a);
			Tensor(Tensor<T, N>&& a);

			Tensor<T, N>& operator=(const Tensor<T, N>& a);
			Tensor<T, N>& operator=(Tensor<T, N>&& a);

			uint64_t globalLength() const;		///< Returns the total number of elements in the tensor.
			const uint64_t* shape() const;		///< Returns the global shape of the tensor.
			uint64_t shape(uint64_t i) const;	///< Returns the size of the tensor on the i-th dimension.
			SubTensor& operator[](uint64_t i);
			const SubTensor& operator[](uint64_t i) const;
			T& get(const uint64_t* indices);				///< Directly get an element from an array of indices.
			const T& get(const uint64_t* indices) const;	///< Directly get an element from an array of indices.
			T& get(uint64_t globalIndex);					///< Directly get an element from its global index.
			const T& get(uint64_t globalIndex) const;		///< Directly get an element from its global index.
			TensorIterator<N> begin() const;				///< Returns an iterator at the begining of the tensor.
			TensorIterator<N> end() const;					///< Returns an iterator right after the end of the tensor.

			Tensor<T, N>& operator+=(const Tensor<T, N>& a);
			Tensor<T, N>& operator-=(const Tensor<T, N>& a);
			Tensor<T, N>& operator*=(T x);
			Tensor<T, N>& operator/=(T x);

			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \brief Computes the minimum element of the tensor using `operator>` between two elements of type `T`.
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			const T& minElement() const;
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \brief Computes the maximum element of the tensor using `operator<` between two elements of type `T`.
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			const T& maxElement() const;
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \brief Returns the square of the norm of a tensor.
			/// The square of the norm of a tensor is the sum of the square of his elements.
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			T normSq() const;
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \brief Returns the norm of a tensor.
			/// The square of the norm of a tensor is the square root of the sum of the square of his elements.
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			T norm() const;

			~Tensor();

		protected:

			Tensor();
			void initSubTensor(T* values, uint64_t* shape, uint64_t length, uint64_t treeLength);

			void free();

			T* _values;				///< Contiguous C array of the elements of the tensor in row-major order.
			SubTensor* _tree;		///< Flattened tree of all possible sub-tensors (not only of order N-1).
			uint64_t* _shape;		///< Shape of the tensor.
			uint64_t _treeOffset;	///< Offset between two sub-tensors of order N-1 in `_tree`.
			uint64_t _length;		///< Total size of the tensor (product of the elements in `_shape`)
			bool _owner;			///< True if the tensor is not a sub-tensor.
			bool _moved;			///< True if moved. In this case, all the pointers are nullptr.

		friend class Tensor<T, N + 1>;
	};

	// External operators

	template<typename T, uint64_t N>
	Tensor<T, N> operator+(const Tensor<T, N>& a, const Tensor<T, N>& b);
	template<typename T, uint64_t N>
	Tensor<T, N>&& operator+(Tensor<T, N>&& a, const Tensor<T, N>& b);
	template<typename T, uint64_t N>
	Tensor<T, N>&& operator+(const Tensor<T, N>& a, Tensor<T, N>&& b);
	template<typename T, uint64_t N>
	Tensor<T, N>&& operator+(Tensor<T, N>&& a, Tensor<T, N>&& b);

	template<typename T, uint64_t N>
	Tensor<T, N> operator-(const Tensor<T, N>& a, const Tensor<T, N>& b);
	template<typename T, uint64_t N>
	Tensor<T, N>&& operator-(Tensor<T, N>&& a, const Tensor<T, N>& b);
	template<typename T, uint64_t N>
	Tensor<T, N>&& operator-(const Tensor<T, N>& a, Tensor<T, N>&& b);
	template<typename T, uint64_t N>
	Tensor<T, N>&& operator-(Tensor<T, N>&& a, Tensor<T, N>&& b);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \relates Tensor
	/// \brief Tensor product between `a` and `b`.
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T, uint64_t N, uint64_t M>
	Tensor<T, N + M> tensorProduct(const Tensor<T, N>& a, const Tensor<T, M>& b);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \relates Tensor
	/// \brief Hadamard product (aka element-wise product) of `a` and `b`.
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T, uint64_t N>
	Tensor<T, N> hadamardProduct(const Tensor<T, N>& a, const Tensor<T, N>& b);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \relates Tensor
	/// \brief Dot product between `a` and `b`.
	/// 
	/// The dot product is defined as the sum of the element-wise products. It is equivalent to the sum of the elements
	/// of the Hadamard product of `a` and `b`.
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T, uint64_t N>
	T dotProduct(const Tensor<T, N>& a, const Tensor<T, N>& b);

	template<typename T, uint64_t N>
	Tensor<T, N> operator*(const Tensor<T, N>& a, const T& x);
	template<typename T, uint64_t N>
	Tensor<T, N>&& operator*(Tensor<T, N>&& a, const T& x);
	template<typename T, uint64_t N>
	Tensor<T, N> operator*(const T& x, const Tensor<T, N>& a);
	template<typename T, uint64_t N>
	Tensor<T, N>&& operator*(const T& x, Tensor<T, N>&& a);

	template<typename T, uint64_t N>
	Tensor<T, N> operator/(const Tensor<T, N>& a, const T& x);
	template<typename T, uint64_t N>
	Tensor<T, N>&& operator/(Tensor<T, N>&& a, const T& x);

	template<typename T, uint64_t N>
	Tensor<T, N> operator-(const Tensor<T, N>& a);
	template<typename T, uint64_t N>
	Tensor<T, N>&& operator-(Tensor<T, N>&& a);

	template<typename T, uint64_t N>
	Tensor<T, N> operator+(const Tensor<T, N>& a);
	template<typename T, uint64_t N>
	Tensor<T, N>&& operator+(Tensor<T, N>&& a);

	// Comparators

	template<typename T, uint64_t N>
	bool operator==(const Tensor<T, N>& a, const Tensor<T, N>& b);
	template<typename T, uint64_t N>
	bool operator!=(const Tensor<T, N>& a, const Tensor<T, N>& b);

	// Specific functions

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \enum ConvolutionMethod
	/// \brief Enumeration of the different conventions for behaviour of convolution at borders.
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	enum class ConvolutionMethod
	{
		Zero,			///< When going beyond the border, the value kept is 0.
		Continuous,		///< When going beyond the border, the value at the border is kept.
		Periodic		///< When going beyond the border, the matrix/vector is repeated.
	};
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \relates Tensor
	/// \brief Convolution product of `a` by `kernel`. For behaviour at borders, see ConvolutionMethod
	/// 
	/// The convolution `kernel` is centered. It means the the coefficient at the "middle" of the kernel is the one
	/// that applies on `a[0][0]` to compute the result in (0, 0) in the case of a tensor of order 2 (matrix).
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T, uint64_t N>
	Tensor<T, N> convolution(const Tensor<T, N>& a, const Tensor<T, N>& kernel, ConvolutionMethod method = ConvolutionMethod::Periodic);
}

#include <SciPP/TensorT.hpp>
