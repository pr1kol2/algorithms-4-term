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

std::vector<int> FromZToPrefix(const std::vector<size_t>& z) {
  std::vector<int> p(z.size(), 0);
  for(int i = 1; i < z.size(); i++) {
    if(z[i] != 0) {
      for (int j = z[i] - 1; j >= 0 && !(p[i + j]); j--) {
        p[i + j] = j + 1;
      }
    }
  }
  return p;
}

int main() {
  size_t n = 0;
  std::cin >> n;
  std::vector<size_t> z(n);

  for (size_t i = 0; i < n; ++i) {
    std::cin >> z[i];
  }

  z[0] = 0;

  std::cout << BuildFromPrefix(FromZToPrefix(z)) << "\n";

  return 0;
}
