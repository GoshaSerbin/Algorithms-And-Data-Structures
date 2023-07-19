#include "Huffman.h"

#include <stack>
#include <unordered_map>
#include <vector>

using PairByteFrequency = std::pair<byte, size_t>;

namespace MySTL {
    // АТД динамический массив
    template <typename T>
    class Array {
     private:
        T* buffer;
        int bufferSize;
        int realSize;

     public:
        Array() : buffer(0), bufferSize(0), realSize(0){};
        Array(const Array& arr) {
            bufferSize = realSize = arr.getSize();
            buffer = new T[bufferSize];
            for (int i = 0; i < bufferSize; ++i) {
                buffer[i] = arr[i];
            }
        };
        Array<T> operator=(const Array& arr) = delete;
        ~Array() { delete[] buffer; };
        int getSize() const { return realSize; };
        void add(T element) {
            if (realSize == bufferSize) grow();
            buffer[realSize++] = element;
        };
        void popBack() {
            if (realSize > 0) --realSize;
        };
        T operator[](int index) const { return buffer[index]; };
        T& operator[](int index) { return buffer[index]; };

     private:
        void grow() {
            int newSize = std::max(2 * bufferSize, 1);
            T* newBuffer = new T[newSize];
            for (int i = 0; i < realSize; ++i) newBuffer[i] = buffer[i];
            delete[] buffer;
            buffer = newBuffer;
            bufferSize = newSize;
        };
    };

    // Двоичная куча
    template <typename T, typename Comparator>
    class Heap {
     private:
        Array<T> arr;
        Comparator cmp;

     public:
        Heap(){};
        Heap(const Array<T>& arr) : arr(arr) { buildHeap(); };
        int getSize() const { return arr.getSize(); };
        void add(T element) {
            arr.add(element);
            siftUp(arr.getSize() - 1);
        };
        T extractTop() {
            T value = arr[0];
            arr[0] = arr[arr.getSize() - 1];
            arr.popBack();
            siftDown(0);
            return value;
        };
        T top() const { return arr[0]; };

     private:
        void buildHeap() {
            for (int i = arr.getSize() / 2 - 1; i >= 0; --i) siftDown(i);
        };
        void siftUp(int index) {
            int parent = (index - 1) / 2;
            if (cmp(arr[parent], arr[index])) {
                std::swap(arr[parent], arr[index]);
                siftUp(parent);
            }
        };
        void siftDown(int index) {
            int left = 2 * index + 1, right = 2 * index + 2;
            int pushedIndex = index;
            if (left < arr.getSize() && cmp(arr[pushedIndex], arr[left])) pushedIndex = left;
            if (right < arr.getSize() && cmp(arr[pushedIndex], arr[right])) pushedIndex = right;
            if (pushedIndex != index) {
                std::swap(arr[pushedIndex], arr[index]);
                siftDown(pushedIndex);
            }
        };
    };

}  // namespace MySTL

void writeToBuffer(IInputStream& input, std::vector<byte>& buffer) {
    byte value;
    while (input.Read(value)) {
        buffer.push_back(value);
    }
}

std::vector<PairByteFrequency> countByteFrequencies(const std::vector<byte>& arr) {
    byte minValue = arr[0];
    byte maxValue = arr[0];
    for (size_t i = 1; i < arr.size(); ++i) {
        minValue = std::min(minValue, arr[i]);
        maxValue = std::max(maxValue, arr[i]);
    }

    int countBufSize = maxValue - minValue + 1;
    size_t* countBuf = new size_t[countBufSize]();

    for (size_t i = 0; i < arr.size(); ++i) {
        countBuf[arr[i] - minValue]++;
    }
    std::vector<PairByteFrequency> result;
    result.reserve(countBufSize);
    for (int i = 0; i < countBufSize; ++i) {
        if (countBuf[i] > 0) {
            result.push_back(std::make_pair(i + minValue, countBuf[i]));
        }
    }
    delete[] countBuf;
    return result;
}

using Code = std::vector<byte>;

class BitWriter {
 public:
    BitWriter() : bitCount(0) {}

    void WriteBit(unsigned char bit) {
        if (bitCount % 8 == 0) {
            buffer.push_back(0);
        }
        if (bit) {
            buffer[bitCount / 8] |= 1 << (7 - bitCount % 8);
        }
        bitCount++;
    }

    void WriteByte(unsigned char value) {
        if (bitCount % 8 == 0) {
            buffer.push_back(value);
        } else {
            int offset = bitCount % 8;
            buffer[bitCount / 8] |= value >> offset;
            buffer.push_back(value << (8 - offset));
        }
        bitCount += 8;
    }

    const std::vector<unsigned char>& GetBuffer() const { return buffer; }

    size_t GetBitCount() const { return bitCount; }

