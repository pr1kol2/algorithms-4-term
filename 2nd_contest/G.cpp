#include <cstdint>
#include <iostream>
#include <vector>

constexpr int kModulo = 1e9 + 9;

template <int Modulo = kModulo>
struct Modular {
  int value;

  Modular(int64_t x = 0)
      : value(x >= 0 ? x % Modulo : Modulo - (-x % Modulo)) {}

  Modular BinPow(int64_t power) const {
    Modular result(1);
    Modular x(*this);
    while (power) {
      if (power & 1) {
        result *= x;
      }
      x *= x;
      power >>= 1;
    }
    return result;
  }

  Modular Inverse() const { return BinPow(Modulo - 2); }

  Modular& operator+=(const Modular& other) {
    value += other.value;
    if (value >= Modulo) {
      value -= Modulo;
    }
    return *this;
  }

  Modular& operator-=(const Modular& other) {
    value -= other.value;
    if (value < 0) {
      value += Modulo;
    }
    return *this;
  }

  Modular& operator*=(const Modular& other) {
    value = (static_cast<int64_t>(value) * static_cast<int64_t>(other.value)) %
            Modulo;
    return *this;
  }

  Modular& operator/=(const Modular& other) { return *this *= other.Inverse(); }

  friend Modular operator+(const Modular& lhs, const Modular& rhs) {
    Modular result(lhs);
    result += rhs;
    return result;
  }

  friend Modular operator-(const Modular& lhs, const Modular& rhs) {
    Modular result(lhs);
    result -= rhs;
    return result;
  }

  friend Modular operator*(const Modular& lhs, const Modular& rhs) {
    Modular result(lhs);
    result *= rhs;
    return result;
  }

  friend Modular operator/(const Modular& lhs, const Modular& rhs) {
    Modular result(lhs);
    result /= rhs;
    return result;
  }

  bool operator==(const Modular& other) const { return value == other.value; }

  bool operator==(int other) const { return value == other; }

  friend std::ostream& operator<<(std::ostream& stream, const Modular& value) {
    return stream << value.value;
  }
};

// fibonacci fast doubling
std::pair<Modular<>, Modular<>> GetFibonacciPair(int64_t n) {
  if (n == 0) {
    return {Modular(0), Modular(1)};
  }

  auto [f_k, f_k1] = GetFibonacciPair(n >> 1);

  Modular two(2);

  // F(2k) = F_k * (2*F(k+1) - F(k)
  Modular f_2k = f_k * (f_k1 * two - f_k);

  // F(2k+1) = F(k)^2 + F(k+1)^2
  Modular f_2k1 = f_k * f_k + f_k1 * f_k1;

  if ((n & 1) == 0) {
    return {f_2k, f_2k1};
  } else {
    return {f_2k1, f_2k + f_2k1};
  }
}

int main() {
  size_t n = 0;
  size_t q = 0;
  std::cin >> n >> q;

  std::vector<int64_t> x(n);
  std::vector<int64_t> y(n);
  std::vector<int64_t> k(n);

  for (size_t i = 0; i < n; ++i) {
    std::cin >> x[i] >> y[i] >> k[i];
    k[i] %= kModulo;
  }

  std::vector<int64_t> c(q);
  for (size_t i = 0; i < q; ++i) {
    std::cin >> c[i];
  }

  for (size_t i = 0; i < q; ++i) {
    Modular result = 0;
    int64_t c_j = c[i];

    for (int j = 0; j < n; ++j) {
      int64_t block_sum = y[j] - x[j] - c_j;
      if (block_sum < 0) {
        continue;
      }
      auto [fibonacci, _] = GetFibonacciPair(block_sum + 1);
      result += Modular(k[j]) * fibonacci;
    }

    std::cout << result << '\n';
  }

  return 0;
}
