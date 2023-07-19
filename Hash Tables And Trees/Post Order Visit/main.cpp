/*2_3. Выведите элементы в порядке post-order (снизу вверх).*/

#include <functional>
#include <iostream>
#include <stack>

template <typename T>
struct Node {
    T data;
    Node<T>* left;
    Node<T>* right;
    Node(const T& data = T()) : data(data), left(nullptr), right(nullptr){};
};

template <typename T>
class DataComparator {
 public:
    bool operator()(const T& left, const T& right) { return left <= right; }
};

template <typename T, typename Comparator = std::less<T>>
class Tree {
 public:
    Tree() : root(nullptr), comparator(){};
    Tree(const Tree& tree) = delete;
    void add(const T& data) {
        if (root == nullptr) {
            root = new Node<T>(data);
            return;
        }
        Node<T>* current = root;
        while (true) {
            if (comparator(current->data, data)) {
                if (current->right == nullptr) {
                    current->right = new Node<T>(data);
                    return;
                } else {
                    current = current->right;
                }
            } else {
                if (current->left == nullptr) {
                    current->left = new Node<T>(data);
                    return;
                } else {
                    current = current->left;
                }
            }
        }
    };

    void print(std::ostream& out = std::cout) {
        postOrderDFS([&out](Node<T>* node) { out << node->data << " "; });
    }

    ~Tree() {
        postOrderDFS([](Node<T>* node) { delete node; });
    };

 private:
    template <typename Function>
    void postOrderDFS(Function visit) {
        Node<T>* current = root;
        std::stack<Node<T>*> s;
        Node<T>* lastVisitedNode;
        while (!s.empty() || current != nullptr) {
            if (current != nullptr) {
                s.push(current);
                current = current->left;
            } else {
                Node<T>* top = s.top();
                if (top->right != nullptr && lastVisitedNode != top->right) {
                    current = top->right;
                } else {
                    visit(top);
                    lastVisitedNode = top;
                    s.pop();
                }
            }
        }
    }

    Comparator comparator;
    Node<T>* root;
};

int main() {
    size_t N{};
    std::cin >> N;
    Tree<int, DataComparator<int>> tree;
    int number{};
    while (std::cin >> number) {
        tree.add(number);
    }
    tree.print();
    return 0;
}