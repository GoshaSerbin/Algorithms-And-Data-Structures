/*
1_1. Для разрешения коллизий используйте квадратичное пробирование. i-ая проба
g(k, i)=g(k, i-1) + i (mod m). m - степень двойки.
*/

#include <iostream>
#include <vector>

enum class elementState {
    KEY,
    NIL,
    DELETED
};

template <typename T>
struct HashTableElement {
    T data;
    elementState state;
    HashTableElement() : data(), state(elementState::NIL){};
    HashTableElement(const T& data) : data(data), state(elementState::KEY){};
};

constexpr size_t defaultPrime = 71;

class StringHasher {
 public:
    StringHasher(size_t prime = defaultPrime) : prime(prime) {}
    size_t operator()(const std::string& str) {
        size_t hash = 0;
        for (int i = 0; i < str.size(); ++i) {
            hash = hash * prime + str[i];
        }
        return hash;
    }

 private:
    size_t prime;
};

using stringElement = HashTableElement<std::string>;
constexpr size_t defaultInitialSize = 8;
constexpr double defaultAlpha = 0.75;

template <typename T, typename Hasher>
class HashTable {
 public:
    HashTable(size_t initialSize = defaultInitialSize, double alpha = defaultAlpha) : table(initialSize), alpha(alpha), size(0){};
    bool Add(const T& data) {
        size_t hashProbe = hasher(data);
        size_t firstDeletedElementIndex = 0;
        bool isFoundDeleted = false;
        for (size_t i = 0; i < table.size(); ++i) {
            hashProbe = (hashProbe + i) % table.size();
            switch (table[hashProbe].state) {
                case elementState::DELETED:
                    if (!isFoundDeleted) {
                        firstDeletedElementIndex = hashProbe;
                        isFoundDeleted = true;
                    }
                    break;
                case elementState::NIL:
                    table[hashProbe] = stringElement(data);
                    ++size;
                    if (size > alpha * table.size()) {
                        grow();
                    }
                    return true;
                case elementState::KEY:
                    if (table[hashProbe].data == data) {
                        return false;
                    }
            }
        }
        table[firstDeletedElementIndex] = stringElement(data);
        return true;
    }
    bool Has(const T& data) {
        size_t hashProbe = hasher(data);
        for (size_t i = 0; i < table.size(); ++i) {
            hashProbe = (hashProbe + i) % table.size();
            switch (table[hashProbe].state) {
                case elementState::NIL:
                    return false;
                case elementState::KEY:
                    if (table[hashProbe].data == data) {
                        return true;
                    }
            }
        }
        return false;
    }
    bool Delete(const T& data) {
        size_t hashProbe = hasher(data);
        for (size_t i = 0; i < table.size(); ++i) {
            hashProbe = (hashProbe + i) % table.size();
            switch (table[hashProbe].state) {
                case elementState::NIL:
                    return false;
                case elementState::KEY:
                    if (table[hashProbe].data == data) {
                        table[hashProbe].state = elementState::DELETED;
                        return true;
                    }
            }
        }
        return false;
    }

 private:
    void grow() {
        std::vector<HashTableElement<T>> newTable(table.size() * 2);
        for (const auto& el : table) {
            if (el.state == elementState::KEY) {
                size_t hashProbe = hasher(el.data);
                for (size_t i = 0;; ++i) {
                    hashProbe = (hashProbe + i) % newTable.size();
                    if (newTable[hashProbe].state == elementState::NIL) {
                        newTable[hashProbe] = stringElement(el.data);
                        break;
                    }
                }
            }
        }
        table.clear();
        table.swap(newTable);
    }

    std::vector<HashTableElement<T>> table;
    Hasher hasher;
    size_t size;
    double alpha;
};

int main() {
    HashTable<std::string, StringHasher> table;
    char op;
    std::string key;
    while (std::cin >> op >> key) {
        switch (op) {
            case '?':
                std::cout << (table.Has(key) ? "OK" : "FAIL") << std::endl;
                break;
            case '-':
                std::cout << (table.Delete(key) ? "OK" : "FAIL") << std::endl;
                break;
            case '+':
                std::cout << (table.Add(key) ? "OK" : "FAIL") << std::endl;
                break;
            case '!':
                return 0;
        }
    }
    return 0;
}