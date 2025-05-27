#include <iostream>
#include <cstddef>
#include <cassert>

constexpr static int kModulo = 1e9 + 7;

template <int Modulo = kModulo>
struct Modular {
  int value;

  Modular(int64_t x = 0) : value(x >= 0 ? x % Modulo : Modulo - (-x % Modulo)) {}

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

  Modular Inverse() const {
    return BinPow(Modulo - 2);
  }

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
    value = (static_cast<int64_t>(value) * static_cast<int64_t>(other.value)) % Modulo;
    return *this;
  }

  Modular& operator/=(const Modular& other) {
    return *this *= other.Inverse();
  }

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

  bool operator==(const Modular& other) const {
    return value == other.value;
  }

  bool operator==(int other) const {
    return value == other;
  }

  friend std::ostream& operator<<(std::ostream& stream, const Modular& value) {
    return stream << value.value;
  }
};

Modular<> Calculate(int a, int b, int c, int d) {
  Modular numerator = Modular(a) * Modular(d) + Modular(b) * Modular(c);
  Modular denominator = Modular(b) * Modular(d);
  return numerator / denominator;
}

int main() {
  int a;
  int b;
  int c;
  int d;
  std::cin >> a >> b >> c >> d;
  std::cout << Calculate(a, b, c, d) << std::endl;
  return 0;
}
