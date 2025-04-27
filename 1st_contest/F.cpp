#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

const int64_t kMod1 = 1e9 + 7;

std::vector<int64_t> ComputePrefixHashes(const std::vector<int>& str,
                                         int64_t base, int n) {
  std::vector<int64_t> hash(n + 1, 0);
  for (int i = 0; i < n; ++i) {
    hash[i + 1] = (hash[i] * base + str[i]) % kMod1;
  }
  return hash;
}

std::vector<int64_t> ComputeReversedPrefixHashes(const std::vector<int>& str,
                                                 int64_t base, int n) {
  std::vector<int64_t> hash(n + 1, 0);
  for (int i = 0; i < n; ++i) {
    hash[i + 1] = (hash[i] * base + str[n - i - 1]) % kMod1;
  }
  return hash;
}

std::vector<int64_t> ComputePowers(int64_t base, int n) {
  std::vector<int64_t> power(n + 1, 1);
  for (int i = 1; i <= n; ++i) {
    power[i] = (power[i - 1] * base) % kMod1;
  }
  return power;
}

// hash of str[l..r)
int64_t GetSubstrHash(const std::vector<int64_t>& hash,
                      const std::vector<int64_t>& power, int l, int r) {
  return (hash[r] - (hash[l] * power[r - l]) % kMod1 + kMod1) % kMod1;
}

std::vector<int> GetPossibleCubesCount(const std::vector<int>& str, int n,
                                       int m) {
  std::vector<int> result;
  std::vector<int64_t> reversed_prefix_hashes =
      ComputeReversedPrefixHashes(str, m + 1, n);
  std::vector<int64_t> prefix_hashes = ComputePrefixHashes(str, m + 1, n);
  std::vector<int64_t> powers = ComputePowers(m + 1, n);

  for (int i = n / 2; i > 0; --i) {
    int64_t hash_real = GetSubstrHash(reversed_prefix_hashes, powers, n - i, n);

    int64_t hash_reflection = GetSubstrHash(prefix_hashes, powers, i, i + i);

    // std::cerr << hash_real << ' ' << hash_reflection << '\n';

    if (hash_real == hash_reflection) {
      result.push_back(n - i);
    }
  }

  result.push_back(n);

  return result;
}

int main() {
  int n = 0;
  int m = 0;
  std::cin >> n >> m;

  std::vector<int> str(n, '\0');
  for (int i = 0; i < n; i++) {
    std::cin >> str[i];
  }

  std::vector<int> result = GetPossibleCubesCount(str, n, m);

  for (int i = 0; i < result.size(); i++) {
    std::cout << result[i] << ' ';
  }
  std::cout << "\n";

  return 0;
}
