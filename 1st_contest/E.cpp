#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cassert>

// Прототипы решений
void NaiveSolution(const std::string& s, const std::string& t);
void CheckIfInteresting(const std::string& s, const std::string& t);
void CheckIfInteresting2(const std::string& s, const std::string& t);

// Функция для генерации случайного слова заданной длины
std::string GenerateRandomString(int length) {
  std::string str;
  for (int i = 0; i < length; ++i) {
    str += 'a' + rand() % 3;  // Генерируем буквы 'a', 'b' или 'c' (чтобы было больше коллизий)
  }
  return str;
}

// Стресс-тестирование
void StressTest(int num_tests, int max_length) {
  srand(time(nullptr));  // Инициализация генератора случайных чисел

  for (int test = 1; test <= num_tests; ++test) {
    int len_s = rand() % max_length + 1;
    int len_t = rand() % max_length + 1;

    std::string s = GenerateRandomString(len_s);
    std::string t = GenerateRandomString(len_t);

    // Захватываем вывод обоих решений в строки
    std::ostringstream naive_output, fast_output;
    std::streambuf* cout_buf = std::cout.rdbuf();

    std::cout.rdbuf(naive_output.rdbuf());
    NaiveSolution(s, t);

    std::cout.rdbuf(fast_output.rdbuf());
    CheckIfInteresting(s, t);

    std::cout.rdbuf(cout_buf);  // Возвращаем стандартный вывод

    if (naive_output.str() != fast_output.str()) {
      std::cout << "❌ Ошибка на тесте #" << test << ":\n";
      std::cout << "S = \"" << s << "\", T = \"" << t << "\"\n";
      std::cout << "Наивное решение:\n" << naive_output.str();
      std::cout << "Быстрое решение:\n" << fast_output.str();
      assert(false);  // Прерываем выполнение, чтобы проанализировать ошибку
    } else {
      std::cout << "✅ Тест #" << test << " пройден\n";
      std::cout << "S = \"" << s << "\", T = \"" << t << "\"\n";
      std::cout << "Наивное решение:\n" << naive_output.str();
      std::cout << "Быстрое решение:\n" << fast_output.str();
    }
  }
}

bool FindDecomposition(const std::string& s, const std::string& t, int index,
                       std::vector<std::string>& decomposition) {
  if (index == t.size()) {
    return true;
  }

  for (int len = 1; len <= s.size(); ++len) {
    if (index + len <= t.size() && t.substr(index, len) == s.substr(0, len)) {
      decomposition.push_back(t.substr(index, len));
      if (FindDecomposition(s, t, index + len, decomposition)) {
        return true;
      }
      decomposition.pop_back();
    }
  }

  return false;
}

void NaiveSolution(const std::string& s, const std::string& t) {
  std::vector<std::string> decomposition;
  if (FindDecomposition(s, t, 0, decomposition)) {
    std::cout << "No\n";
    for (const std::string& part : decomposition) {
      std::cout << part << " ";
    }
    std::cout << "\n";
  } else {
    std::cout << "Yes\n";
  }
}

std::vector<int> GetZFunction(const std::string& str) {
  int n = str.size();
  std::vector<int> z(n, 0);
  int left = 0;
  int right = 0;
  for (int i = 1; i < n; ++i) {
    if (i <= right) {
      z[i] = std::min(z[i - left], right - i + 1);
    }
    while (i + z[i] < n && str[z[i]] == str[i + z[i]]) {
      ++z[i];
    }
    if (i + z[i] - 1 > right) {
      left = i;
      right = i + z[i] - 1;
    }
  }
  return z;
}

void CheckIfInteresting(const std::string& s, const std::string& t) {
  std::string str = s + '#' + t;
  auto z = GetZFunction(str);
  size_t n = s.size();
  size_t m = t.size();

  size_t it = n + 1;
  size_t count = 0;
  std::vector<int> result;

  while (it < z.size()) {
    size_t next_it = it;
    size_t max_len = 0;
    size_t it2 = it;

    for (; it2 < std::min(it + z[it] + 1, z.size()); ++it2) {
      if (z[it2] + (it2 - it) >= max_len) {
        max_len = z[it2] + (it2 - it);
        next_it = it2;
      }
    }

    size_t prefix_len = next_it - it;
    prefix_len = (prefix_len == 0 && it2 == z.size()) ? z[it] : prefix_len;
    result.push_back(prefix_len);
    count += prefix_len;

    if (it == next_it) {
      break;
    }
    it = next_it;
  }

  if (count < m) {
    std::cout << "Yes\n";
    return;
  }

  std::cout << "No\n";
  size_t counter = 0;
  for (auto i : result) {
    counter += i;
    if (counter > m) {
      break;
    }
    std::cout << s.substr(0, i) << ' ';
  }
  std::cout << '\n';
}

int main() {
  std::string s;
  std::string t;

  std::cin >> s;
  std::cin >> t;

  CheckIfInteresting(s, t);

  // NaiveSolution(s, t);

  // StressTest(1000, 10);

  return 0;
}
