#include <iostream>
#include <vector>
#include <numeric>
#include <limits>

int main() {
  size_t n = 0;
  std::cin >> n;

  std::vector<int> data(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> data[i];
  }

  size_t ones_count = 0;
  for (int a : data) {
    if (a == 1) {
      ++ones_count;
    }
  }

  if (ones_count > 0) {
    std::cout << (n - ones_count) << '\n';
    return 0;
  }

  size_t result = std::numeric_limits<size_t>::max();
  for (size_t i = 0; i < n; ++i) {
    int current_gcd = data[i];
    if (current_gcd == 1) {
      result = 1;
      break;
    }
  
    for (size_t j = i + 1; j < n; ++j) {
      current_gcd = std::gcd(current_gcd, data[j]);
      if (current_gcd == 1) {
        result = std::min(result, j - i + 1);
        break;
      }
    }
  }

  if (result == std::numeric_limits<size_t>::max()) {
    std::cout << -1 << '\n';
  } else {
    std::cout << (result - 1) + (n - 1) << '\n';
  }

  return 0;
}
