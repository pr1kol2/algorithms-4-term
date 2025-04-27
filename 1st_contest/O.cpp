#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct Suffix {
  int index;  // begin of the suffix in the string
  int rank;   //
  int next_rank;

  bool operator<(const Suffix& other) const {
    return (this->rank == other.rank)
               ? (this->next_rank < other.next_rank ? 1 : 0)
               : (this->rank < other.rank ? 1 : 0);
  }
};

// indexation from 0
std::vector<int> BuildSuffixArray(const std::string& str) {
  size_t n = str.size();
  std::vector<Suffix> suffix_data(n);

  // assigning ranks & next_ranks to suffixes
  for (int i = 0; i < n; i++) {
    suffix_data[i].index = i;
    suffix_data[i].rank = str[i] - ' ';
    suffix_data[i].next_rank = ((i + 1) < n) ? (str[i + 1] - ' ') : -1;
  }

  // sorting the suffixes according to first 2 characters
  std::sort(suffix_data.begin(), suffix_data.end());

  // sorting suffixes according to first 4, 8, ... characters
  std::vector<int> suffixes_index(
      n);  // index in suffix_data by index in string
  for (int i = 4; i < 2 * n; i = i * 2) {
    int rank = 0;
    int prev_rank = suffix_data[0].rank;
    suffix_data[0].rank = rank;
    suffixes_index[suffix_data[0].index] = 0;

    for (int j = 1; j < n; j++) {
      if (suffix_data[j].rank == prev_rank &&
          suffix_data[j].next_rank == suffix_data[j - 1].next_rank) {
        prev_rank = suffix_data[j].rank;
        suffix_data[j].rank = rank;
      } else {
        prev_rank = suffix_data[j].rank;
        suffix_data[j].rank = ++rank;
      }
      suffixes_index[suffix_data[j].index] = j;
    }

    // Assign next rank to every Suffix
    for (int j = 0; j < n; j++) {
      int next_index = suffix_data[j].index + i / 2;
      suffix_data[j].next_rank =
          (next_index < n) ? suffix_data[suffixes_index[next_index]].rank : -1;
    }

    // sorting the suffixes according to first i characters
    std::sort(suffix_data.begin(), suffix_data.end());
  }

  std::vector<int> suffix_array(n);
  for (int i = 0; i < n; i++) {
    suffix_array[i] = suffix_data[i].index;
  }

  return suffix_array;
}

// indexation from 0
std::vector<int> GetLCP(const std::string& str,
                        const std::vector<int>& suffix_array) {
  size_t n = str.size();
  std::vector<int> lcp(n);

  // inverse suffix array[j] = i <=> suffix_array[i] = j
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
  std::string input;
  std::getline(std::cin, input);

  std::vector<int> suffix_array = BuildSuffixArray(input);
  std::vector<int> lcp = GetLCP(input, suffix_array);

  size_t n = input.size();
  int64_t result = 0;
  for (size_t i = 0; i < n; ++i) {
    result += n - suffix_array[i];
  }
  for (size_t i = 0; i < n - 1; ++i) {
    result -= lcp[i];
  }

  std::cout << result << '\n';

  return 0;
}
