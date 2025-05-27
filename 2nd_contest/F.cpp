#include <array>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <vector>

const static int kPrecalcutedCount = 20000010;
const static int kProductArraySize = 6;
const static int kDPSize = 50;
const static int kSumOfFirstProductArraySize = 2 * 3 * 7 * 5 * 11 * 13;

std::vector<int> primes;
std::bitset<kPrecalcutedCount> is_prime;
std::array<int, kProductArraySize> prime_product;
std::array<int, kPrecalcutedCount> prime_count_precalculated;
std::array<std::array<int64_t, kSumOfFirstProductArraySize>, kDPSize> dp;

class PrimeNumbersCounter {
 public:
  PrimeNumbersCounter() {
    Sieve();
    Generate();
  }

  // Meissel-Lehmer algorithm
  int64_t Count(int64_t x) {
    if (x < kPrecalcutedCount) {
      return prime_count_precalculated[x];
    }

    int square_root = std::sqrt(0.5 + x);
    int cubic_root = std::cbrt(0.5 + x);

    int64_t result = CalculatePhi(x, prime_count_precalculated[cubic_root]) +
                     prime_count_precalculated[cubic_root] - 1;

    for (int i = prime_count_precalculated[cubic_root];
         i < prime_count_precalculated[square_root]; ++i) {
      result -= Count(x / primes[i]) - i;
    }

    return result;
  }

 private:
  void Sieve() {
    is_prime[2] = true;
    for (int i = 3; i < kPrecalcutedCount; i += 2) {
      is_prime[i] = true;
    }

    for (int i = 3; i * i < kPrecalcutedCount; i += 2) {
      for (int j = i * i; is_prime[i] && j < kPrecalcutedCount; j += (i << 1)) {
        is_prime[j] = false;
      }
    }

    for (int i = 1; i < kPrecalcutedCount; ++i) {
      prime_count_precalculated[i] =
          prime_count_precalculated[i - 1] + is_prime[i];
      if (is_prime[i]) {
        primes.push_back(i);
      }
    }
  }

  void Generate() {
    int i = 0;
    int j = 0;

    for (prime_product[0] = primes[0], i = 1; i < kProductArraySize; ++i) {
      prime_product[i] = prime_product[i - 1] * primes[i];
    }

    for (i = 0; i < kSumOfFirstProductArraySize; ++i) {
      dp[0][i] = i;
    }

    for (i = 1; i < kDPSize; ++i) {
      for (j = 1; j < kSumOfFirstProductArraySize; ++j) {
        dp[i][j] = dp[i - 1][j] - dp[i - 1][j / primes[i - 1]];
      }
    }
  }

  int64_t CalculatePhi(int64_t a, int x) {
    if (x == 0) {
      return a;
    }

    if (x < kDPSize && a < kSumOfFirstProductArraySize) {
      return dp[x][a];
    }

    if (x < kProductArraySize) {
      return dp[x][a % prime_product[x - 1]] +
             (a / prime_product[x - 1]) * dp[x][prime_product[x - 1]];
    }

    int64_t curr_prime = primes[x - 1];

    if (a < kPrecalcutedCount && curr_prime * curr_prime >= a) {
      return prime_count_precalculated[a] - x + 1;
    }

    if (curr_prime * curr_prime * curr_prime < a || a >= kPrecalcutedCount) {
      return CalculatePhi(a, x - 1) - CalculatePhi(a / curr_prime, x - 1);
    }

    int limit = prime_count_precalculated[(int)sqrt(0.5 + a)];

    int64_t result =
        prime_count_precalculated[a] - (limit + x - 2) * (limit - x + 1) / 2;

    for (int i = x; i < limit; ++i) {
      result += prime_count_precalculated[a / primes[i]];
    }

    return result;
  }
};

int main() {
  int64_t n = 0;
  std::cin >> n;

  PrimeNumbersCounter prime_counter;

  std::cout << prime_counter.Count(n) << std::endl;

  return 0;
}