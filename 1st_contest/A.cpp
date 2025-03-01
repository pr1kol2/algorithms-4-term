#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

std::vector<int> GetZFunction(const std::string& s) {
  int n = s.size();
  std::vector<int> z(n, 0);
  int left = 0;
  int right = 0;
  for (int i = 1; i < n; ++i) {
    if (i <= right) {
      z[i] = std::min(z[i - left], right - i + 1);
    }
    while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
      ++z[i];
    }
    if (i + z[i] - 1 > right) {
      left = i;
      right = i + z[i] - 1;
    }
  }
  return z;
}

void PrintPatternOccurrencesInString(const std::string& s,
                                     const std::string& p) {
  auto z = GetZFunction(p + '#' + s);

  for (int i = 0; i < z.size(); ++i) {
    if (z[i] == p.size()) {
      std::cout << i - (1 + p.size()) << '\n';
    }
  }
}

int main() {
  std::string s;
  std::string p;

  std::cin >> s;
  std::cin >> p;

  PrintPatternOccurrencesInString(s, p);

  return 0;
}