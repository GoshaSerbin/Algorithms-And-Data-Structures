/*
4_1. Солдаты. В одной военной части решили построить в одну шеренгу по росту. Т.к. часть была далеко не образцовая, то солдаты часто
приходили не вовремя, а то их и вовсе приходилось выгонять из шеренги за плохо начищенные сапоги. Однако солдаты в процессе прихода и ухода
должны были всегда быть выстроены по росту – сначала самые высокие, а в конце – самые низкие. За расстановку солдат отвечал прапорщик,
который заметил интересную особенность – все солдаты в части разного роста. Ваша задача состоит в том, чтобы помочь прапорщику правильно
расставлять солдат, а именно для каждого приходящего солдата указывать, перед каким солдатом в строе он должен становится.
Требования: скорость выполнения команды - O(log n).
*/

#include <cassert>
#include <iostream>

template <typename T, typename Comparator>
class AvlTree {
    struct Node {
        Node(const T& data) : data(data), left(nullptr), right(nullptr), height(1), count(1) {}
        T data;
        Node* left;
        Node* right;
        size_t height;
        size_t count;
    };

 public:
    AvlTree() : root(nullptr), comparator() {}
    ~AvlTree() { destroyTree(root); }

    // Добавляет элемент и возвращает его статистику
    size_t Add(const T& data) {
        size_t k{};
        root = addInternal(root, data, k);
        return k;
    }

    T FindKthStatistic(size_t k) {
        Node* tmp = root;
        while (tmp) {
            if (getCount(tmp->left) == k)
                return tmp->data;
            else if (getCount(tmp->left) < k) {
                k = k - getCount(tmp->left) - 1;
                tmp = tmp->right;
            } else {
                tmp = tmp->left;
            }
        }
        return T();
    }

    bool Has(const T& data) {
        Node* tmp = root;
        while (tmp) {
            if (tmp->data == data)
                return true;
            else if (comparator(tmp->data, data))
                tmp = tmp->right;
            else
                tmp = tmp->left;
        }
        return false;
    }

    void Delete(const T& data) { root = deleteInternal(root, data); }

    void DeleteKthStatistic(size_t k) { Delete(FindKthStatistic(k)); }

 private:
    void destroyTree(Node* node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    Node* findAndRemoveMin(Node* node) {
        Node* min = findMin(node);
        min->right = removeMin(node);
        return min;
    }

    Node* findMin(Node* node) {
        while (node->left) node = node->left;
        return node;
    }

    Node* removeMin(Node* node) {
        if (!node->left) return node->right;
        node->left = removeMin(node->left);
        return doBalance(node);
    }

    Node* findAndRemoveMax(Node* node) {
        Node* max = findMax(node);
        max->left = removeMax(node);
        return max;
    }

    Node* findMax(Node* node) {
        while (node->right) node = node->right;
        return node;
    }

    Node* removeMax(Node* node) {
        if (!node->right) return node->left;
        node->right = removeMax(node->right);
        return doBalance(node);
    }

    Node* deleteInternal(Node* node, const T& data) {
        if (!node) return nullptr;
        if (comparator(node->data, data)) {
            node->right = deleteInternal(node->right, data);
        } else if (node->data == data) {
            Node* left = node->left;
            Node* right = node->right;
            delete node;

            if (!right) return left;
            if (!left) return right;

            if (getHeight(right) > getHeight(left)) {
                node = findAndRemoveMin(right);
                node->left = left;
            } else {
                node = findAndRemoveMax(left);
                node->right = right;
            }

            return doBalance(node);
        } else {
            node->left = deleteInternal(node->left, data);
        }

        return doBalance(node);
    }

    Node* addInternal(Node* node, const T& data, size_t& k) {
        if (!node) {
            k = 0;
            return new Node(data);
        }
        if (comparator(node->data, data)) {
            node->right = addInternal(node->right, data, k);
            k += getCount(node->left) + 1;
        } else {
            node->left = addInternal(node->left, data, k);
        }

        return doBalance(node);
    }

    size_t getHeight(Node* node) { return node ? node->height : 0; }
    size_t getCount(Node* node) { return node ? node->count : 0; }

    void fixHeight(Node* node) { node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1; }

    void fixCount(Node* node) { node->count = getCount(node->left) + getCount(node->right) + 1; }

    int getBalance(Node* node) { return getHeight(node->right) - getHeight(node->left); }

    void fixHeightAndCount(Node* node) {
        fixHeight(node);
        fixCount(node);
    }

    Node* rotateLeft(Node* node) {
        Node* tmp = node->right;
        node->right = tmp->left;
        tmp->left = node;
        fixHeightAndCount(node);
        fixHeightAndCount(tmp);
        return tmp;
    }

    Node* rotateRight(Node* node) {
        Node* tmp = node->left;
        node->left = tmp->right;
        tmp->right = node;
        fixHeightAndCount(node);
        fixHeightAndCount(tmp);
        return tmp;
    }

    Node* doBalance(Node* node) {
        fixHeightAndCount(node);
        switch (getBalance(node)) {
            case 2: {
                if (getBalance(node->right) < 0) node->right = rotateRight(node->right);
                return rotateLeft(node);
            }
            case -2: {
                if (getBalance(node->left) > 0) node->left = rotateLeft(node->left);
                return rotateRight(node);
            }
            default:
                return node;
        }
    }

    Node* root;
    Comparator comparator;
};

class ReverseIntComparator {
 public:
    bool operator()(const int left, const int right) { return left > right; }
};

int main(int argc, const char* argv[]) {
    size_t N{};
    std::cin >> N;
    AvlTree<int, ReverseIntComparator> tree;
    char op{};
    int data{};

    for (size_t i = 0; i < N; ++i) {
        std::cin >> op >> data;
        switch (op) {
            case '1':
                std::cout << tree.Add(data) << std::endl;
                break;
            case '2':
                tree.DeleteKthStatistic(data);
                break;
        }
    }
    return 0;
}