#pragma once

#include <SciPP/Mat.hpp>

namespace scp
{
	// Constructors

	template<typename T>
	Mat<T>::Mat(const Tensor<T, 2>& tensor) : Tensor<T, 2>(tensor)
	{
	}

	template<typename T>
	Mat<T>::Mat(Tensor<T, 2>&& tensor) : Tensor<T, 2>(std::move(tensor))
	{
	}

	template<typename T>
	Mat<T>::Mat(uint64_t row, uint64_t col, int64_t x) : Tensor<T, 2>({ row, col }, x)
	{
	}
	
	template<typename T>
	Mat<T>::Mat(uint64_t row, uint64_t col, const T& x) : Tensor<T, 2>({ row, col }, x)
	{
	}
	
	template<typename T>
	Mat<T>::Mat(const std::vector<std::vector<T>>& values) : Tensor<T, 2>({ values.size(), values.size() == 0 ? 0 : values[0].size() })
	{
		for (uint64_t i(0); i < this->shape(0); i++)
		{
			assert(values[i].size() == this->shape(1));
			for (uint64_t j(0); j < this->shape(1); j++)
			{
				(*this)[i][j] = values[i][j];
			}
		}
	}

	template<typename T>
	Mat<T> Mat<T>::identity(uint64_t size, int64_t x)
	{
		Mat<T> a(size, size);
		for (uint64_t i(0); i < size; i++)
			a[i][i] = x;

		return a;
	}

	template<typename T>
	Mat<T> Mat<T>::identity(uint64_t size, const T& x)
	{
		Mat<T> a(size, size);
		for (uint64_t i(0); i < size; i++)
			a[i][i] = x;

		return a;
	}


	// Assignment operators

	template<typename T>
	Mat<T>& Mat<T>::operator=(const Mat<T>& a)
	{
		Tensor<T, 2>::operator=(a);
		return *this;
	}

	template<typename T>
	Mat<T>& Mat<T>::operator=(Mat<T>&& a)
	{
		Tensor<T, 2>::operator=(std::move(a));
		return *this;
	}


	// Accessors

	template<typename T>
	Vec<T>& Mat<T>::operator[](uint64_t i)
	{
		return static_cast<Vec<T>&>(Tensor<T, 2>::operator[](i));
	}

	template<typename T>
	const Vec<T>& Mat<T>::operator[](uint64_t i) const
	{
		return static_cast<const Vec<T>&>(Tensor<T, 2>::operator[](i));
	}


	// External operators

	template<typename T>
	Mat<T> operator*(const Mat<T>& a, const Mat<T>& b)
	{
		assert(a.shape(1) == b.shape(0));

		Mat<T> c(a.shape(0), b.shape(1));
		for (uint64_t i(0); i < c.shape(0); i++)
			for (uint64_t j(0); j < c.shape(1); j++)
				for (uint64_t k(0); k < a.shape(1); k++)
					c[i][j] += a[i][k]*b[k][j];
		
		return c;
	}
	

	// Display

	template<typename T>
	std::ostream& operator<<(std::ostream& stream, const Mat<T>& a)
	{
		for (uint64_t i(0); i < a.shape(0); i++)
		{
			for (uint64_t j(0); j < a.shape(1); j++)
				if (j == a.shape(1) - 1)
					stream << a[i][j];
				else
					stream << a[i][j] << " ";
			if (i != a.shape(0) - 1)
				stream << std::endl;
		}

		return stream;
	}
	

	// Specific functions

	template<typename T>
	Mat<T> inverse(const Mat<T>& a)
	{
		assert(a.shape(0) == a.shape(1));

		Mat<T> b(a), inv(Mat<T>::identity(a.shape(0)));

		for (uint64_t j(0); j < a.shape(1); j++)
		{
			if (b[j][j] == 0)
			{
				for (uint64_t i(j + 1); i < a.shape(0); i++)
				{
					if (b[i][j] != 0)
					{
						inv[i] /= b[i][j];
						b[i] /= b[i][j];
						inv[j] += inv[i];
						b[j] += b[i];
						break;
					}
				}

				if (b[j][j] == 0)
					throw std::runtime_error("The matrix cannot be inverted.");
			}
			else
			{
				inv[j] /= b[j][j];
				b[j] /= b[j][j];
			}

			for (uint64_t i(j + 1); i < a.shape(0); i++)
			{
				if (b[i][j] != 0)
				{
					inv[i] -= b[i][j] * inv[j];
					b[i] -= b[i][j] * b[j];
				}
			}
		}

		for (uint64_t j(a.shape(1) - 1); j != UINT64_MAX; j--)
		{
			for (uint64_t i(j - 1); i != UINT64_MAX; i--)
			{
				if (b[i][j] != 0)
				{
					inv[i] -= b[i][j] * inv[j];
					b[i] -= b[i][j] * b[j];
				}
			}
		}

		return inv;
	}

