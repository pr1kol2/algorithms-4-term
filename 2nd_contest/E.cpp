#include <algorithm>
#include <iostream>
#include <vector>

// FIXME : WA 4
int main() {
  size_t n = 0;
  size_t q = 0;
  std::cin >> n >> q;

  std::vector<int> elem_count(q);
  int max_elem_count = 0;
  for (size_t i = 0; i < q; ++i) {
    std::cin >> elem_count[i];
    max_elem_count = std::max(max_elem_count, elem_count[i]);
  }

  std::vector<std::vector<int>> divisors(n + 1);
  for (int x = 2; x <= n; ++x) {
    for (size_t j = x; j <= n; j += x) {
      divisors[j].push_back(x);
    }
  }

  int version = 0;
  std::vector<int> used(n + 1, 0);
  std::vector<int> diff(max_elem_count + 2, 0);

  int prev_filled = 0;
  for (size_t i = 1; i <= n && prev_filled < max_elem_count; ++i) {
    ++version;
    int count = 0;

    for (int x = 1; x <= n; ++x) {
      bool have_to_add = true;
      for (int divisor : divisors[x]) {
        if (divisor <= i) {
          continue;
        }

        if (used[divisor] == version) {
          have_to_add = false;
          break;
        }
      }

      if (!have_to_add) {
        continue;
      }

      ++count;
      for (int divisor : divisors[x]) {
        if (divisor <= i) {
          break;
        }
        used[divisor] = version;
      }

      if (count >= max_elem_count) {
        break;
      }
    }

    int min_added = std::min(count, max_elem_count);
    if (min_added > prev_filled) {
      diff[prev_filled + 1] += i;
      diff[min_added + 1] -= i;
      prev_filled = min_added;
    }
  }

  std::vector<int> min_beauties(max_elem_count + 1);
  for (size_t k = 1; k <= max_elem_count; ++k) {
    min_beauties[k] = min_beauties[k - 1] + diff[k];
  }

  for (int k : elem_count) {
    std::cout << min_beauties[k] << ' ';
  }
  std::cout << '\n';
}
