#pragma once

#include <SciPP/Vec.hpp>

namespace scp
{
	// Constructors

	template<typename T>
	Vec<T>::Vec(const Tensor<T, 1>& tensor) : Tensor<T, 1>(tensor)
	{
	}

	template<typename T>
	Vec<T>::Vec(Tensor<T, 1>&& tensor) : Tensor<T, 1>(tensor)
	{
	}

	template<typename T>
	Vec<T>::Vec(uint64_t count, int64_t value) : Tensor<T, 1>({ count }, value)
	{
	}

	template<typename T>
	Vec<T>::Vec(uint64_t count, const T& value) : Tensor<T, 1>({ count }, value)
	{
	}

	template<typename T>
	Vec<T>::Vec(const std::vector<T>& values) : Tensor<T, 1>({ values.size() })
	{
		for (uint64_t i = 0; i < values.size(); i++)
		{
			(*this)[i] = values[i];
		}
	}


	// Assignment operators

	template<typename T>
	Vec<T>& Vec<T>::operator=(const Vec<T>& a)
	{
		Tensor<T, 1>::operator=(a);
		return *this;
	}

	template<typename T>
	Vec<T>& Vec<T>::operator=(Vec<T>&& a)
	{
		Tensor<T, 1>::operator=(std::move(a));
		return *this;
	}


	// External operators

	template<typename T>
	Vec<T> operator*(const Mat<T>& a, const Vec<T>& b)
	{
		assert(a.shape(1) == b.shape(0));

		Vec<T> c(a.shape(0));

		for (uint64_t i(0); i < a.shape(0); i++)
			c[i] = dotProduct(a[i], b);

		return c;
	}

	template<typename T>
	Vec<T> operator*(const Vec<T>& a, const Mat<T>& b)
	{
		assert(a.shape(0) == b.shape(0));

		Vec<T> c(b.shape(1));

		for (uint64_t i(0); i < b.shape(1); i++)
			for (uint64_t j(0); j < b.shape(0); j++)
				c[i] += b[j][i] * a[j];

		return c;
	}



	// Display
	
	template<typename T>
	std::ostream& operator<<(std::ostream& stream, const Vec<T>& v)
	{
		for (uint64_t i(0); i < v.shape(0); i++)
			if (i == v.shape(0) - 1)
				stream << v[i];
			else
				stream << v[i] << " ";

		return stream;
	}


	// Specific functions
	
	template<typename T>
	Vec<T> crossProduct(const Vec<T>& u, const Vec<T>& v)
	{
		assert(u.shape(0) == 3 && v.shape(0) == 3);
		return Vec<T>({u[1]*v[2] - u[2]*v[1], u[2]*v[0] - u[0]*v[2], u[0]*v[1] - u[1]*v[0]});
	}


	template<typename T>
	Vec<std::complex<T>> dft(const Vec<std::complex<T>>& f)
	{
		Vec<std::complex<T>> fh(f.shape(0));

		for (uint64_t k(0); k < f.shape(0); k++)
			for (uint64_t i(0); i < f.shape(0); i++)
				fh[k] += f[i] * std::exp(std::complex<T>(0, -2 * pi * k * i / f.shape(0)));

		return fh;
	}
	
	template<typename T>
	Vec<std::complex<T>> idft(const Vec<std::complex<T>>& fh)
	{
		Vec<std::complex<T>> f(fh.shape(0));

		for (uint64_t i(0); i < fh.shape(0); i++)
			f[i] = std::conj(fh[i]);

		f = dft(f);

		for (uint64_t i(0); i < fh.shape(0); i++)
			f[i] = std::conj(f[i]);

		return f / std::complex<T>(fh.shape(0));
	}

	template<typename T>
	Vec<std::complex<T>> fft(const Vec<std::complex<T>>& f)
	{
		if (f.shape(0) == 1)
			return f;

		Vec<std::complex<T>> even(f.shape(0) / 2), odd(f.shape(0) / 2), fh(f.shape(0));

		for (uint64_t k(0); k < even.shape(0); k++)
		{
			even[k] = f[2*k];
			odd[k] = f[2*k + 1];
		}

		even = fft(even);
		odd = fft(odd);

		for (uint64_t k(0); k < even.shape(0); k++)
		{
			fh[k] = even[k] + std::exp(std::complex<T>(0, -2 * pi * k / f.shape(0))) * odd[k];
			fh[k + even.shape(0)] = even[k] - std::exp(std::complex<T>(0, -2 * pi * k / f.shape(0))) * odd[k];
		}

		return fh;
	}

	template<typename T>
	Vec<std::complex<T>> ifft(const Vec<std::complex<T>>& fh)
	{
		Vec<std::complex<T>> f(fh.shape(0));

		for (uint64_t i(0); i < fh.shape(0); i++)
			f[i] = std::conj(fh[i]);

		f = fft(f);

		for (uint64_t i(0); i < fh.shape(0); i++)
			f[i] = std::conj(f[i]);

		return f / std::complex<T>(fh.shape(0));
	}


	namespace
	{
		template<typename T>
		T dctBase(uint64_t n, uint64_t k, uint64_t i)
		{
			if (k == 0)
				return std::cos(pi * (i + 0.5L) * k / n) / std::sqrt(n);
			else
				return std::sqrt(2.0 / n) * std::cos(pi * (i + 0.5) * k / n);
		}
	}
	
	template<typename T>
	Vec<T> dct(const Vec<T>& f)
	{
		Vec<T> fh(f.shape(0));

		for (uint64_t k(0); k < f.shape(0); k++)
			for (uint64_t i(0); i < f.shape(0); i++)
				fh[k] += f[i] * dctBase<T>(f.shape(0), k, i);

		return fh;
	}
	
	template<typename T>
	Vec<T> idct(const Vec<T>& fh)
	{
		Vec<T> f(fh.shape(0));

		for (uint64_t k(0); k < fh.shape(0); k++)
			for (uint64_t i(0); i < fh.shape(0); i++)
				f[k] += fh[i] * dctBase<T>(fh.shape(0), i, k);

		return f;
	}
}
