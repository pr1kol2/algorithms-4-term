#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct State {
  int length;
  int link;
  int first_pos;
  std::unordered_map<char, int> next;
};

class SuffixAutomaton {
 public:
  explicit SuffixAutomaton(const std::string& text) {
    states_.reserve(2 * text.size());

    states_.emplace_back(0, -1, -1);
    last_ = 0;

    for (int position = 0; position < static_cast<int>(text.size());
         ++position) {
      AddCharacter(text[position], position);
    }
  }

  int Size() const { return static_cast<int>(states_.size()); }

  const State& GetState(int index) const { return states_[index]; }

  State& GetState(int index) { return states_[index]; }

 private:
  int last_;
  std::vector<State> states_;

  void AddCharacter(char character, int position) {
    int current = Size();
    states_.emplace_back(states_[last_].length + 1, 0, position);
    int i = last_;

    while (i != -1 && !states_[i].next.count(character)) {
      states_[i].next[character] = current;
      i = states_[i].link;
    }

    if (i == -1) {
      states_[current].link = 0;
    } else {
      int j = states_[i].next[character];

      if (states_[i].length + 1 == states_[j].length) {
        states_[current].link = j;
      } else {
        int clone = Size();

        states_.emplace_back(states_[i].length + 1, states_[j].link,
                             states_[j].first_pos, states_[j].next);

        while (i != -1 && states_[i].next[character] == j) {
          states_[i].next[character] = clone;
          i = states_[i].link;
        }

        states_[j].link = states_[current].link = clone;
      }
    }
    last_ = current;
  }
};

void SolveProblem(const std::string& text) {
  SuffixAutomaton automaton(text);
  int size = automaton.Size();

  std::vector<int> min_pos(size);
  std::vector<int> max_pos(size);
  for (int i = 0; i < size; ++i) {
    min_pos[i] = automaton.GetState(i).first_pos;
    max_pos[i] = automaton.GetState(i).first_pos;
  }

  std::vector<int> order(size);
  for (int i = 0; i < size; ++i) {
    order[i] = i;
  }
  std::sort(order.begin(), order.end(), [&automaton](int a, int b) {
    return automaton.GetState(a).length > automaton.GetState(b).length;
  });

  for (int i : order) {
    if (automaton.GetState(i).link != -1) {
      int j = automaton.GetState(i).link;
      min_pos[j] = std::min(min_pos[j], min_pos[i]);
      max_pos[j] = std::max(max_pos[j], max_pos[i]);
    }
  }

  int answer = 0;
  for (int i = 1; i < size; ++i) {
    int left = automaton.GetState(automaton.GetState(i).link).length + 1;
    int right = automaton.GetState(i).length;
    int valid_right = std::min(right, max_pos[i] - min_pos[i]);
    if (valid_right >= left) {
      answer += static_cast<int>(
          valid_right - automaton.GetState(automaton.GetState(i).link).length);
    }
  }

  std::cout << answer << '\n';
}

int main() {
  std::string text;

  std::cin >> text;

  SolveProblem(text);

  return 0;
}
