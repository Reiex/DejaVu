#pragma once

#include <SciPP/Tensor.hpp>

namespace scp
{
	template<uint64_t N>
	template<typename T>
	TensorIterator<N>::TensorIterator(const Tensor<T, N>& a, bool end)
	{
		std::memcpy(_shape, a.shape(), sizeof(uint64_t) * N);
		std::memset(_pos.indices, 0, sizeof(uint64_t) * N);
		_pos.globalIndex = 0;

		if (end)
		{
			_pos.indices[0] = _shape[0];
			_pos.globalIndex = a.globalLength();
		}
	}

	template<uint64_t N>
	TensorIterator<N>::TensorIterator(const uint64_t* shape, const uint64_t* indices, uint64_t globalIndex)
	{
		std::memcpy(_shape, shape, sizeof(uint64_t) * N);
		std::memcpy(_pos.indices, indices, sizeof(uint64_t) * N);
		_pos.globalIndex = globalIndex;
	}

	template<uint64_t N>
	const TensorPosition<N>& TensorIterator<N>::operator*() const
	{
		return _pos;
	}

	template<uint64_t N>
	const TensorPosition<N>* TensorIterator<N>::operator->() const
	{
		return &_pos;
	}

	template<uint64_t N>
	TensorIterator<N>& TensorIterator<N>::operator++()
	{
		uint64_t i;
		for (i = N - 1; _pos.indices[i] == _shape[i] - 1 && i != 0; --i)
		{
			_pos.indices[i] = 0;
		}

		++_pos.indices[i];
		++_pos.globalIndex;

		return *this;
	}

	template<uint64_t N>
	TensorIterator<N> TensorIterator<N>::operator++(int)
	{
		TensorIterator<N> copy(_shape, _pos.indices, _pos.globalindex);

		operator++();

		return copy;
	}

	template<uint64_t N>
	bool TensorIterator<N>::operator==(const TensorIterator<N>& iterator) const
	{
		return !std::memcmp(this, &iterator, sizeof(TensorIterator<N>));
	}

	template<uint64_t N>
	bool TensorIterator<N>::operator!=(const TensorIterator<N>& iterator) const
	{
		return std::memcmp(this, &iterator, sizeof(TensorIterator<N>));
	}


	// Constructors

	template<typename T, uint64_t N>
	Tensor<T, N>::Tensor() :
		_values(nullptr),
		_tree(nullptr),
		_shape(nullptr),
		_treeOffset(1),
		_length(0),
		_owner(true),
		_moved(true)
	{
	}

	template<typename T, uint64_t N>
	void Tensor<T, N>::initSubTensor(T* values, uint64_t* shape, uint64_t length, uint64_t treeLength)
	{
		_values = values;
		if constexpr (N == 1)
		{
			_tree = values;
			_treeOffset = 1;
		}
		else
		{
			_tree = reinterpret_cast<SubTensor*>(this + 1);
			_treeOffset = treeLength / shape[0];

			uint64_t subLength = length / shape[0];
			for (uint64_t i = 0; i < shape[0]; ++i)
			{
				_tree[i*_treeOffset].initSubTensor(_values + i*subLength, shape + 1, subLength, _treeOffset - 1);
			}
		}
		_shape = shape;
		_length = length;
		_owner = false;
		_moved = false;
	}

	template<typename T, uint64_t N>
	Tensor<T, N>::Tensor(const uint64_t* shape, const T& x) : Tensor()
	{
		_shape = new uint64_t[N];
		std::memcpy(_shape, shape, sizeof(uint64_t) * N);

		_length = 1;
		uint64_t *it = _shape, *end = _shape + N;
		while (it != end)
		{
			_length *= *it;
			++it;
		}
		assert(_length != 0);

		_values = new T[_length];

		if constexpr (N == 1)
		{
			_tree = _values;
			_treeOffset = 1;
		}
		else
		{
			uint64_t treeLength = 0;
			it = _shape + N - 2;
			end = _shape - 1;
			while (it != end)
			{
				treeLength = (treeLength + 1) * *it;
				--it;
			}

			_tree = new SubTensor[treeLength];
			_treeOffset = treeLength / _shape[0];

			uint64_t subLength = _length / _shape[0];
			for (uint64_t i = 0; i < _shape[0]; ++i)
			{
				_tree[i*_treeOffset].initSubTensor(_values + i*subLength, _shape + 1, subLength, _treeOffset - 1);
			}
		}
		std::fill<T*, T>(_values, _values + _length, x);

		_owner = true;
		_moved = false;
	}

