#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

// = 7 * 2^20 + 1
static constexpr int kMod = 7340033;
static constexpr int kPrimitiveRootNotFound = -1;

int BinPow(int64_t base, int64_t exp) {
  int64_t result = 1;
  base %= kMod;
  while (exp > 0) {
    if (exp & 1) result = (result * base) % kMod;
    base = (base * base) % kMod;
    exp >>= 1;
  }
  return static_cast<int>(result);
}

void BitReversePermute(std::vector<int>& poly) {
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

class NTT {
 public:
  NTT() { primitive_root_ = FindPrimitiveRoot(); }

  void Transform(std::vector<int>& poly_A, bool invert) const {
    size_t length = poly_A.size();
    BitReversePermute(poly_A);
    int root = ComputeRoot(length, invert);

    for (size_t step = 2; step <= length; step <<= 1) {
      int w_length = BinPow(root, length / step);
      for (size_t block_start = 0; block_start < length; block_start += step) {
        int64_t w = 1;
        for (size_t offset = 0; offset < step / 2; ++offset) {
          int u = poly_A[block_start + offset];
          int v = static_cast<int>(poly_A[block_start + offset + step / 2] * w %
                                   kMod);
          poly_A[block_start + offset] = u + v < kMod ? u + v : u + v - kMod;
          poly_A[block_start + offset + step / 2] =
              u - v >= 0 ? u - v : u - v + kMod;
          w = w * w_length % kMod;
        }
      }
    }

    if (invert) {
      int inv_length = BinPow(length, kMod - 2);
      for (auto& x : poly_A) {
        x = static_cast<int>(x * 1ll * inv_length % kMod);
      }
    }
  }

  void PointwiseMultiply(std::vector<int>& poly_A,
                         const std::vector<int>& poly_B) const {
    for (size_t i = 0; i < poly_A.size(); ++i) {
      poly_A[i] = static_cast<int>(poly_A[i] * 1ll * poly_B[i] % kMod);
    }
  }

  size_t ComputeNTTSize(size_t target_size) const {
    size_t ntt_size = 1;
    while (ntt_size < target_size) {
      ntt_size <<= 1;
    }
    return ntt_size;
  }

 private:
  int primitive_root_;

  int FindPrimitiveRoot() {
    std::vector<int> factors = {2, 7};
    for (int candidate = 2; candidate < kMod; ++candidate) {
      bool ok = true;
      for (int factor : factors) {
        if (BinPow(candidate, (kMod - 1) / factor) == 1) {
          ok = false;
          break;
        }
      }
      if (ok) {
        return candidate;
      }
    }
    return kPrimitiveRootNotFound;
  }

  int ComputeRoot(size_t length, bool invert) const {
    int root = BinPow(primitive_root_, (kMod - 1) / length);
    if (invert) {
      root = BinPow(root, kMod - 2);
    }
    return root;
  }
};

class Polynomial {
 public:
  explicit Polynomial(std::vector<int> coefficients)
      : coeffs_(std::move(coefficients)) {}

  Polynomial operator*(const Polynomial& other) const {
    size_t result_degree = coeffs_.size() + other.coeffs_.size() - 1;
    size_t ntt_size = ntt_.ComputeNTTSize(result_degree);

    std::vector<int> poly_A = coeffs_;
    std::vector<int> poly_B = other.coeffs_;
    poly_A.resize(ntt_size);
    poly_B.resize(ntt_size);

    ntt_.Transform(poly_A, false);
    ntt_.Transform(poly_B, false);
    ntt_.PointwiseMultiply(poly_A, poly_B);
    ntt_.Transform(poly_A, true);

    poly_A.resize(result_degree);
    return Polynomial(std::move(poly_A));
  }

  Polynomial Invert(size_t m) const {
    std::vector<int> q = {BinPow(coeffs_[0], kMod - 2)};
    size_t current = 1;

    while (current < m) {
      size_t next = std::min(current << 1, m);
      std::vector<int> cut_P(coeffs_.begin(),
                             coeffs_.begin() + std::min(coeffs_.size(), next));
      Polynomial cut_poly_P(cut_P), poly_Q(q);

      Polynomial t = cut_poly_P * poly_Q;
      t.coeffs_.resize(next);

      std::vector<int> v(next);
      v[0] = (2 + kMod - t.coeffs_[0]) % kMod;
      for (size_t i = 1; i < next; ++i) {
        int ti = i < t.coeffs_.size() ? t.coeffs_[i] : 0;
        v[i] = (kMod - ti) % kMod;
      }

      Polynomial poly_V(v);
      Polynomial new_Q = poly_Q * poly_V;
      new_Q.coeffs_.resize(next);

      q = std::move(new_Q.coeffs_);
      current = next;
    }

    q.resize(m);
    return Polynomial(std::move(q));
  }

  const std::vector<int>& Coefficients() const { return coeffs_; }

 private:
  std::vector<int> coeffs_;
  static NTT ntt_;
};

NTT Polynomial::ntt_;

int main() {
  size_t m = 0;
  size_t n = 0;
  std::cin >> m >> n;

  std::vector<int> p_coeffs(n + 1);
  for (size_t i = 0; i <= n; ++i) {
    std::cin >> p_coeffs[i];
  }

  if (p_coeffs.empty() || p_coeffs[0] == 0) {
    std::cout << "The ears of a dead donkey\n";
    return 0;
  }

  Polynomial poly_P(std::move(p_coeffs));
  Polynomial poly_Q = poly_P.Invert(m);

  for (size_t i = 0; i < m; ++i) {
    std::cout << poly_Q.Coefficients()[i] << ' ';
  }
  std::cout << '\n';
}