	template<typename T>
	Mat<T> transpose(const Mat<T>& a)
	{
		Mat<T> b(a.shape(1), a.shape(0));
		for (uint64_t i(0); i < a.shape(1); i++)
			for (uint64_t j(0); j < a.shape(0); j++)
				b[i][j] = a[j][i];

		return b;
	}

	template<typename T>
	T det(const Mat<T>& a)
	{
		assert(a.shape(0) == a.shape(1));

		Mat<T> b(a);

		for (uint64_t j(0); j < a.shape(1); j++)
		{
			if (b[j][j] == 0)
			{
				for (uint64_t i(j + 1); i < a.shape(1); i++)
				{
					if (b[i][j] != 0)
					{
						b[j] += b[i];
						break;
					}
				}

				if (b[j][j] == 0)
					return 0;
			}

			for (uint64_t i(j + 1); i < a.shape(1); i++)
				b[i] -= (b[i][j] / b[j][j]) * b[j];
		}

		T prod(b[0][0]);
		for (uint64_t i(1); i < a.shape(1); i++)
			prod *= b[i][i];

		return prod;
	}


	template<typename T>
	Mat<std::complex<T>> dft(const Mat<std::complex<T>>& f)
	{
		Mat<std::complex<T>> wM(f.shape(0), f.shape(0));
		Mat<std::complex<T>> wN(f.shape(1), f.shape(1));

		for (uint64_t i(0); i < f.shape(0); i++)
			for (uint64_t j(0); j < f.shape(0); j++)
				wM[i][j] = std::exp(std::complex<T>(0, -2 * pi * i * j / f.shape(0)));

		for (uint64_t i(0); i < f.shape(1); i++)
			for (uint64_t j(0); j < f.shape(1); j++)
				wN[i][j] = std::exp(std::complex<T>(0, -2 * pi * i * j / f.shape(1)));

		return wM * f * wN;
	}

	template<typename T>
	Mat<std::complex<T>> idft(const Mat<std::complex<T>>& fh)
	{
		Mat<std::complex<T>> wM(fh.shape(0), fh.shape(0));
		Mat<std::complex<T>> wN(fh.shape(1), fh.shape(1));

		for (uint64_t i(0); i < fh.shape(0); i++)
			for (uint64_t j(0); j < fh.shape(0); j++)
				wM[i][j] = std::exp(std::complex<T>(0, 2 * pi * i * j / fh.shape(0)));

		for (uint64_t i(0); i < fh.shape(1); i++)
			for (uint64_t j(0); j < fh.shape(1); j++)
				wN[i][j] = std::exp(std::complex<T>(0, 2 * pi * i * j / fh.shape(1)));

		return wM * fh * wN / std::complex<T>(fh.shape(0) * fh.shape(1), 0);
	}

	template<typename T>
	Mat<std::complex<T>> fft(const Mat<std::complex<T>>& f)
	{
		if (f.shape(0) == 1 && f.shape(1) == 1)
			return f;

		Mat<std::complex<T>> EE(f.shape(0)/2, f.shape(1)/2), OE(f.shape(0)/2, f.shape(1)/2), EO(f.shape(0)/2, f.shape(1)/2), OO(f.shape(0)/2, f.shape(1)/2), fh(f.shape(0), f.shape(1));

		for (uint64_t i(0); i < EE.shape(0); i++)
		{
			for (uint64_t j(0); j < EE.shape(1); j++)
			{
				EE[i][j] = f[2*i][2*j];
				OE[i][j] = f[2*i+1][2*j];
				EO[i][j] = f[2*i][2*j+1];
				OO[i][j] = f[2*i+1][2*j+1];
			}
		}

		EE = fft(EE);
		OE = fft(OE);
		EO = fft(EO);
		OO = fft(OO);

		for (uint64_t i(0); i < EE.shape(0); i++)
		{
			for (uint64_t j(0); j < EE.shape(1); j++)
			{
				std::complex<T> e1 = std::exp(std::complex<T>(0, -2*pi*i/f.shape(0)));
				std::complex<T> e2 = std::exp(std::complex<T>(0, -2*pi*j/f.shape(1)));

				fh[i][j] = EE[i][j] + e1*OE[i][j] + e2*EO[i][j] + e1*e2*OO[i][j];
				fh[i+EE.shape(0)][j] = EE[i][j] - e1*OE[i][j] + e2*EO[i][j] - e1*e2*OO[i][j];
				fh[i][j+EE.shape(1)] = EE[i][j] + e1*OE[i][j] - e2*EO[i][j] - e1*e2*OO[i][j];
				fh[i+EE.shape(0)][j+EE.shape(1)] = EE[i][j] - e1*OE[i][j] - e2*EO[i][j] + e1*e2*OO[i][j];
			}
		}

		return fh;
	}
	
