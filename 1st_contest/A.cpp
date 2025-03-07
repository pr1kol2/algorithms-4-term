#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

std::vector<size_t> GetZFunction(const std::string& string) {
  size_t str_size = string.size();
  std::vector<size_t> z(str_size, 0);
  size_t left = 0;
  size_t right = 0;
  for (size_t i = 1; i < str_size; ++i) {
    if (i <= right) {
      z[i] = std::min(z[i - left], right - i + 1);
    }
    while (i + z[i] < str_size && string[z[i]] == string[i + z[i]]) {
      ++z[i];
    }
    if (i + z[i] - 1 > right) {
      left = i;
      right = i + z[i] - 1;
    }
  }
  return z;
}

void PrintPatternOccurrencesInString(const std::string& string,
                                     const std::string& pattern) {
  auto z = GetZFunction(pattern + '#' + string);

  for (size_t i = 0; i < z.size(); ++i) {
    if (z[i] == pattern.size()) {
      std::cout << i - (1 + pattern.size()) << '\n';
    }
  }
}

int main() {
  std::string string;
  std::string pattern;

  std::cin >> string;
  std::cin >> pattern;

  PrintPatternOccurrencesInString(string, pattern);

  return 0;
}