#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

std::vector<int> manacher_odd(std::string s) {
  std::vector<int> radius(s.size(), 1);
  int left = 0;
  int right = 0;
  for (int i = 1; i < s.size(); ++i) {
    if (i < right)
      radius[i] = std::min(right - i + 1, radius[left + right - i]);
    while (i - radius[i] >= 0 && i + radius[i] < s.size() &&
           s[i - radius[i]] == s[i + radius[i]])
      ++radius[i];
    if (i + radius[i] - 1 > right)
      left = i - radius[i] + 1, right = i + radius[i] - 1;
  }
  return radius;
}

int main() {
  std::string s;
  std::cin >> s;

  std::string s_trick;

  for (int i = 0; i < 2 * s.size() - 1; i++) {
    if (i % 2 == 0) {
      s_trick.push_back(s[i / 2]);
    } else {
      s_trick.push_back('#');
    }
  }

  // std::cout << s_trick << "\n";

  std::vector<int> radius = manacher_odd(s_trick);
  int res = 0;
  for (int i = 0; i < radius.size(); i++) {
    // std::cout << radius[i] << " ";
    if (i % 2 != 0) {
      res += std::max(0, radius[i] / 2);
    } else {
      res += std::max(0, (radius[i] - 1) / 2);
    }
  }
  // std::cout << "\n";

  std::cout << res << "\n";

  return 0;
}
