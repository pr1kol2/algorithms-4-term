#include <cstdint>
#include <iostream>
#include <vector>

int64_t CalculateLeastPrimeDivisorSum(int n) {
  std::vector<int> least_prime_divisor(n + 1, 0);
  std::vector<int> primes;

  for (uint32_t i = 2; i <= n; ++i) {
    if (least_prime_divisor[i] == 0) {
      least_prime_divisor[i] = i;
      primes.push_back(i);
    }
  
    for (uint32_t p : primes) {
      if (p > least_prime_divisor[i] || p * i > n) break;
      least_prime_divisor[p * i] = p;
    }
  }

  uint64_t sum = 0;
  for (uint32_t i = 3; i <= n; ++i) {
    if (i != least_prime_divisor[i]) {
      sum += least_prime_divisor[i];
    }
  }
  return sum;
}

int main() {
  uint32_t n = 0;
  std::cin >> n;

  std::cout << CalculateLeastPrimeDivisorSum(n) << '\n';
  
  return 0;
}
