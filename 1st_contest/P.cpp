#include <iostream>
#include <map>

class TwoSuffixTree {
 public:
  TwoSuffixTree(const std::string& str1, const std::string& str2)
      : str1(str1), str2(str2), text(str1 + str2) {
    root_end_ = new int(-1);

    root = new Node(-1, root_end_, root);

    BuildSuffixTree();

    CountNodesDFS(root);
  }

  ~TwoSuffixTree() {
    ClearNodesDFS(root); 
    delete root_end_;
  }

  friend std::ostream& operator<<(std::ostream& out, const TwoSuffixTree& tree);

  int GetTreeSize() const { return tree_size; }

 private:
  struct Node;

  std::string text;
  std::string str1;
  std::string str2;

  int tree_size = 0;
  Node* root = nullptr;

  Node* global_end_ = nullptr;

  Node* active_node_ = nullptr;
  int active_edge_ = -1;
  int active_length_ = 0;

  int remaining_suffix_count_ = 0;
  int leaf_end_ = -1;
  int* root_end_ = nullptr;
  int* split_end_ = nullptr;

  struct Node {
    std::map<char, Node*> children;

    int start;
    int* end;

    Node* suffix_link;
    int suffix_index;

    Node(int start, int* end, Node* root)
        : start(start), end(end), suffix_link(root), suffix_index(-1) {}
  };

  enum ReturnCode : int {
    NONE = -1,
    CONTINUE = 0,
    BREAK = 1,
  };

  int EdgeLength(Node* node) {
    if (node == root) {
      return 0;
    }
    return *(node->end) - (node->start) + 1;
  }

  bool WalkDown(Node* currNode) {
    if (active_length_ >= EdgeLength(currNode)) {
      active_edge_ += EdgeLength(currNode);
      active_length_ -= EdgeLength(currNode);
      active_node_ = currNode;
      return true;
    }
    return false;
  }

  ReturnCode ProcessSuffix(int pos) {
    if (active_length_ == 0) {
      active_edge_ = pos;
    }

    if (active_node_->children[text[active_edge_]] == nullptr) {
      active_node_->children[text[active_edge_]] =
          new Node(pos, &leaf_end_, root);

      if (global_end_ != nullptr) {
        global_end_->suffix_link = active_node_;
        global_end_ = nullptr;
      }
    } else {
      Node* next = active_node_->children[text[active_edge_]];
      if (WalkDown(next)) {
        return CONTINUE;
      }
      if (text[next->start + active_length_] == text[pos]) {
        if (global_end_ != nullptr && active_node_ != root) {
          global_end_->suffix_link = active_node_;
          global_end_ = nullptr;
        }

        ++active_length_;
        return BREAK;
      }

      split_end_ = new int;
      *split_end_ = next->start + active_length_ - 1;

      Node* split = new Node(next->start, split_end_, root);
      active_node_->children[text[next->start]] = split;

      split->children[text[pos]] = new Node(pos, &leaf_end_, root);
      next->start += active_length_;
      split->children[text[next->start]] = next;

      if (global_end_ != nullptr) {
        global_end_->suffix_link = split;
      }

      global_end_ = split;
    }

    return NONE;
  }

  void PostProcessSuffix(int pos) {
    --remaining_suffix_count_;

    if (active_node_ == root && active_length_ > 0) {
      active_length_--;
      active_edge_ = pos - remaining_suffix_count_ + 1;
    } else if (active_node_ != root) {
      active_node_ = active_node_->suffix_link;
    }
  }

  // Ukkonen's algorithm
  void ExtendSuffixTree(int pos) {
    leaf_end_ = pos;

    remaining_suffix_count_++;

    global_end_ = nullptr;

    while (remaining_suffix_count_ > 0) {
      ReturnCode code = ProcessSuffix(pos);
      if (code == BREAK) {
        break;
      } else if (code == CONTINUE) {
        continue;
      }

      PostProcessSuffix(pos);
    }
  }

  void BuildSuffixTree() {
    active_node_ = root;
    for (int i = 0; i < text.size(); ++i) {
      ExtendSuffixTree(i);
    }
  }

  void CountNodesDFS(Node* node) {
    if (node == nullptr) {
      return;
    }

    ++tree_size;

    for (auto [_, child] : node->children) {
      if (child != nullptr) {
        CountNodesDFS(child);
      }
    }
  }

  void ClearNodesDFS(Node* node) {
    if (node == nullptr) {
      return;
    }

    for (auto [_, child] : node->children) {
      if (child != nullptr) {
        ClearNodesDFS(child);
      }
    }

    delete node;
  }

  void PrintSuffixTreeByDFS(Node* node, int level, int& index, int parent) const {
    if (node == nullptr) {
      return;
    }

    if (parent != -1) {
      if (node->start < str1.size()) {
        std::cout << parent << " " << 0 << " " << node->start << " "
                  << std::min(*(node->end) + 1, static_cast<int>(str1.size())) << "\n";
      } else {
        std::cout << parent << " " << 1 << " " << node->start - str1.size()
                  << " " << *(node->end) + 1 - str1.size() << "\n";
      }
    }

    ++index;

    int current_parent = index;

    for (auto [_, child] : node->children) {
      if (child != nullptr) {
        PrintSuffixTreeByDFS(child, level + 2, index, current_parent);
      }
    }
  }
};

std::ostream& operator<<(std::ostream& out, const TwoSuffixTree& tree) {
  int index = -1;
  tree.PrintSuffixTreeByDFS(tree.root, 0, index, -1);
  return out;
}

int main() {
  std::string str1;
  std::string str2;

  std::cin >> str1 >> str2;

  TwoSuffixTree tree(str1, str2);

  std::cout << tree.GetTreeSize() << "\n";

  std::cout << tree;

  return 0;
}
