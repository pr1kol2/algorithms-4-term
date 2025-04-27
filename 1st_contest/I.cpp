#include <cstdint>
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

const uint64_t KHashPrimary1 = 313;
const uint64_t KHashPrimary2 = 317;

struct HashPair {
  uint64_t hash1, hash2;
  bool operator==(const HashPair& other) const {
    return hash1 == other.hash1 && hash2 == other.hash2;
  }
};

std::vector<HashPair> ComputePrefixHashes(const std::string& str, uint64_t n) {
  std::vector<HashPair> hash(n + 1, {0, 0});
  for (uint64_t i = 0; i < n; ++i) {
    hash[i + 1].hash1 = (hash[i].hash1 * KHashPrimary1 + uint64_t(str[i]));
    hash[i + 1].hash2 = (hash[i].hash2 * KHashPrimary2 + uint64_t(str[i]));
  }
  return hash;
}

std::vector<HashPair> ComputeReversedPrefixHashes(const std::string& str,
                                                  uint64_t n) {
  std::vector<HashPair> hash(n + 1, {0, 0});
  for (uint64_t i = 0; i < n; ++i) {
    hash[i + 1].hash1 =
        (hash[i].hash1 * KHashPrimary1 + uint64_t(str[n - i - 1]));
    hash[i + 1].hash2 =
        (hash[i].hash2 * KHashPrimary2 + uint64_t(str[n - i - 1]));
  }
  return hash;
}

std::vector<HashPair> ComputePowers(uint64_t n) {
  std::vector<HashPair> power(n + 1, {1, 1});
  for (uint64_t i = 1; i <= n; ++i) {
    power[i].hash1 = (power[i - 1].hash1 * KHashPrimary1);
    power[i].hash2 = (power[i - 1].hash2 * KHashPrimary2);
  }
  return power;
}

HashPair GetSubstrHash(const std::vector<HashPair>& hash,
                       const std::vector<HashPair>& power, uint64_t l,
                       uint64_t r) {
  return {(hash[r].hash1 - (hash[l].hash1 * power[r - l].hash1)),
          (hash[r].hash2 - (hash[l].hash2 * power[r - l].hash2))};
}

struct StringInfo {
  uint64_t index;
  uint64_t begin;
  uint64_t end;
  HashPair hash;
  HashPair reverse_hash;
};

bool IsPalindrome(uint64_t l, uint64_t r,
                  const std::vector<HashPair>& prefix_hashes,
                  const std::vector<HashPair>& powers,
                  const std::vector<HashPair>& reversed_prefix_hashes,
                  uint64_t str_len) {
  return GetSubstrHash(prefix_hashes, powers, l, r) ==
         GetSubstrHash(reversed_prefix_hashes, powers, str_len - r,
                       str_len - l);
}

int main() {
  uint64_t n = 0;
  std::cin >> n;

  std::vector<StringInfo> string_data(n);
  std::string merged_string = "";

  for (uint64_t i = 0; i < n; ++i) {
    std::string str;
    std::cin >> str;
    string_data[i].begin = merged_string.size();
    merged_string += str;
    string_data[i].end = merged_string.size();
    string_data[i].index = i;
  }

  std::vector<HashPair> prefix_hashes =
      ComputePrefixHashes(merged_string, merged_string.size());
  std::vector<HashPair> reversed_prefix_hashes =
      ComputeReversedPrefixHashes(merged_string, merged_string.size());
  std::vector<HashPair> powers = ComputePowers(merged_string.size());

  std::unordered_map<uint64_t, StringInfo> reverse_hashes_map;

  for (uint64_t i = 0; i < n; ++i) {
    string_data[i].hash = GetSubstrHash(
        prefix_hashes, powers, string_data[i].begin, string_data[i].end);
    string_data[i].reverse_hash =
        GetSubstrHash(reversed_prefix_hashes, powers,
                      merged_string.size() - string_data[i].end,
                      merged_string.size() - string_data[i].begin);
    reverse_hashes_map.insert(
        {string_data[i].reverse_hash.hash1, string_data[i]});
  }

  std::set<std::pair<uint64_t, uint64_t>> result;

  for (auto& elem : string_data) {
    for (uint64_t i = elem.begin; i <= elem.end; ++i) {
      if (IsPalindrome(elem.begin, i, prefix_hashes, powers,
                       reversed_prefix_hashes, merged_string.size())) {
        uint64_t substr_hash =
            GetSubstrHash(prefix_hashes, powers, i, elem.end).hash1;
        auto it = reverse_hashes_map.find(substr_hash);
        if (it != reverse_hashes_map.end() && it->second.index != elem.index) {
          result.emplace(it->second.index, elem.index);
        }
      }
      if (IsPalindrome(i, elem.end, prefix_hashes, powers,
                       reversed_prefix_hashes, merged_string.size())) {
        uint64_t substr_hash =
            GetSubstrHash(prefix_hashes, powers, elem.begin, i).hash1;
        auto it = reverse_hashes_map.find(substr_hash);
        if (it != reverse_hashes_map.end() && it->second.index != elem.index) {
          result.emplace(elem.index, it->second.index);
        }
      }
    }
  }

  std::cout << result.size() << '\n';
  for (auto& elem : result) {
    std::cout << elem.first + 1 << ' ' << elem.second + 1 << '\n';
  }

  return 0;
}
