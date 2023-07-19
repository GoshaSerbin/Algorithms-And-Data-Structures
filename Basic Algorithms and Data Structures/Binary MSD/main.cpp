/*
7_3. Binary MSD для long long.
Дан массив неотрицательных целых 64-разрядных чисел.
Количество чисел не больше 106.
Отсортировать массив методом MSD по битам (бинарный QuickSort).
*/

#include <cassert>
#include <iostream>

int getNthBit(size_t number, int N) {
    return (number >> N) & 1;
}

// Глубина рекурсии <= 64
void binarySort(size_t* arr, int left, int right, int bitPos) {
    int i = left, j = right;
    while (true) {
        while (i <= right && getNthBit(arr[i], bitPos) == 0) ++i;
        while (j >= left && getNthBit(arr[j], bitPos) == 1) --j;
        if (i >= j) {
            break;
        }
        std::swap(arr[i], arr[j]);
        ++i;
        --j;
    }
    if (bitPos == 0) return;
    if (j > left) binarySort(arr, left, j, bitPos - 1);
    if (j + 1 < right) binarySort(arr, j + 1, right, bitPos - 1);
}

void binaryMSD(size_t* arr, size_t n) {
    assert(arr != nullptr);
    binarySort(arr, 0, n - 1, sizeof(size_t) * 8 - 1);
}

int main() {
    int n = 0;
    std::cin >> n;

    size_t* arr = new size_t[n];
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }

    binaryMSD(arr, n);

    for (int i = 0; i < n; ++i) {
        std::cout << arr[i] << " ";
    }
    delete[] arr;
    return 0;
}