#include <iostream>
#include <string>
#include <unordered_set>

struct PairHash {
  std::size_t operator()(const std::pair<int64_t, int64_t>& p) const {
    static const std::hash<int64_t> hasher;
    std::size_t seed = hasher(p.first);
    return seed ^ (hasher(p.second) + 0x9e3779b9 + (seed << 6) + (seed >> 2));
  }
};

// polynomial hash
const int64_t kMod1 = 1e9 + 21;
const int64_t kMod2 = 1e9 + 33;
const int64_t KHashPrimary = 53;

std::pair<int64_t, int64_t> ComputeHash(const std::string& str) {
  int64_t hash_1 = 0;
  int64_t hash_2 = 0;

  // goerner's method
  for (char character : str) {
    hash_1 = (1LL * hash_1 * KHashPrimary + character) % kMod1;
    hash_2 = (1LL * hash_2 * KHashPrimary + character) % kMod2;
  }

  return {hash_1, hash_2};
}

std::pair<int64_t, int64_t> ComputePowers(int64_t n) {
  int64_t power1 = 1;
  int64_t power2 = 1;

  for (int64_t i = 0; i < n - 1; ++i) {
    power1 = (1ll * power1 * KHashPrimary) % kMod1;
    power2 = (1ll * power2 * KHashPrimary) % kMod2;
  }

  return {power1, power2};
}

std::unordered_set<std::pair<int64_t, int64_t>, PairHash> GetCyclicShiftsHashes(
    const std::string& pattern) {
  int64_t n = pattern.size();
  std::string double_pattern = pattern + pattern;
  std::unordered_set<std::pair<int64_t, int64_t>, PairHash> shift_hashes;

  auto double_pattern_hash = ComputeHash(double_pattern.substr(0, n));
  shift_hashes.insert(double_pattern_hash);

  // powers update before rolling hash
  auto [power1, power2] = ComputePowers(n);

  // rolling hash algorithm
  for (int64_t i = n; i < 2 * n - 1; ++i) {
    double_pattern_hash.first =
        ((1ll * double_pattern_hash.first - double_pattern[i - n] * power1) * KHashPrimary +
         double_pattern[i]) %
        kMod1;
    double_pattern_hash.second =
        ((1ll * double_pattern_hash.second - double_pattern[i - n] * power2) * KHashPrimary +
         double_pattern[i]) %
        kMod2;

    if (double_pattern_hash.first < 0) {
      double_pattern_hash.first += kMod1;
    }
    if (double_pattern_hash.second < 0) {
      double_pattern_hash.second += kMod2;
    }

    shift_hashes.insert(double_pattern_hash);
  }

  return shift_hashes;
}

int64_t CountCyclicPatternOccurrences(const std::string& pattern,
                                  const std::string& text) {
  int64_t n = pattern.size();
  int64_t m = text.size();
  auto shift_hashes = GetCyclicShiftsHashes(pattern);

  std::pair<int64_t, int64_t> text_hash = ComputeHash(text.substr(0, n));
  auto count = shift_hashes.count(text_hash);

  // powers update before rolling hash
  auto [power1, power2] = ComputePowers(n);

  // rolling hash algorithm
  for (int64_t i = n; i < m; ++i) {
    text_hash.first =
        ((1ll * text_hash.first - text[i - n] * power1) * KHashPrimary +
         text[i]) %
        kMod1;
    text_hash.second =
        ((1ll * text_hash.second - text[i - n] * power2) * KHashPrimary +
         text[i]) %
        kMod2;

    if (text_hash.first < 0) {
      text_hash.first += kMod1;
    }
    if (text_hash.second < 0) {
      text_hash.second += kMod2;
    }

    count += shift_hashes.count(text_hash);
  }

  return count;
}

int main() {
  std::string pattern;
  std::string text;

  std::cin >> pattern >> text;

  std::cout << CountCyclicPatternOccurrences(pattern, text) << "\n";
  return 0;
}
