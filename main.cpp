#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

struct Node {
  int key_;
  int priority_;
  Node* left_;
  Node* right_;
  Node* parent_;
  int size_;
};
void InorderWalk(Node* x);
void DeleteTree(Node* p);
class DekartTree {
  Node* root_;

 public:
  DekartTree() : root_(nullptr) {
  }
  Node* Merge(Node* t1, Node* t2) {
    if (t1 == nullptr) {
      return t2;
    }
    if (t2 == nullptr) {
      return t1;
    }
    if (t1->priority_ < t2->priority_) {
      t1->right_ = Merge(t1->right_, t2);
      t1->right_->parent_ = t1;
      Fix(t1);
      return t1;
    }
    t2->left_ = Merge(t1, t2->left_);
    t2->left_->parent_ = t2;
    Fix(t2);
    return t2;
  }
  std::pair<Node*, Node*> Split(Node* t, int x) {
    if (t == nullptr) {
      return {nullptr, nullptr};
    }
    if (t->key_ < x) {
      auto tmp = Split(t->right_, x);
      auto left = tmp.first;
      auto right = tmp.second;
      t->right_ = left;
      if (left != nullptr) {
        left->parent_ = t;
      }
      if (right != nullptr) {
        right->parent_ = nullptr;
      }
      Fix(t);
      return {t, right};
    }
    auto tmp = Split(t->left_, x);
    auto left = tmp.first;
    auto right = tmp.second;
    t->left_ = right;
    Fix(t);
    if (right != nullptr) {
      right->parent_ = t;
    }
    if (left != nullptr) {
      left->parent_ = nullptr;
    }
    return {left, t};
  }
  int Result(Node* node) {
    return (node != nullptr) ? node->size_ : 0;
  }
  void Fix(Node* node) {
    if (node != nullptr) {
      node->size_ = Result(node->left_) + 1 + Result(node->right_);
    }
  }
  void InorderWalk(Node* x) {
    if (x != nullptr) {
      std::cout << x->key_ << " " << x->size_ << "\n";
      InorderWalk(x->left_);
      InorderWalk(x->right_);
    }
  }
  Node* Insert(Node* t, int x, int y) {
    auto tmp = Split(t, x);
    auto now = new Node{x, y, nullptr, nullptr, nullptr, 0};
    root_ = Merge(Merge(tmp.first, now), tmp.second);
    return root_;
  }
  Node* Find(Node* x, int k) {
    if (x == nullptr || k == x->key_) {
      return x;
    }
    if (k < x->key_) {
      return Find(x->left_, k);
    }
    return Find(x->right_, k);
  }
  Node* Prev(Node* t, int x) {
    Node* tmp = t;
    Node* ans = nullptr;
    while (tmp != nullptr) {
      if (tmp->key_ < x) {
        ans = tmp;
        tmp = tmp->right_;
      } else {
        tmp = tmp->left_;
      }
    }
    return ans;
  }
  Node* Maximum(Node* x) {
    if (x == nullptr) {
      return nullptr;
    }
    if (x->right_ == nullptr) {
      return x;
    }
    return Maximum(x->right_);
  }
  Node* Next(Node* t, int x) {
    Node* tmp = t;
    Node* ans = nullptr;
    while (tmp != nullptr) {
      if (tmp->key_ > x) {
        ans = tmp;
        tmp = tmp->left_;
      } else {
        tmp = tmp->right_;
      }
    }
    return ans;
  }
  Node* Erase(Node* t, int x) {
    auto keyz = Next(t, x);
    int key;
    if (keyz != nullptr) {
      key = keyz->key_;
      auto tmp = Split(t, x);
      auto tmp2 = Split(tmp.second, key);
      DeleteTree(tmp2.first);
      root_ = Merge(tmp.first, tmp2.second);
    } else {
      auto tmp = Split(t, x);
      DeleteTree(tmp.second);
      root_ = tmp.first;
    }
    return root_;
  }
  Node* Kth(Node* t, int k) {
    if (t->size_ <= k || k < 0) {
      return nullptr;
    }
    while (t != nullptr) {
      if (t->left_ != nullptr) {
        if (t->left_->size_ < k) {
          k -= (t->left_->size_ + 1);
          t = t->right_;
        } else if (t->left_->size_ > k) {
          t = t->left_;
        } else {
          return t;
        }
      } else {
        if (t->right_ != nullptr) {
          if (k == 0) {
            return t;
          }
          t = t->right_;
        } else {
          return t;
        }
      }
    }
    return nullptr;
  }
  Node* Root() {
    return root_;
  }
  ~DekartTree() {
    DeleteTree(root_);
  }
};
void DeleteTree(Node* p) {
  if (p == nullptr) {
    return;
  }
  DeleteTree(p->left_);
  DeleteTree(p->right_);
  delete p;
}
int main() {
  DekartTree tree;
  std::string s;
  int tmp;
  while (std::cin >> s) {
    if (s == "insert") {
      int x;
      std::cin >> x;
      if (tree.Find(tree.Root(), x) == nullptr) {
        int y = rand() % 1000000;
        tree.Insert(tree.Root(), x, y);
      }
    }
    if (s == "exists") {
      std::cin >> tmp;
      std::cout << ((tree.Find(tree.Root(), tmp) != nullptr) ? "true" : "false") << "\n";
    }
    if (s == "delete") {
      std::cin >> tmp;
      tree.Erase(tree.Root(), tmp);
    }
    if (s == "next") {
      std::cin >> tmp;
      auto h = tree.Next(tree.Root(), tmp);
      if (h == nullptr) {
        std::cout << "none"
                  << "\n";
      } else {
        std::cout << h->key_ << "\n";
      }
    }
    if (s == "prev") {
      std::cin >> tmp;
      auto h = tree.Prev(tree.Root(), tmp);
      if (h == nullptr) {
        std::cout << "none"
                  << "\n";
      } else {
        std::cout << h->key_ << "\n";
      }
    }
    if (s == "kth") {
      std::cin >> tmp;
      auto h = tree.Kth(tree.Root(), tmp);
      if (h == nullptr) {
        std::cout << "none"
                  << "\n";
      } else {
        std::cout << h->key_ << "\n";
      }
    }
  }
  return 0;
}
