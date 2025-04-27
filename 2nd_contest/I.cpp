#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

// 7340033 = 7 * 2^20 + 1
static const int kMod = 7340033;

int BinPow(int64_t base, int64_t power) {
  int64_t result = 1;
  base %= kMod;
  while (power) {
    if (power & 1) {
      result = (result * base) % kMod;
    }
    base = (base * base) % kMod;
    power >>= 1;
  }
  return static_cast<int>(result);
}

int GetPrimitiveRoot() {
  std::vector<int> factors = {2, 7};
  for (int g = 2; g < kMod; ++g) {
    bool found = true;
    for (int factor : factors) {
      if (BinPow(g, (kMod - 1) / factor) == 1) {
        found = false;
        break;
      }
    }
    if (found) {
      return g;
    }
  }
  return -1;
}

void Ntt(std::vector<int>& a, bool invert) {
  size_t n = a.size();

  for (int i = 1, j = 0; i < n; ++i) {
    int bit = n >> 1;
    for (; j & bit; bit >>= 1) j ^= bit;
    j |= bit;
    if (i < j) std::swap(a[i], a[j]);
  }

  const int primitive_root = GetPrimitiveRoot();

  int root = BinPow(primitive_root, (kMod - 1) / n);

  if (invert) {
    root = BinPow(root, kMod - 2);
  }

  for (int len = 2; len <= n; len <<= 1) {
    int w_length = BinPow(root, n / len);
    for (int i = 0; i < n; i += len) {
      int64_t w = 1;
      for (int j = 0; j < len / 2; ++j) {
        int u = a[i + j];
        int v = static_cast<int>(a[i + j + len / 2] * w % kMod);
        a[i + j] = u + v < kMod ? u + v : u + v - kMod;
        a[i + j + len / 2] = u - v >= 0 ? u - v : u - v + kMod;
        w = w * w_length % kMod;
      }
    }
  }

  if (invert) {
    int inv_n = BinPow(n, kMod - 2);
    for (int& x : a) {
      x = static_cast<int>(x * 1ll * inv_n % kMod);
    }
  }
}

std::vector<int> Multiply(const std::vector<int>& a, const std::vector<int>& b,
                          size_t degree) {
  size_t ntt_size = 1;
  while (ntt_size < a.size() + b.size() - 1) {
    ntt_size <<= 1;
  }

  std::vector<int> a_values(a.begin(), a.end());
  std::vector<int> b_values(b.begin(), b.end());

  a_values.resize(ntt_size);
  b_values.resize(ntt_size);

  Ntt(a_values, false);
  Ntt(b_values, false);

  for (size_t i = 0; i < ntt_size; ++i) {
    a_values[i] = static_cast<int>(a_values[i] * 1ll * b_values[i] % kMod);
  }

  Ntt(a_values, true);

  a_values.resize(degree);

  return a_values;
}

std::vector<int> InvertPolynomial(const std::vector<int>& P, size_t m) {
  std::vector<int> Q(1);

  Q[0] = BinPow(P[0], kMod - 2);

  size_t current = 1;
  while (current < m) {
    int next = std::min(current << 1, m);

    // T = P_cut * Q mod x^next
    std::vector<int> P_cut(P.begin(),
                           P.begin() + std::min<int>(P.size(), next));
    std::vector<int> T = Multiply(P_cut, Q, next);

    // v = 2 - T
    std::vector<int> v(next);
    v[0] = (2 + kMod - T[0]) % kMod;
    for (int i = 1; i < next; ++i) {
      int t = i < static_cast<int>(T.size()) ? T[i] : 0;
      v[i] = (kMod - t) % kMod;
    }

    // Q_new = Q * v mod x^next
    std::vector<int> Q_new = Multiply(Q, v, next);
    Q.swap(Q_new);

    current = next;
  }

  Q.resize(m);

  return Q;
}

int main() {
  size_t m = 0;
  size_t n = 0;
  std::cin >> m >> n;

  std::vector<int> P(n + 1);

  for (size_t i = 0; i <= n; ++i) {
    std::cin >> P[i];
  }

  if (P.empty() || P[0] == 0) {
    std::cout << "The ears of a dead donkey\n";
    return 0;
  }

  std::vector<int> Q = InvertPolynomial(P, m);

  for (size_t i = 0; i < m; ++i) {
    std::cout << Q[i] << ' ';
  }
  std::cout << '\n';

  return 0;
}
