#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

std::vector<size_t> GetZFunction(const std::string& string) {
  size_t str_size = string.size();
  std::vector<size_t> z_func(str_size, 0);
  size_t left = 0;
  size_t right = 0;
  for (size_t i = 1; i < str_size; ++i) {
    if (i <= right) {
      z_func[i] = std::min(z_func[i - left], right - i + 1);
    }
    while (i + z_func[i] < str_size && string[z_func[i]] == string[i + z_func[i]]) {
      ++z_func[i];
    }
    if (i + z_func[i] - 1 > right) {
      left = i;
      right = i + z_func[i] - 1;
    }
  }
  return z_func;
}

void PrintPatternOccurrencesInString(const std::string& string,
                                     const std::string& pattern) {
  auto z_func = GetZFunction(pattern + '#' + string);

  for (size_t i = 0; i < z_func.size(); ++i) {
    if (z_func[i] == pattern.size()) {
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