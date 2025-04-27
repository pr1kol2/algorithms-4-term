#include <algorithm>
#include <iostream>
#include <string>
#include <vector>


// indexation from 0
std::vector<int> GetLCP(const std::string& str,
                        const std::vector<int>& suffix_array) {
  size_t n = str.size();
  std::vector<int> lcp(n);
  std::vector<int> inverse_suffix_array(n);
  for (int i = 0; i < n; ++i) {
    inverse_suffix_array[suffix_array[i]] = i;
  }

  int current_lcp_value = 0;

  for (int i = 0; i < n; ++i) {
    if (inverse_suffix_array[i] == n - 1) {
      continue;
    }

    int next = suffix_array[inverse_suffix_array[i] + 1];

    while (str[i + current_lcp_value] == str[next + current_lcp_value] &&
           std::max(i, next) + current_lcp_value < n) {
      ++current_lcp_value;
    }

    lcp[inverse_suffix_array[i]] = current_lcp_value;
    current_lcp_value = std::max(0, current_lcp_value - 1);
  }

  return lcp;
}

int main() {
  size_t n = 0;
  std::string str;

  std::cin >> n >> str;

  std::vector<int> suffix_array(n);
  for (size_t i = 0; i < n; i++) {
    std::cin >> suffix_array[i];
    --suffix_array[i];
  }

  std::vector<int> lcp = GetLCP(str, suffix_array);

  for (size_t i = 0; i < n - 1; ++i) {
    std::cout << lcp[i] << ' ';
  }
  std::cout << '\n';

  return 0;
}
