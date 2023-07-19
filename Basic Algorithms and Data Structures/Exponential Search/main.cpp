/*
    Дан массив целых чисел А[0..n - 1].
    Известно, что на интервале [0, m] значения массива строго возрастают,
    а на интервале [m, n - 1] строго убывают. Найти m за O(log m).
    Требования:  Время работы O(log m).
    Внимание! В этой задаче сначала нужно определить диапазон для бинарного поиска
    размером порядка m с помощью экспоненциального поиска,
    а потом уже в нем делать бинарный поиск.
*/

#include <iostream>

void findRange(int* A, int n, int& left, int& right) {
    if (A[1] < A[0]) {  // Отдельно проверяем случай m = 0
        left = right = 0;
        return;
    }
    left = 1, right = 2;
    while (right < n) {
        if (A[left - 1] < A[left] && A[right - 1] > A[right])
            return;
        else {
            left = right;
            right *= 2;
        }
    }
    right = n - 1;
}

int binarySearch(int* A, int n, int left, int right) {
    while (left < right) {
        int mid = (left + right) / 2;
        if (A[mid] < A[mid + 1])
            left = mid + 1;
        else
            right = mid;
    }
    return left;
}

int findM(int* A, int n) {
    int left = 0, right = 0;
    findRange(A, n, left, right);
    return binarySearch(A, n, left, right);
}

int main() {
    int n = 0;
    std::cin >> n;
    int* A = new int[n];
    for (int i = 0; i < n; ++i) std::cin >> A[i];
    std::cout << findM(A, n);
    delete[] A;
    return 0;
}