	template<typename T, uint64_t N>
	Tensor<T, N>::Tensor(const uint64_t* shape, int64_t x) : Tensor(shape, T(x))
	{
	}

	template<typename T, uint64_t N>
	Tensor<T, N>::Tensor(const std::vector<uint64_t>& shape, const T& x) : Tensor(shape.data(), x)
	{
		assert(shape.size() == N);
	}

	template<typename T, uint64_t N>
	Tensor<T, N>::Tensor(const std::vector<uint64_t>& shape, int64_t x) : Tensor(shape, T(x))
	{
	}
	
	template<typename T, uint64_t N>
	Tensor<T, N>::Tensor(const Tensor<T, N>& a) : Tensor()
	{
		assert(!a._moved);

		_shape = new uint64_t[N];
		std::memcpy(_shape, a._shape, sizeof(uint64_t) * N);

		_length = a._length;
		uint64_t treeLength = _length / _shape[N - 1];

		_values = new T[_length];
		if constexpr (N == 1)
		{
			_tree = _values;
			_treeOffset = 1;
		}
		else
		{
			uint64_t treeLength = 0;
			uint64_t *it = _shape + N - 2, *end = _shape - 1;
			while (it != end)
			{
				treeLength = (treeLength + 1) * *it;
				--it;
			}

			_tree = new SubTensor[treeLength];

			_treeOffset = treeLength / _shape[0];
			uint64_t subLength = _length / _shape[0];
			for (uint64_t i = 0; i < _shape[0]; ++i)
			{
				_tree[i*_treeOffset].initSubTensor(_values + i*subLength, _shape + 1, subLength, _treeOffset - 1);
			}
		}

		std::copy<T*, T*>(a._values, a._values + a._length, _values);

		_owner = true;
		_moved = false;
	}
	
	template<typename T, uint64_t N>
	Tensor<T, N>::Tensor(Tensor<T, N>&& a) :
		_values(a._values),
		_tree(a._tree),
		_shape(a._shape),
		_treeOffset(a._treeOffset),
		_length(a._length),
		_owner(true),
		_moved(false)
	{
		assert(!a._moved);
		assert(a._owner);

		a._values = nullptr;
		a._tree = nullptr;
		a._shape = nullptr;
		a._treeOffset = 1;
		a._length = 0;
		a._owner = true;
		a._moved = true;
	}


	// Assignment operators

	template<typename T, uint64_t N>
	Tensor<T, N>& Tensor<T, N>::operator=(const Tensor<T, N>& a)
	{
		assert(!_moved);
		assert(!a._moved);
		assert(!std::memcmp(_shape, a._shape, sizeof(uint64_t) * N));

		std::copy<T*, T*>(a._values, a._values + a._length, _values);

		return *this;
	}
	
	template<typename T, uint64_t N>
	Tensor<T, N>& Tensor<T, N>::operator=(Tensor<T, N>&& a)
	{
		assert(!_moved);
		assert(!a._moved);
		assert(!std::memcmp(_shape, a._shape, sizeof(uint64_t) * N));

		if (_owner)
		{
			free();

			_values = a._values;
			_tree = a._tree;
			_shape = a._shape;
			_treeOffset = a._treeOffset;
			_length = a._length;
			_owner = a._owner;
			_moved = false;

			a._values = nullptr;
			a._tree = nullptr;
			a._shape = nullptr;
			a._treeOffset = 1;
			a._length = 0;
			a._owner = true;
			a._moved = true;
		}
		else
		{
			std::copy<T*, T*>(a._values, a._values + a._length, _values);
		}

		return *this;
	}
	

	// Accessors

	template<typename T, uint64_t N>
	uint64_t Tensor<T, N>::globalLength() const
	{
		assert(!_moved);
		return _length;
	}

	template<typename T, uint64_t N>
	const uint64_t* Tensor<T, N>::shape() const
	{
		assert(!_moved);
		return _shape;
	}

	template<typename T, uint64_t N>
	uint64_t Tensor<T, N>::shape(uint64_t i) const
	{
		assert(!_moved);
		assert(i < N);
		return _shape[i];
	}

