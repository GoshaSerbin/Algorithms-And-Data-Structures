/*
5_3. Закраска прямой 1.
На числовой прямой окрасили N отрезков.
Известны координаты левого и правого концов каждого отрезка (Li и Ri).
Найти длину окрашенной части числовой прямой.
*/

#include <iostream>

// Класс, содержащий информацию об изменении толщины линии
struct ThicknessChange {
 public:
    double x;          // Координата, в которой меняется толщина
    bool makeThicker;  // 1 - становится толще, 0 - становится тоньше
    ThicknessChange(double x = 0.0, bool makeThicker = false) : x(x), makeThicker(makeThicker){};
};

class ThicknessChangeComparator {
 public:
    bool operator()(const ThicknessChange& l, const ThicknessChange& r) { return l.x < r.x; }
};

template <typename T, typename Comparator>
void merge(T* leftArr, int leftLen, T* rightArr, int rightLen, Comparator cmp, T* tempArr) {
    int leftIndex = 0, rightIndex = 0;
    while (leftIndex < leftLen && rightIndex < rightLen) {
        if (cmp(leftArr[leftIndex], rightArr[rightIndex])) {
            tempArr[leftIndex + rightIndex] = leftArr[leftIndex];
            ++leftIndex;
        } else {
            tempArr[leftIndex + rightIndex] = rightArr[rightIndex];
            ++rightIndex;
        }
    }
    for (; leftIndex < leftLen; ++leftIndex) tempArr[leftIndex + rightIndex] = leftArr[leftIndex];
    for (; rightIndex < rightLen; ++rightIndex) tempArr[leftIndex + rightIndex] = rightArr[rightIndex];
}

// Глубина рекурсии log(N)
template <typename T, typename Comparator>
void mergeSort(T* arr, int arrLen, Comparator cmp) {
    if (arrLen <= 1) return;
    int leftLen = arrLen / 2;
    int rightLen = arrLen - leftLen;
    mergeSort(arr, leftLen, cmp);
    mergeSort(arr + leftLen, rightLen, cmp);
    T* tempArr = new T[arrLen];
    merge(arr, leftLen, arr + leftLen, rightLen, cmp, tempArr);
    for (int i = 0; i < arrLen; ++i) arr[i] = tempArr[i];
    delete[] tempArr;
}

void findColoredLength(ThicknessChange* changes, int N) {
    int currentThickness = 0;
    double coloredLength = 0;  // Длина закрашенного участка
    int lastNonzeroThicknessIndex = 0;
    for (int i = 0; i < 2 * N; ++i) {
        if (changes[i].makeThicker)
            ++currentThickness;
        else
            --currentThickness;
        if (currentThickness == 0) {
            coloredLength += changes[i].x - changes[lastNonzeroThicknessIndex].x;
            lastNonzeroThicknessIndex = i + 1;
        }
    }
    std::cout << coloredLength;
}

int main() {
    int N = 0;  // N отрезков
    std::cin >> N;
    ThicknessChange* changes = new ThicknessChange[2 * N];  // 2*N изменений
    for (int i = 0; i < 2 * N; i += 2) {
        double l = 0, r = 0;
        std::cin >> l >> r;
        changes[i] = ThicknessChange(l, true);
        changes[i + 1] = ThicknessChange(r, false);
    }
    mergeSort(changes, 2 * N, ThicknessChangeComparator());
    findColoredLength(changes, N);
    delete[] changes;
    return 0;
}