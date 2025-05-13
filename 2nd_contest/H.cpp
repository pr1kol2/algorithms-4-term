#include <cmath>
#include <complex>
#include <iostream>
#include <numbers>
#include <vector>

using Complex = std::complex<double>;

static const double kPi = std::numbers::pi_v<double>;

std::vector<int> ReadPolynomial() {
  size_t degree = 0;
  std::cin >> degree;
  std::vector<int> coefficients(degree + 1);
  for (size_t i = 0; i <= degree; ++i) {
    std::cin >> coefficients[degree - i];
  }
  return coefficients;
}

void BitReversePermute(std::vector<Complex>& poly) {
  size_t length = poly.size();
  for (size_t i = 1, j = 0; i < length; ++i) {
    size_t half = length >> 1;
    for (; j & half; half >>= 1) {
      j ^= half;
    }
    j |= half;
    if (i < j) {
      std::swap(poly[i], poly[j]);
    }
  }
}

void PerformFFT(std::vector<Complex>& poly, bool invert = false) {
  size_t length = poly.size();
  BitReversePermute(poly);

  for (size_t block_size = 2; block_size <= length; block_size <<= 1) {
    double angle_increment = 2 * kPi / block_size * (invert ? -1 : 1);
    Complex w_root =
        Complex(std::cos(angle_increment), std::sin(angle_increment));
    for (size_t start = 0; start < length; start += block_size) {
      Complex w(1);
      for (size_t offset = 0; offset < block_size / 2; ++offset) {
        Complex even_val = poly[start + offset];
        Complex odd_val = poly[start + offset + block_size / 2] * w;
        poly[start + offset] = even_val + odd_val;
        poly[start + offset + block_size / 2] = even_val - odd_val;
        w *= w_root;
      }
    }
  }

  if (invert) {
    for (auto& value : poly) {
      value /= static_cast<double>(length);
    }
  }
}

size_t ComputeFFTSize(size_t targetSize) {
  size_t fft_size = 1;
  while (fft_size < targetSize) {
    fft_size <<= 1;
  }
  return fft_size;
}

void PointwiseMultiply(std::vector<Complex>& values_A,
                       const std::vector<Complex>& values_B) {
  size_t length = values_A.size();
  for (size_t i = 0; i < length; ++i) {
    values_A[i] *= values_B[i];
  }
}

std::vector<int> ConvertToIntCoefficients(const std::vector<Complex>& values,
                                          size_t resultSize) {
  std::vector<int> result(resultSize);
  for (size_t i = 0; i < resultSize; ++i) {
    result[i] = static_cast<int>(std::lround(values[i].real()));
  }
  return result;
}

// Умножает два многочлена через FFT
// Умножает два многочлена через FFT
std::vector<int> MultiplyPolynomials(const std::vector<int>& first_poly,
                                     const std::vector<int>& second_poly) {
  size_t size_A = first_poly.size();
  size_t size_B = second_poly.size();
  size_t target_size = size_A + size_B - 1;
  size_t fft_size = ComputeFFTSize(target_size);

  std::vector<Complex> values_A(fft_size), values_B(fft_size);
  for (size_t i = 0; i < size_A; ++i) {
    values_A[i] = Complex(first_poly[i], 0);
  }
  for (size_t i = 0; i < size_B; ++i) {
    values_B[i] = Complex(second_poly[i], 0);
  }

  PerformFFT(values_A, false);
  PerformFFT(values_B, false);
  PointwiseMultiply(values_A, values_B);
  PerformFFT(values_A, true);

  return ConvertToIntCoefficients(values_A, target_size);
}

int main() {
  auto poly_A = ReadPolynomial();
  auto poly_B = ReadPolynomial();
  auto product_coeffs = MultiplyPolynomials(poly_A, poly_B);

  size_t degree = product_coeffs.size() - 1;
  std::cout << degree;
  for (size_t i = 0; i < product_coeffs.size(); ++i) {
    std::cout << ' ' << product_coeffs[degree - i];
  }
  std::cout << '\n';
  return 0;
}
