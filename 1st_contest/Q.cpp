#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct State {
  int64_t length;
  int64_t link;
  std::unordered_map<char, int64_t> next;

  State(int64_t length, int64_t link) : length(length), link(link) {}
};

class SuffixAutomaton {
 public:
  explicit SuffixAutomaton(const std::string &text) {
    states_.reserve(2 * text.size());

    states_.emplace_back(0, -1);
    last_ = 0;

    int64_t current_total = 0;

    for (char character : text) {
      current_total += AddCharacter(character);
      substring_count.push_back(current_total);
    }
  }

  const std::vector<int64_t>& GetSubstringCounts() const {
    return substring_count;
  }

 private:
  std::vector<State> states_;
  std::vector<int64_t> substring_count;
  int64_t last_;

  int64_t AddCharacter(char character) {
    int64_t current = states_.size();
    states_.emplace_back(states_[last_].length + 1, 0);
    int64_t i = last_;

    while (i != -1 && !states_[i].next.count(character)) {
      states_[i].next[character] = current;
      i = states_[i].link;
    }

    if (i == -1) {
      states_[current].link = 0;
    } else {
      int64_t j = states_[i].next[character];

      if (states_[i].length + 1 == states_[j].length) {
        states_[current].link = j;
      } else {
        int64_t clone = states_.size();

        states_.push_back(states_[j]);
        states_[clone].length = states_[i].length + 1;

        while (i != -1 && states_[i].next[character] == j) {
          states_[i].next[character] = clone;
          i = states_[i].link;
        }

        states_[j].link = states_[current].link = clone;
      }
    }
  
    last_ = current;

    return states_[current].length - states_[states_[current].link].length;
  }
};

void SolveProblem(const std::string &text) {
  SuffixAutomaton automaton(text);

  for (int64_t count : automaton.GetSubstringCounts()) {
    std::cout << count << "\n";
  }
}

int main() {
  std::string text;

  std::cin >> text;

  SolveProblem(text);
  return 0;
}
