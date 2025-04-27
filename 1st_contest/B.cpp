#include <iostream>
#include <string>
#include <vector>

std::string BuildFromPrefix(const std::vector<int>& p) {
  static std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
  std::string str{};

  for (size_t i = 0; i < p.size(); ++i) {
    if (p[i] == 0) {
      bool found_candidate = false;
      for (char candidate : alphabet) {
        size_t j = (i > 0 ? p[i - 1] : 0);
        while (str[j] != candidate && j > 0) {
          j = p[j - 1];
        }
        if (str[j] == candidate) {
          ++j;
        }
        if (j == 0) {
          str += candidate;
          found_candidate = true;
          break;
        }
      }
    } else {
      str += str[p[i] - 1];
    } 
  }
  return str;
}

int main() {
  size_t n = 0;
  std::cin >> n;
  std::vector<int> p(n);

  for (size_t i = 0; i < n; ++i) {
    std::cin >> p[i];
  }

  std::cout << BuildFromPrefix(p) << "\n";

  return 0;
}
