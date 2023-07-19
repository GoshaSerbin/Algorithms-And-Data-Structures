#include <bitset>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

typedef unsigned char byte;

#define interface struct

interface IInputStream {
    virtual bool Read(byte& value) = 0;
};

interface IOutputStream {
    virtual void Write(byte value) = 0;
};

class InputStream : public IInputStream {
 public:
    InputStream(std::string input) : input(input), i(0) {}
    bool Read(byte& value) override {
        if (i >= input.size()) return false;
        value = input[i];
        i++;
        return true;
    }

 private:
    std::string input;
    int i = 0;
};

class FileInputStream : public IInputStream {
 public:
    FileInputStream(std::string filename) { file.open(filename); }
    bool Read(byte& value) override {
        if (file >> value)
            return true;
        else
            return false;
    }

 private:
    std::ifstream file;
};

struct OutputStream : public IOutputStream {
    std::stringstream ss;

    void Write(byte value) override { ss << value; }
};
struct FileOutputStream : public IOutputStream {
    std::ofstream file;
    FileOutputStream(std::string filename) { file.open(filename); }
    void Write(byte value) override { file << value; }
};
void Encode(IInputStream& original, IOutputStream& compressed);
void Decode(IInputStream& compressed, IOutputStream& original);

void visualizeByte(byte b) {
    std::cout << std::bitset<8>(b) << "|";
}
int main() {
    // std::ifstream file("text.txt");
    std::string input("hello, world");
    // std::cout << input;
    // getline(file, input);
    for (size_t i = 0; i < 100000; ++i) {
        input += (char)(random() % 256);
    }
    for (auto b : input) {
        //  visualizeByte(b);
    }
    std::cout << std::endl;

    InputStream original(input);
    // FileInputStream original("text.txt");
    OutputStream compressed;
    Encode(original, compressed);

    for (auto b : compressed.ss.str()) {
        // visualizeByte(b);
    }
    std::cout << std::endl;

    InputStream comp(compressed.ss.str());
    // FileOutputStream decomp("text2.txt");
    OutputStream decomp;
    Decode(comp, decomp);
    for (auto b : decomp.ss.str()) {
        // visualizeByte(b);
    }
    std::cout << std::endl;
    assert(decomp.ss.str() == input);
}
