#include <array>
#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <vector>

template <typename T>
class AhoKorasikTrie {
  const size_t k_alphabet_size_;
  const T k_first_char_;

 public:
  AhoKorasikTrie(T first_char, size_t alphabet_size)
      : k_first_char_(first_char),
        k_alphabet_size_(alphabet_size),
        root_(std::make_unique<Node>(-1, nullptr, k_alphabet_size_)),
        str_count(0) {}

  void AddString(const std::vector<T>& str) {
    Node* current = root_.get();
    for (char character : str) {
      size_t index = character - k_first_char_;
      if (!current->children[index]) {
        current->children[index] =
            std::make_unique<Node>(index, current, k_alphabet_size_);
      }
      current = current->children[index].get();
    }
    current->is_terminal = true;
    current->length = str.size();
    current->ids.push_back(str_count++);
  }

  bool Find(const std::vector<T>& str) const {
    const Node* current = root_.get();
    for (char character : str) {
      size_t index = character - k_first_char_;
      if (!current->children[index]) {
        return false;
      }
      current = current->children[index].get();
    }
    return current->is_terminal;
  }

  void BuildAutomaton() {
    std::queue<Node*> queue;
    root_->link = root_.get();

    for (size_t i = 0; i < k_alphabet_size_; ++i) {
      if (root_->children[i]) {
        Node* child = root_->children[i].get();
        child->link = root_.get();
        queue.push(child);
      }
    }

    while (!queue.empty()) {
      Node* node_ptr = queue.front();
      queue.pop();

      for (size_t i = 0; i < k_alphabet_size_; ++i) {
        if (node_ptr->children[i]) {
          Node* child = node_ptr->children[i].get();
          child->link = UpdateState(node_ptr->link, i);
          queue.push(child);
        }
      }
    }
  }

  std::vector<size_t> CountMaskSubstringsOccurences(
      const std::vector<T>& text, const std::vector<size_t>& substr_positions) {
    std::vector<size_t> result(text.size() + 1);
    Node* node_ptr = root_.get();

    for (size_t i = 0; i < text.size(); ++i) {
      char character = text[i];
      size_t index = character - k_first_char_;

      if (index >= k_alphabet_size_) {
        node_ptr = root_.get();
        continue;
      }

      node_ptr = UpdateState(node_ptr, index);

      Node* temp = node_ptr;
      while (temp != nullptr && temp != root_.get()) {
        if (temp->is_terminal) {
          for (size_t id : temp->ids) {
            size_t pos = i - temp->length + 1;
            if (pos <= i) {
              if (pos + 1 < substr_positions[id]) {
                continue;
              }
              ++result[pos - substr_positions[id] + 1];
            }
          }
        }
        temp = temp->link;
      }
    }

    return result;
  }

 private:
  struct Node {
    std::vector<std::unique_ptr<Node>> children{};
    std::vector<Node*> go{};
    Node* link = nullptr;
    Node* parent;
    int parent_char;
    bool is_terminal = false;
    size_t length = 0;
    std::vector<size_t> ids{};

    Node(int p_ch, Node* p, size_t k_alphabet_size)
        : parent_char(p_ch), parent(p) {
      children.reserve(k_alphabet_size);
      go.assign(k_alphabet_size, nullptr);
    }
  };

  // const char k_first_char_;
  std::unique_ptr<Node> root_;
  size_t str_count = 0;

  Node* Link(Node* node_ptr) {
    if (node_ptr->link == nullptr) {
      node_ptr->link =
          (node_ptr == root_.get() || node_ptr->parent == root_.get())
              ? root_.get()
              : UpdateState(Link(node_ptr->parent), node_ptr->parent_char);
    }
    return node_ptr->link;
  }

  Node* UpdateState(Node* node_ptr, size_t added_char) {
    if (node_ptr->go[added_char] == nullptr) {
      node_ptr->go[added_char] =
          node_ptr->children[added_char]
              ? node_ptr->children[added_char].get()
              : (node_ptr == root_.get()
                     ? root_.get()
                     : UpdateState(Link(node_ptr), added_char));
    }
    return node_ptr->go[added_char];
  }
};

void GetMaskSubstrings(const std::string& mask,
                       std::vector<std::string>& substrings,
                       std::vector<size_t>& substr_positions) {
  std::string current_substr;
  for (size_t i = 0; i < mask.size(); ++i) {
    char character = mask[i];
    if (character != '?') {
      current_substr.push_back(character);
    } else if (!current_substr.empty()) {
      substrings.push_back(current_substr);
      substr_positions.push_back(i - current_substr.size());
      current_substr = "";
    }
  }

  if (!current_substr.empty()) {
    substrings.push_back(current_substr);
    substr_positions.push_back(mask.size() - current_substr.size());
  }
}

void FindAllMaskOccurences(const std::string& mask, const std::string& text) {
  std::vector<std::string> substrings;
  std::vector<size_t> substr_positions;
  GetMaskSubstrings(mask, substrings, substr_positions);

  AhoKorasikTrie<char> trie('a', 26);
  for (auto& substr : substrings) {
    trie.AddString(std::vector(substr.begin(), substr.end()));
  }
  trie.BuildAutomaton();

  auto mask_substr_occurences = trie.CountMaskSubstringsOccurences(
      std::vector(text.begin(), text.end()), substr_positions);

  if (text.size() < mask.size()) {
    std::cout << "0\n";
    return;
  }

  for (size_t i = 1;
       i <= text.size() - mask.size() + 1;
       ++i) {
    if (mask_substr_occurences[i] == substrings.size()) {
      std::cout << i - 1 << " ";
    }
  }
  std::cout << "\n";
}

int main() {
  std::string mask;
  std::string text;
  std::cin >> mask >> text;

  FindAllMaskOccurences(mask, text);

  return 0;
}