	template<typename T, uint64_t N>
	typename Tensor<T, N>::SubTensor& Tensor<T, N>::operator[](uint64_t i)
	{
		assert(!_moved);
		assert(i < _shape[0]);
		return _tree[i*_treeOffset];
	}
	
	template<typename T, uint64_t N>
	const typename Tensor<T, N>::SubTensor& Tensor<T, N>::operator[](uint64_t i) const
	{
		assert(!_moved);
		assert(i < _shape[0]);
		return _tree[i*_treeOffset];
	}

	template<typename T, uint64_t N>
	T& Tensor<T, N>::get(const uint64_t* indices)
	{
		assert(!_moved);
		assert(indices[0] < _shape[0]);
		uint64_t index = indices[0];
		for (uint64_t i = 1; i < N; ++i)
		{
			assert(indices[i] < _shape[i]);
			index = index * _shape[i - 1] + indices[i];
		}

		return _values[index];
	}

	template<typename T, uint64_t N>
	const T& Tensor<T, N>::get(const uint64_t* indices) const
	{
		assert(!_moved);
		assert(indices[0] < _shape[0]);
		uint64_t index = indices[0];
		for (uint64_t i = 1; i < N; ++i)
		{
			assert(indices[i] < _shape[i]);
			index = index * _shape[i - 1] + indices[i];
		}

		return _values[index];
	}

	template<typename T, uint64_t N>
	T& Tensor<T, N>::get(uint64_t globalIndex)
	{
		assert(!_moved);
		assert(globalIndex < _length);

		return _values[globalIndex];
	}

	template<typename T, uint64_t N>
	const T& Tensor<T, N>::get(uint64_t globalIndex) const
	{
		assert(!_moved);
		assert(globalIndex < _length);

		return _values[globalIndex];
	}

	template<typename T, uint64_t N>
	TensorIterator<N> Tensor<T, N>::begin() const
	{
		assert(!_moved);
		return TensorIterator<N>(*this);
	}

	template<typename T, uint64_t N>
	TensorIterator<N> Tensor<T, N>::end() const
	{
		assert(!_moved);
		return TensorIterator<N>(*this, true);
	}


	// Internal operators

	template<typename T, uint64_t N>
	Tensor<T, N>& Tensor<T, N>::operator+=(const Tensor<T, N>& a)
	{
		assert(!_moved);
		assert(!a._moved);
		assert(!std::memcmp(_shape, a._shape, sizeof(uint64_t) * N));

		for (uint64_t i = 0; i < _length; ++i)
		{
			_values[i] += a._values[i];
		}

		return *this;
	}
	
	template<typename T, uint64_t N>
	Tensor<T, N>& Tensor<T, N>::operator-=(const Tensor<T, N>& a)
	{
		assert(!_moved);
		assert(!a._moved);
		assert(!std::memcmp(_shape, a._shape, sizeof(uint64_t) * N));

		for (uint64_t i = 0; i < _length; ++i)
		{
			_values[i] -= a._values[i];
		}

		return *this;
	}
	
	template<typename T, uint64_t N>
	Tensor<T, N>& Tensor<T, N>::operator*=(T x)
	{
		assert(!_moved);

		for (uint64_t i = 0; i < _length; ++i)
		{
			_values[i] *= x;
		}

		return *this;
	}
	
	template<typename T, uint64_t N>
	Tensor<T, N>& Tensor<T, N>::operator/=(T x)
	{
		assert(!_moved);

		for (uint64_t i = 0; i < _length; ++i)
		{
			_values[i] /= x;
		}

		return *this;
	}


	// Specific methods

	template<typename T, uint64_t N>
	const T& Tensor<T, N>::minElement() const
	{
		T* minElt = _values;
		for (uint64_t i = 1; i < _length; ++i)
		{
			if (*minElt > _values[i])
			{
				minElt = _values + i;
			}
		}

		return *minElt;
	}

	template<typename T, uint64_t N>
	const T& Tensor<T, N>::maxElement() const
	{
		T* maxElt = _values;
		for (uint64_t i = 1; i < _length; ++i)
		{
			if (*maxElt < _values[i])
			{
				maxElt = _values + i;
			}
		}

		return *maxElt;
	}

