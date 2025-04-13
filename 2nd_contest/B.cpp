#include <iostream>
#include <vector>

int64_t CalculateLeastPrimeDivisorSum(int n) {
  std::vector<int> least_prime_divisor(n + 1, 0);
  std::vector<int> primes;

  for (int i = 2; i <= n; ++i) {
    if (least_prime_divisor[i] == 0) {
      least_prime_divisor[i] = i;
      primes.push_back(i);
    }
  
    for (int p : primes) {
      if (p > least_prime_divisor[i] || p * i > n) break;
      least_prime_divisor[p * i] = p;
    }
  }

  int64_t sum = 0;
  for (int i = 3; i <= n; ++i) {
    if (i != least_prime_divisor[i]) {
      sum += least_prime_divisor[i];
    }
  }
  return sum;
}

int main() {
  int n = 0;
  std::cin >> n;

  std::cout << CalculateLeastPrimeDivisorSum(n) << std::endl;
  
  return 0;
}
