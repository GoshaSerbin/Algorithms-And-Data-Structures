#include <functional>
#include <iostream>
#include <queue>
#include <vector>

template <typename T, typename Comparator = std::less<T>>
class BTree {
 public:
    struct Node {
        Node(bool leaf) : leaf(leaf) {}

        ~Node() {
            for (Node* child : children) {
                delete child;
            }
        }

        bool leaf;
        std::vector<T> keys;
        std::vector<Node*> children;
    };

    BTree(size_t min_degree) : t(min_degree), root(nullptr), cmp() {}

    ~BTree() {
        if (root) delete root;
    }

    void Insert(const T& key) {
        if (!root) root = new Node(true);

        // здесь и дальше: если при спуске по дереву находим
        // переполненный узел -- сначала разбиваем его, потом спускаемся
        if (isNodeFull(root)) {
            Node* newRoot = new Node(false);
            newRoot->children.push_back(root);
            root = newRoot;
            splitChild(root, 0);
        }

        // теперь корень точно не переполнен, можем вызвать insertNonFull
        insertNonFull(root, key);
    }

    void debugPrint() { debugPrintInternal(root, 0); }

    void BFSprint() {
        BFS([](Node* node) {
            for (const auto& k : node->keys) {
                std::cout << k << " ";
            }
        });
    }

 private:
    void debugPrintInternal(Node* node, int indent) {
        std::cout << std::string(indent, ' ');
        std::cout << "keys: [";
        for (auto it = node->keys.begin(); it != node->keys.end(); it++) {
            std::cout << (*it);
            if (it + 1 != node->keys.end()) std::cout << ", ";
        }
        std::cout << "]" << std::endl;

        for (auto child : node->children) {
            debugPrintInternal(child, indent + 4);
        }
    }

    bool isNodeFull(Node* node) { return node->keys.size() == 2 * t - 1; }

    // разбить переполненного потомка index узла node
    void splitChild(Node* node, size_t index) {
        Node* Y = node->children[index];
        Node* Z = new Node(Y->leaf);
        Z->keys.resize(t - 1);
        for (int j = 0; j < t - 1; ++j) {
            Z->keys[j] = Y->keys[j + t];
        }
        if (!Y->leaf) {
            Y->children.resize(t);
            Z->children.resize(t);
            for (int j = 0; j < t; ++j) {
                Z->children[j] = Y->children[j + t];
            }
        }
        Y->keys.resize(t - 1);
        node->children.push_back(new Node(0));
        for (int j = node->children.size() - 2; j >= index + 1; --j) {
            node->children[j + 1] = node->children[j];
        }
        node->children[index + 1] = Z;

        node->keys.push_back(T());
        for (int j = static_cast<int>(node->keys.size()) - 2; j >= static_cast<int>(index); --j) {
            node->keys[j + 1] = node->keys[j];
        }
        node->keys[index] = Y->keys[t - 1];
    }

    // вставить ключ key в гарантированно не переполненную ноду node
    void insertNonFull(Node* node, const T& key) {
        int pos = node->keys.size() - 1;

        // гарантированно не перепеполненный лист -- запишем новый ключ в него
        if (node->leaf) {
            // расширили вектор ключей для вставки нового
            node->keys.resize(node->keys.size() + 1);
            while (pos >= 0 && cmp(key, node->keys[pos])) {
                // обходим ключи справа налево, сдвигая вправо на 1
                node->keys[pos + 1] = node->keys[pos];
                pos--;
            }
            // вставляем новый ключ на освобожденное в цикле место
            node->keys[pos + 1] = key;
        }
        // не лист, значит есть потомки, пишем в один из них
        else {
            // ищем позицию потомка, в которого добавим ключ
            while (pos >= 0 && cmp(key, node->keys[pos])) {
                pos--;
            }

            // если потомок и так полон, надо его сначала разбить
            if (isNodeFull(node->children[pos + 1])) {
                splitChild(node, pos + 1);
                // после разбиения потомка в текущий узел из него поднялся ключ
                // надо сравниться и с ним
                if (cmp(node->keys[pos + 1], key)) pos++;
            }
            insertNonFull(node->children[pos + 1], key);
        }
    }

    template <typename Function>
    void BFS(Function visit) {
        std::vector<Node*> v1 = {root};
        std::vector<Node*> v2{};
        while (v1.size() > 0) {
            for (const auto& current : v1) {
                visit(current);
                if (!current->leaf) {
                    for (int i = 0; i < current->children.size(); ++i) {
                        v2.push_back(current->children[i]);
                    }
                }
            }
            v1.clear();
            std::swap(v1, v2);
            std::cout << "\n";
        }
    }

    size_t t;
    Node* root;
    Comparator cmp;

    friend void test1();
};

// случаи вставки с иллюстраций в лекции
void test1() {
    BTree<char> tree(3);

    tree.root = new BTree<char>::Node(false);
    tree.root->keys = {'G', 'M', 'P', 'X'};

    {
        auto child = new BTree<char>::Node(true);
        child->keys = {'A', 'C', 'D', 'E'};
        tree.root->children.push_back(child);
    }

    {
        auto child = new BTree<char>::Node(true);
        child->keys = {'J', 'K'};
        tree.root->children.push_back(child);
    }
    {
        auto child = new BTree<char>::Node(true);
        child->keys = {'N', 'O'};
        tree.root->children.push_back(child);
    }
    {
        auto child = new BTree<char>::Node(true);
        child->keys = {'R', 'S', 'T', 'U', 'V'};
        tree.root->children.push_back(child);
    }
    {
        auto child = new BTree<char>::Node(true);
        child->keys = {'Y', 'Z'};
        tree.root->children.push_back(child);
    }

    std::cout << "Initial tree:" << std::endl;
    tree.debugPrint();
    std::cout << std::endl;

    std::string insertKeys = "BQLF";
    // посимвольно добавляем в дерево ключи
    for (auto c : insertKeys) {
        tree.Insert(c);
        std::cout << "After inserting " << c << ":" << std::endl;
        tree.debugPrint();
        std::cout << std::endl;
    }
    tree.BFSprint();
}

int main(int argc, const char* argv[]) {
    // test1();
    int min_degree{};
    std::cin >> min_degree;
    BTree<int, std::less<int>> tree(min_degree);
    int number{};
    while (std::cin >> number) {
        tree.Insert(number);
    }
    tree.BFSprint();
    return 0;
}