    void WriteNBits(unsigned char N, unsigned char value) {
        for (unsigned char i = 8 - N; i < 8; ++i) {
            WriteBit(getNthBit(value, i));
        }
    }
    bool setBit(size_t position, unsigned char value) {
        if (position >= bitCount) return false;
        if (getNthBit(buffer[position / 8], position % 8) != value) {
            buffer[position / 8] ^= (1 << (7 - position % 8));
        }
        return true;
    }
    bool setNBits(size_t position, unsigned char N, unsigned char value) {
        for (unsigned char i = 0; i < N; ++i) {
            if (!setBit(position + i, getNthBit(value, 8 - N + i))) return false;
        }
        return true;
    }

 private:
    unsigned char getNthBit(unsigned char value, unsigned char N) { return (value >> (7 - N)) & 1; }
    std::vector<unsigned char> buffer;
    size_t bitCount;
};

class BitReader {
 public:
    BitReader(const std::vector<unsigned char>& buffer) : bitPos(0), buffer(buffer) {}

    bool ReadBit(unsigned char& bit) {
        if (bitPos >= 8 * buffer.size()) return false;
        bit = getNthBit(buffer[bitPos / 8], bitPos % 8);
        bitPos++;
        return true;
    }

    bool ReadByte(unsigned char& value) {
        if (bitPos + 7 >= 8 * buffer.size()) return false;
        if (bitPos % 8 == 0) {
            value = buffer[bitPos / 8];
        } else {
            int offset = bitPos % 8;
            value = 0;
            value |= buffer[bitPos / 8] << offset;
            value |= buffer[bitPos / 8 + 1] >> (8 - offset);
        }
        bitPos += 8;
        return true;
    }

    bool ReadNBits(int N, unsigned char& value) {
        if (bitPos >= 8 * buffer.size() - N + 1 || N > 8) return false;
        unsigned char bit{};
        for (int i = 0; i < N - 1; ++i) {
            ReadBit(bit);
            value += bit;
            value = value << 1;
        }
        ReadBit(bit);
        value += bit;
        return true;
    }

    const std::vector<unsigned char>& GetBuffer() const { return buffer; }

    size_t GetBitPos() const { return bitPos; }

 private:
    unsigned char getNthBit(unsigned char value, unsigned char N) { return (value >> (7 - N)) & 1; }
    std::vector<unsigned char> buffer;
    size_t bitPos;
};

class HuffmanTree {
 private:
    enum class NodeState {
        inner,
        leaf
    };

    struct Node {
        NodeState state;
        byte value;
        size_t frequency;
        Node* left;
        Node* right;
        Node(byte value = 0, size_t frequency = 0)
            : value(value), frequency(frequency), state(NodeState::leaf), left(nullptr), right(nullptr){};
        Node(size_t frequency, Node* left, Node* right) : frequency(frequency), state(NodeState::inner), left(left), right(right){};
    };

    class NodeComparator {
     public:
        bool operator()(Node* const l, Node* const r) { return l->frequency > r->frequency; }
    };

 public:
    HuffmanTree(const std::vector<PairByteFrequency>& pairs) {
        // Создаем кучу с приоритетом
        MySTL::Heap<Node*, NodeComparator> heap;

        // Добавляем в нее пары (буква, частотность)
        for (auto p : pairs) {
            if (p.second > 0) {
                heap.add(new Node(p.first, p.second));
            }
        }

        while (heap.getSize() > 1) {
            Node* left = heap.extractTop();
            Node* right = heap.extractTop();
            heap.add(new Node(left->frequency + right->frequency, left, right));
        }

        // Оставшийся узел - корень дерева Хаффмана
        root = heap.extractTop();
    }

    HuffmanTree(const HuffmanTree&) = delete;

    ~HuffmanTree() {
        postOrderDFS(root, [](Node* node) { delete node; });
    }

 public:
    std::unordered_map<byte, Code> makeTable() {
        std::unordered_map<byte, Code> table;
        if (root->state == NodeState::leaf) {  // В случае если всего 1 слово
            table[root->value] = {0};
        } else {
            buildTable(root, {}, table);
        }
        return table;
    }

 private:
    void buildTable(Node* current, const Code& pathCode, std::unordered_map<byte, Code>& table) {
        if (current->state == NodeState::leaf) {
            table[current->value] = pathCode;
        } else {
            std::vector<byte> newPath(pathCode);
            newPath.push_back(0);
            buildTable(current->left, newPath, table);
            newPath.pop_back();
            newPath.push_back(1);
            buildTable(current->right, newPath, table);
        }
    }

 public:
    // Кодирование дерева
    void encode(BitWriter& bitWriter) { encodeInternal(root, bitWriter); }

 private:
    void encodeInternal(Node* current, BitWriter& bitWriter) {
        if (current->state == NodeState::leaf) {
            bitWriter.WriteBit(0);
            bitWriter.WriteByte(current->value);
        } else {
            bitWriter.WriteBit(1);
            encodeInternal(current->left, bitWriter);
            encodeInternal(current->right, bitWriter);
        }
    }

 public:
    // Декодирование дерева
    HuffmanTree(BitReader& bitReader) { root = decodeInternal(bitReader); }

