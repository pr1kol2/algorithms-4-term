#include <iostream>

int main() {
  int n;
  std::cin >> n;
  for (int i = 1; i <= 2 * n; i++) {
    std::cout << i << " ";
  }
  return 0;
}