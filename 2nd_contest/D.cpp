#include <iostream>
#include <vector>

constexpr int kModulo = 1e9 + 7;

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

int main() {
  size_t n = 0;
  size_t k = 0;
  std::cin >> n >> k;
  
  std::vector<int> sequence(n);
  for (size_t i = 0; i < n; ++i) {
    std::cin >> sequence[i];
  }

  Modular sequence_sum = 0;
  for (int a : sequence) {
    sequence_sum += Modular(a);
  }

  std::vector<Modular<>> factorial(k + 1);
  std::vector<Modular<>> inverse_factorial(k + 1);

  factorial[0] = 1;
  for (size_t i = 1; i <= k; ++i) {
    factorial[i] = factorial[i - 1] * Modular(i);
  }

  inverse_factorial[k] = factorial[k].Inverse();
  for (size_t i = k; i > 0; --i) {
    inverse_factorial[i - 1] = inverse_factorial[i] * Modular(i);
  }

  Modular intermediate_sum = 0;
  for (size_t j = 0; j <= k; ++j) {
    Modular c = factorial[k] * inverse_factorial[j] * inverse_factorial[k - j];

    if ((k - j) & 1) {
      c = Modular(0) - c;
    }

    Modular p = (n > 1 ? Modular(j).BinPow(n - 1) : Modular(1));
    Modular term = c * p * Modular(j + static_cast<int64_t>(n) - 1);
    intermediate_sum += term;
  }

  Modular stirling = intermediate_sum * inverse_factorial[k];
  Modular result = sequence_sum * stirling;

  std::cout << result << '\n';

  return 0;
}