	template<typename T, uint64_t N>
	T Tensor<T, N>::normSq() const
	{
		T result(0);
		for (uint64_t i = 0; i < _length; ++i)
		{
			result += _values[i] * _values[i];
		}

		return result;
	}

	template<typename T, uint64_t N>
	T Tensor<T, N>::norm() const
	{
		return std::sqrt(normSq());
	}


	// Destructors

	template<typename T, uint64_t N>
	void Tensor<T, N>::free()
	{
		if (!_moved)
		{
			if (_owner)
			{
				delete[] _values;
				if constexpr (N != 1)
				{
					delete[] _tree;
				}
				delete[] _shape;
			}

			_values = nullptr;
			_tree = nullptr;
			_shape = nullptr;
		}

		_treeOffset = 1;
		_length = 1;
		_owner = true;
		_moved = true;
	}

	template<typename T, uint64_t N>
	Tensor<T, N>::~Tensor()
	{
		free();
	}


	// External operators

	template<typename T, uint64_t N>
	Tensor<T, N> operator+(const Tensor<T, N>& a, const Tensor<T, N>& b)
	{
		Tensor<T, N> c(a);
		c += b;

		return c;
	}

	template<typename T, uint64_t N>
	Tensor<T, N>&& operator+(Tensor<T, N>&& a, const Tensor<T, N>& b)
	{
		a += b;
		return std::move(a);
	}

	template<typename T, uint64_t N>
	Tensor<T, N>&& operator+(const Tensor<T, N>& a, Tensor<T, N>&& b)
	{
		b += a;
		return std::move(b);
	}

	template<typename T, uint64_t N>
	Tensor<T, N>&& operator+(Tensor<T, N>&& a, Tensor<T, N>&& b)
	{
		a += b;
		return std::move(a);
	}


	template<typename T, uint64_t N>
	Tensor<T, N> operator-(const Tensor<T, N>& a, const Tensor<T, N>& b)
	{
		Tensor<T, N> c(a);
		c -= b;

		return c;
	}

	template<typename T, uint64_t N>
	Tensor<T, N>&& operator-(Tensor<T, N>&& a, const Tensor<T, N>& b)
	{
		a -= b;
		return std::move(a);
	}

	template<typename T, uint64_t N>
	Tensor<T, N>&& operator-(const Tensor<T, N>& a, Tensor<T, N>&& b)
	{
		b -= a;
		return -std::move(b);
	}

	template<typename T, uint64_t N>
	Tensor<T, N>&& operator-(Tensor<T, N>&& a, Tensor<T, N>&& b)
	{
		a -= b;
		return std::move(a);
	}


	template<typename T, uint64_t N, uint64_t M>
	Tensor<T, N + M> tensorProduct(const Tensor<T, N>& a, const Tensor<T, M>& b)
	{
		uint64_t shape[N + M];
		std::memcpy(shape, a.shape(), sizeof(uint64_t) * N);
		std::memcpy(shape + N, b.shape(), sizeof(uint64_t) * M);

		Tensor<T, N + M> c(shape);
		for (const TensorPosition<N + M>& pos : c)
		{
			c.get(pos.indices) = a.get(pos.indices) * b.get(pos.indices + N);
		}

		return c;
	}

	template<typename T, uint64_t N>
	Tensor<T, N> hadamardProduct(const Tensor<T, N>& a, const Tensor<T, N>& b)
	{
		assert(!std::memcmp(a.shape(), b.shape(), sizeof(uint64_t) * N));

		Tensor<T, N> c(a.shape());

		for (const TensorPosition<N>& pos : c)
		{
			c.get(pos.globalIndex) = a.get(pos.globalIndex) * b.get(pos.globalIndex);
		}

		return c;
	}

	template<typename T, uint64_t N>
	T dotProduct(const Tensor<T, N>& a, const Tensor<T, N>& b)
	{
		assert(!std::memcmp(a.shape(), b.shape(), sizeof(uint64_t) * N));

		T result(0);
		for (const TensorPosition<N>& pos : a)
		{
			result += a.get(pos.globalIndex) * b.get(pos.globalIndex);
		}

		return result;
	}


	template<typename T, uint64_t N>
	Tensor<T, N> operator*(const Tensor<T, N>& a, const T& x)
	{
		Tensor<T, N> b(a);
		b *= x;

		return b;
	}