 private:
    Node* decodeInternal(BitReader& bitReader) {
        Node* node = new Node;
        unsigned char bit{};
        bitReader.ReadBit(bit);
        if (bit == 0) {
            node->state = NodeState::leaf;
            bitReader.ReadByte(node->value);
        } else {
            if (bit == 1) {
                node->state = NodeState::inner;
                node->left = decodeInternal(bitReader);
                node->right = decodeInternal(bitReader);
            }
        }
        return node;
    }

    template <typename Function>
    void postOrderDFS(Node* root, Function visit) {
        Node* current = root;
        std::stack<Node*> s;
        Node* lastVisitedNode;
        while (!s.empty() || current != nullptr) {
            if (current != nullptr) {
                s.push(current);
                current = current->left;
            } else {
                Node* top = s.top();
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

    Node* root;
    friend class Decoder;
};

struct ArchiveParameters {
    unsigned char neededToDecode = 0;  // = 0, в случае если закодированная версия весит больше
    unsigned char insignificantBitsNumber = 0;  // количество незначащих нулевых бит в конце
};

class Encoder {
 public:
    static void encode(const std::vector<byte>& original, const std::unordered_map<byte, Code>& table, BitWriter& bitWriter) {
        byte value;
        for (const auto value : original) {
            for (auto bit : table.at(value)) {
                bitWriter.WriteBit(bit);
            }
        }
    };

    static void writeParams(ArchiveParameters params, BitWriter& bitWriter) {
        bitWriter.setBit(0, params.neededToDecode);
        bitWriter.setNBits(1, 3, params.insignificantBitsNumber);
    }

 private:
    std::vector<byte> buffer;
};

void writeToOutput(const std::vector<byte>& buffer, IOutputStream& output) {
    for (const auto value : buffer) {
        output.Write(value);
    }
}

void Encode(IInputStream& original, IOutputStream& compressed) {
    std::vector<byte> buffer;
    writeToBuffer(original, buffer);
    if (buffer.size() == 0) {  // Если сообщение пустое
        return;
    }

    std::vector<PairByteFrequency> pairs = countByteFrequencies(buffer);
    HuffmanTree tree(pairs);

    BitWriter bitWriter;

    // Выделяем место для параметров архивации
    bitWriter.WriteNBits(4, 0);

    // Кодируем дерево
    tree.encode(bitWriter);

    std::unordered_map<byte, Code> table = tree.makeTable();

    // Кодируем сообщение
    Encoder::encode(buffer, table, bitWriter);

    // Узнаем параметры
    ArchiveParameters params;
    params.insignificantBitsNumber = bitWriter.GetBuffer().size() * 8 - bitWriter.GetBitCount();
    if (bitWriter.GetBuffer().size() > buffer.size()) {
        params.neededToDecode = 0;
    } else {
        params.neededToDecode = 1;
    }
    Encoder::writeParams(params, bitWriter);
    if (params.neededToDecode) {
        writeToOutput(bitWriter.GetBuffer(), compressed);
    } else {
        BitWriter newBW;
        newBW.WriteBit(0);
        for (const auto b : buffer) {
            newBW.WriteByte(b);
        }
        writeToOutput(newBW.GetBuffer(), compressed);
    }
}

class Decoder {
 public:
    static void decode(BitReader& compressed, const HuffmanTree& tree, ArchiveParameters params, IOutputStream& original) {
        HuffmanTree::Node* current = tree.root;
        if (tree.root->state == HuffmanTree::NodeState::leaf) {  // Если только 1 слово
            for (size_t i = compressed.GetBitPos(); i < compressed.GetBuffer().size() * 8 - params.insignificantBitsNumber; i++) {
                original.Write(tree.root->value);
            }
            return;
        }
        unsigned char bit{};
        while (true) {
            while (current->state != HuffmanTree::NodeState::leaf) {
                if (!compressed.ReadBit(bit)) return;
                if (bit == 0) {
                    current = current->left;
                } else {
                    current = current->right;
                }
            }
            original.Write(current->value);
            current = tree.root;
            if (compressed.GetBuffer().size() * 8 - compressed.GetBitPos() == params.insignificantBitsNumber) return;
        }
    }
    static ArchiveParameters readParams(BitReader& bitReader) {
        ArchiveParameters params;
        bitReader.ReadBit(params.neededToDecode);
        if (params.neededToDecode) bitReader.ReadNBits(3, params.insignificantBitsNumber);
        return params;
    }
};

void writeToOutput(BitReader& bitReader, IOutputStream& output) {
    byte value;
    while (bitReader.ReadByte(value)) {
        output.Write(value);
    }
}

void Decode(IInputStream& compressed, IOutputStream& original) {
    std::vector<byte> buffer;
    writeToBuffer(compressed, buffer);

    BitReader bitReader(buffer);
    ArchiveParameters params = Decoder::readParams(bitReader);

    if (params.neededToDecode) {
        HuffmanTree tree(bitReader);
        // Декодируем сообщение
        Decoder::decode(bitReader, tree, params, original);
    } else {
        writeToOutput(bitReader, original);
    }
}