	template<typename T>
	Mat<std::complex<T>> ifft(const Mat<std::complex<T>>& fh)
	{
		if (fh.shape(0) == 1 && fh.shape(1) == 1)
			return fh;

		Mat<std::complex<T>> EE(fh.shape(0)/2, fh.shape(1)/2), OE(fh.shape(0)/2, fh.shape(1)/2), EO(fh.shape(0)/2, fh.shape(1)/2), OO(fh.shape(0)/2, fh.shape(1)/2), f(fh.shape(0), fh.shape(1));

		for (uint64_t i(0); i < EE.shape(0); i++)
		{
			for (uint64_t j(0); j < EE.shape(1); j++)
			{
				EE[i][j] = fh[2*i][2*j];
				OE[i][j] = fh[2*i+1][2*j];
				EO[i][j] = fh[2*i][2*j+1];
				OO[i][j] = fh[2*i+1][2*j+1];
			}
		}

		EE = ifft(EE);
		OE = ifft(OE);
		EO = ifft(EO);
		OO = ifft(OO);

		for (uint64_t i(0); i < EE.shape(0); i++)
		{
			for (uint64_t j(0); j < EE.shape(1); j++)
			{
				std::complex<T> e1 = std::exp(std::complex<T>(0, 2*pi*i/fh.shape(0)));
				std::complex<T> e2 = std::exp(std::complex<T>(0, 2*pi*j/fh.shape(1)));

				f[i][j] = EE[i][j] + e1*OE[i][j] + e2*EO[i][j] + e1*e2*OO[i][j];
				f[i+EE.shape(0)][j] = EE[i][j] - e1*OE[i][j] + e2*EO[i][j] - e1*e2*OO[i][j];
				f[i][j+EE.shape(1)] = EE[i][j] + e1*OE[i][j] - e2*EO[i][j] - e1*e2*OO[i][j];
				f[i+EE.shape(0)][j+EE.shape(1)] = EE[i][j] - e1*OE[i][j] - e2*EO[i][j] + e1*e2*OO[i][j];
			}
		}

		return f / std::complex<T>(4, 0);
	}


	namespace
	{
		template<typename T>
		T dct2DBase(uint64_t M, uint64_t N, uint64_t p, uint64_t q, uint64_t m, uint64_t n)
		{
			T r(2 * std::cos(pi * p * (m + 0.5) / M) * std::cos(pi * q * (n + 0.5) / N));

			if (p == 0)
				r /= std::sqrt(2.0L);

			if (q == 0)
				r /= std::sqrt(2.0L);

			return r;
		}
	}
	
	template<typename T>
	Mat<T> dct(const Mat<T>& f)
	{
		Mat<T> wM(f.shape(0), f.shape(0));
		Mat<T> wN(f.shape(1), f.shape(1));

		for (uint64_t i(0); i < f.shape(0); i++)
			for (uint64_t j(0); j < f.shape(0); j++)
				if (i == 0)
					wM[i][j] = 1.L;
				else
					wM[i][j] = std::sqrt(2.f)*cos(pi*i*(j + 0.5)/f.shape(0));

		for (uint64_t i(0); i < f.shape(1); i++)
			for (uint64_t j(0); j < f.shape(1); j++)
				if (j == 0)
					wN[i][j] = 1.L;
				else
					wN[i][j] = std::sqrt(2.f)*cos(pi*j*(i + 0.5)/f.shape(1));

		return wM * f * wN;
	}
	
	template<typename T>
	Mat<T> idct(const Mat<T>& fh)
	{
		Mat<T> wM(fh.shape(0), fh.shape(0));
		Mat<T> wN(fh.shape(1), fh.shape(1));

		for (uint64_t i(0); i < fh.shape(0); i++)
			for (uint64_t j(0); j < fh.shape(0); j++)
				if (j == 0)
					wM[i][j] = 1.L;
				else
					wM[i][j] = std::sqrt(2.f)*cos(pi*j*(i + 0.5)/fh.shape(0));

		for (uint64_t i(0); i < fh.shape(1); i++)
			for (uint64_t j(0); j < fh.shape(1); j++)
				if (i == 0)
					wN[i][j] = 1.L / (fh.shape(0)*fh.shape(1));
				else
					wN[i][j] = std::sqrt(2.f)*cos(pi*i*(j + 0.5)/fh.shape(1)) / (fh.shape(0)*fh.shape(1));

		return wM * fh * wN;
	}
}