	template<typename T, uint64_t N>
	Tensor<T, N>&& operator*(Tensor<T, N>&& a, const T& x)
	{
		a *= x;
		return std::move(a);
	}

	template<typename T, uint64_t N>
	Tensor<T, N> operator*(const T& x, const Tensor<T, N>& a)
	{
		Tensor<T, N> b(a);
		b *= x;

		return b;
	}

	template<typename T, uint64_t N>
	Tensor<T, N>&& operator*(const T& x, Tensor<T, N>&& a)
	{
		a *= x;
		return std::move(a);
	}


	template<typename T, uint64_t N>
	Tensor<T, N> operator/(const Tensor<T, N>& a, const T& x)
	{
		Tensor<T, N> b(a);
		b /= x;

		return b;
	}

	template<typename T, uint64_t N>
	Tensor<T, N>&& operator/(Tensor<T, N>&& a, const T& x)
	{
		a /= x;
		return std::move(a);
	}


	template<typename T, uint64_t N>
	Tensor<T, N> operator-(const Tensor<T, N>& a)
	{
		Tensor<T, N> b(a.shape());
		for (const TensorPosition<N>& pos : b)
		{
			b.get(pos.globalIndex) = -a.get(pos.globalIndex);
		}

		return b;
	}

	template<typename T, uint64_t N>
	Tensor<T, N>&& operator-(Tensor<T, N>&& a)
	{
		for (const TensorPosition<N>& pos : a)
		{
			a.get(pos.globalIndex) = -a.get(pos.globalIndex);
		}

		return std::move(a);
	}


	template<typename T, uint64_t N>
	Tensor<T, N> operator+(const Tensor<T, N>& a)
	{
		return a;
	}

	template<typename T, uint64_t N>
	Tensor<T, N>&& operator+(Tensor<T, N>&& a)
	{
		return std::move(a);
	}


	// Comparators

	template<typename T, uint64_t N>
	bool operator==(const Tensor<T, N>& a, const Tensor<T, N>& b)
	{
		if (std::memcmp(a.shape(), b.shape(), sizeof(uint64_t) * N))
		{
			return false;
		}

		for (const TensorPosition<N>& pos : a)
		{
			if (a.get(pos.globalIndex) != b.get(pos.globalIndex))
			{
				return false;
			}
		}

		return true;
	}

	template<typename T, uint64_t N>
	bool operator!=(const Tensor<T, N>& a, const Tensor<T, N>& b)
	{
		return !(a == b);
	}


	// Specific functions

	template<typename T, uint64_t N>
	Tensor<T, N> convolution(const Tensor<T, N>& a, const Tensor<T, N>& kernel, ConvolutionMethod method)
	{
		for (uint64_t i = 0; i < N; i++)
		{
			assert(kernel.shape(i) % 2 == 1);
			assert(kernel.shape(i) <= a.shape(i));
		}

		Tensor<T, N> b(a.shape());

		uint64_t offset[N];
		for (uint64_t i = 0; i < N; i++)
		{
			offset[i] = kernel.shape(i) / 2;
		}

		for (const TensorPosition<N>& pos : a)
		{
			b.get(pos.globalIndex) = T(0);

			for (const TensorPosition<N>& kernelPos : kernel)
			{
				int64_t offsetedIndices[N];
				bool setToZero(false);
				for (uint64_t i = 0; i < N; i++)
				{
					offsetedIndices[i] = int64_t(pos.indices[i]) + int64_t(kernelPos.indices[i]) - int64_t(offset[i]);

					switch (method)
					{
						case ConvolutionMethod::Zero:
							setToZero = (offsetedIndices[i] < 0 || offsetedIndices[i] >= a.shape(i));
							break;
						case ConvolutionMethod::Continuous:
							offsetedIndices[i] = std::clamp<int64_t>(offsetedIndices[i], 0, a.shape(i) - 1);
							break;
						case ConvolutionMethod::Periodic:
							offsetedIndices[i] = (offsetedIndices[i] + a.shape(i)) % a.shape(i);
							break;
						default:
							assert(false);
					}

					if (setToZero)
					{
						break;
					}
				}

				if (!setToZero)
				{
					b.get(pos.globalIndex) += a.get(reinterpret_cast<uint64_t*>(offsetedIndices)) * kernel.get(kernelPos.globalIndex);
				}
			}
		}

		return b;
	}
}