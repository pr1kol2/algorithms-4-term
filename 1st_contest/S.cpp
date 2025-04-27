#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>

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

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s, t;
    long long k;
    cin >> s >> t >> k;
    
    string u = s + "$" + t + "#";
    int len_s = s.size();
    int len_u = u.size();
    
    vector<int> sa = BuildSuffixArray(u);
    vector<int> lcp = GetLCP(u, sa);
    
    vector<int> mark(len_u, -1);
    for (int i = 0; i < len_u; i++) {
        if (sa[i] < len_s) mark[i] = 0;
        else if (sa[i] > len_s) mark[i] = 1;
    }
    
    vector<int> common(len_u, 0);
    for (int i = 0; i < len_u; i++) {
        int cand = 0;
        if (i > 0 && mark[i] != mark[i-1]) cand = max(cand, lcp[i-1]);
        if (i < len_u - 1 && mark[i] != mark[i+1]) cand = max(cand, lcp[i]);
        common[i] = cand;
    }
    
    long long total = 0;
    vector<long long> newCount(len_u, 0);
    for (int i = 0; i < len_u; i++) {
        int base = (i == 0 ? 0 : lcp[i-1]);
        if (common[i] > base) {
            newCount[i] = common[i] - base;
            total += newCount[i];
        }
    }
    
    if (total < k) {
        cout << -1 << "\n";
        return 0;
    }
    
    long long sum = 0;
    int posIndex = -1;
    for (int i = 0; i < len_u; i++) {
        if (newCount[i] > 0) {
            if (sum + newCount[i] >= k) {
                posIndex = i;
                break;
            } else {
                sum += newCount[i];
            }
        }
    }
    
    assert(posIndex != -1);
    int offset = k - sum;
    int prefixLength = (posIndex == 0 ? 0 : lcp[posIndex-1]) + offset;
    string ans = u.substr(sa[posIndex], prefixLength);
    cout << ans << "\n";
    
    return 0;
}
