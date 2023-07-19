/*
1.3 Если в числе содержится только один бит со значением 1,
    записать в выходной поток OK. Иначе записать FAIL
*/

#include <iostream>

bool haveOnlyOneNonzeroBit(unsigned int number) {
    bool alreadyFound = false;  // Был ли уже найден ненулевой бит
    while (number != 0) {
        if (number & 1) {
            if (!alreadyFound)
                alreadyFound = true;
            else
                return false;
        }
        number = number >> 1;
    }
    return alreadyFound;
}

int main() {
    unsigned int number;
    std::cin >> number;
    std::cout << (haveOnlyOneNonzeroBit(number) ? "OK" : "FAIL");
    return 0;
}