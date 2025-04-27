#include <array>
#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>

struct Trie {
  static const size_t kAlphabetSize = 2;
  static const char kFirstChar = '0';

  struct Node {
    std::array<std::unique_ptr<Node>, kAlphabetSize> children{};
    std::array<Node*, kAlphabetSize> go{};
    Node* link = nullptr;
    Node* parent;
    int parent_char;
    bool is_terminal = false;

    Node(int p_ch, Node* p) : parent_char(p_ch), parent(p) { go.fill(nullptr); }
  };

  Trie() : root_(std::make_unique<Node>(-1, nullptr)) {}

  void AddString(const std::string& str) {
    Node* current = root_.get();
    for (char character : str) {
      size_t index = character - kFirstChar;
      if (!current->children[index]) {
        current->children[index] = std::make_unique<Node>(index, current);
      }
      current = current->children[index].get();
    }
    current->is_terminal = true;
  }

  bool Find(const std::string& str) const {
    const Node* current = root_.get();
    for (char character : str) {
      size_t index = character - kFirstChar;
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

    for (size_t i = 0; i < kAlphabetSize; ++i) {
      if (root_->children[i]) {
        root_->children[i]->link = root_.get();
        queue.push(root_->children[i].get());
      }
    }

    while (!queue.empty()) {
      Node* node_ptr = queue.front();
      queue.pop();


	  // ANCHOR : MarkBadNodes()	
      if (node_ptr->link->is_terminal) {
        node_ptr->is_terminal = true;
      }

      for (size_t i = 0; i < kAlphabetSize; ++i) {
        if (node_ptr->children[i]) {
          Node* u = node_ptr->children[i].get();
          u->link = Go(node_ptr->link, i);
          queue.push(u);
        }
      }
    }
  }

  Node* Link(Node* node_ptr) {
    if (node_ptr->link == nullptr) {
      node_ptr->link =
          (node_ptr == root_.get() || node_ptr->parent == root_.get())
              ? root_.get()
              : Go(Link(node_ptr->parent), node_ptr->parent_char);
    }
    return node_ptr->link;
  }

  Node* Go(Node* node_ptr, size_t added_char) {
    if (node_ptr->go[added_char] == nullptr) {
      node_ptr->go[added_char] =
          node_ptr->children[added_char]
              ? node_ptr->children[added_char].get()
              : (node_ptr == root_.get() ? root_.get()
                                         : Go(Link(node_ptr), added_char));
    }
    return node_ptr->go[added_char];
  }

  std::unique_ptr<Node> root_;

  bool HasSafeCycle() {
    std::unordered_set<Node*> visited;
    std::unordered_set<Node*> current_path_stack;

    return DfsRecursive(root_.get(), visited, current_path_stack);
  }

  bool DfsRecursive(Node* node_ptr, std::unordered_set<Node*>& visited,
                    std::unordered_set<Node*>& current_path_stack) {
    if (current_path_stack.count(node_ptr)) {
      return true;
    }
    if (visited.count(node_ptr)) {
      return false;
    }

    visited.insert(node_ptr);
    current_path_stack.insert(node_ptr);

    for (size_t i = 0; i < kAlphabetSize; ++i) {
      Node* next_node = Go(node_ptr, i);
      if (!next_node->is_terminal &&
          DfsRecursive(next_node, visited, current_path_stack)) {
        return true;
      }
    }

    current_path_stack.erase(node_ptr);
    return false;
  }
};

std::string SolveWithTrie(const std::vector<std::string>& virus_codes) {
  Trie trie;
  for (const auto& code : virus_codes) {
    trie.AddString(code);
  }

  trie.BuildAutomaton();
  // trie.MarkBadNodes();

  return trie.HasSafeCycle() ? "TAK" : "NIE";
}

int main() {
  size_t n;
  std::cin >> n;

  std::vector<std::string> virus_codes;
  for (size_t i = 0; i < n; ++i) {
    std::string str;
    std::cin >> str;
    virus_codes.push_back(str);
  }

  std::cout << SolveWithTrie(virus_codes) << std::endl;

  return 0;
}
