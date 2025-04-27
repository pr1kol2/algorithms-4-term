#include <iostream>
#include <string>
#include <vector>

size_t GetMaxOverlap(const std::string& lhs_str, const std::string& rhs_str) {
  size_t m = lhs_str.size();
  size_t n = rhs_str.size();
  size_t max_overlap = 0;

  for (size_t i = 1; i <= std::min(m, n); ++i) {
    if (lhs_str.substr(m - i) == rhs_str.substr(0, i)) {
      max_overlap = i;
    }
  }

  return max_overlap;
}

void MergeIntoPun(std::string& old_pun, const std::string& next_word) {
  size_t overlap = GetMaxOverlap(old_pun, next_word);

  old_pun += next_word.substr(overlap);
}

int main() {
  size_t n = 0;
  std::cin >> n;

  std::string old_pun;
  std::cin >> old_pun;

  for (size_t i = 1; i < n; ++i) {
    std::string next_word;
    std::cin >> next_word;
    MergeIntoPun(old_pun, next_word);
  }

  std::cout << old_pun << '\n';

  return 0;
}