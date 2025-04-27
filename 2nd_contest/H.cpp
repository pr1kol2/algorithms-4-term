#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <numbers>

using Complex = std::complex<double>;

const double kPi = std::numbers::pi_v<double>;

void Fft(std::vector<Complex>& a, bool invert) {
  int n = static_cast<int>(a.size());

  for (int i = 1, j = 0; i < n; ++i) {
    int bit = n >> 1;
    for (; j & bit; bit >>= 1) {
      j ^= bit;
    }
    j |= bit;
    if (i < j) {
      std::swap(a[i], a[j]);
    }
  }

  for (int length = 2; length <= n; length <<= 1) {
    double angle = 2 * kPi / length * (invert ? -1 : 1);
    Complex w_length = Complex(std::cos(angle), std::sin(angle));
    for (int i = 0; i < n; i += length) {
      Complex w = 1;
      for (int j = 0; j < length / 2; ++j) {
        Complex u = a[i + j];
        Complex v = a[i + j + length / 2] * w;
        a[i + j] = u + v;
        a[i + j + length / 2] = u - v;
        w *= w_length;
      }
    }
  }

  if (invert) {
    for (Complex& x : a) {
      x /= n;
    }
  }
}

std::vector<int> MultiplyPolynomials(const std::vector<int>& A,
                                           const std::vector<int>& B) {
  size_t n = A.size();
  size_t m = B.size();
  size_t result_size = n + m - 1;

  size_t fft_size = 1;
  while (fft_size < result_size) {
    fft_size <<= 1;
  }

  std::vector<Complex> A_values(fft_size);
  std::vector<Complex> B_values(fft_size);

  for (size_t i = 0; i < n; ++i) {
    A_values[i] = Complex(A[i], 0);
  }
  for (size_t i = 0; i < m; ++i) {
    B_values[i] = Complex(B[i], 0);
  }

  Fft(A_values, false);
  Fft(B_values, false);

  for (size_t i = 0; i < fft_size; ++i) {
    A_values[i] *= B_values[i];
  }

  Fft(A_values, true);

  std::vector<int> result(result_size);
  for (size_t i = 0; i < result_size; ++i) {
    result[i] = static_cast<int>(std::lround(A_values[i].real()));
  }
  return result;
}

int main() {
  size_t n = 0;
  std::cin >> n;
  std::vector<int> A(n + 1);
  for (size_t i = 0; i <= n; ++i) {
    std::cin >> A[n - i];
  }

  size_t m = 0;
  std::cin >> m;
  std::vector<int> B(m + 1);
  for (size_t i = 0; i <= m; ++i) {
    std::cin >> B[m - i];
  }

  std::vector<int> C = MultiplyPolynomials(A, B);

  std::cout << C.size() - 1;
  for (size_t i = 0; i < C.size(); ++i) {
    std::cout << ' ' << C[C.size() - 1 - i];
  }
  std::cout << '\n';

  return 0;
}
