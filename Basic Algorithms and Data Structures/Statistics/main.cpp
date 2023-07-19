/*
6_4. Реализуйте стратегию выбора опорного элемента “случайный элемент”.
Функцию Partition реализуйте методом прохода двумя итераторами от конца массива к началу.
*/

#include <functional>
#include <iostream>

template <typename T, typename Comparator = std::less<T>>
int partition(T* arr, int left, int right, Comparator cmp = Comparator()) {
    int pivotPos = left + rand() % (1 + right - left);
    T pivot = arr[pivotPos];
    std::swap(arr[left], arr[pivotPos]);
    int i = right, j = right;

    while (j > left) {
        if (cmp(arr[j], pivot)) {
            --j;
        } else {
            std::swap(arr[i], arr[j]);
            --i;
            --j;
        }
    }
    std::swap(arr[i], arr[left]);
    return i;
}

template <typename T, typename Comparator = std::less<T>>
T kthStatistic(T* arr, int k, int left, int right, Comparator cmp = Comparator()) {
    int pivotPos = partition(arr, left, right, cmp);
    while (pivotPos != k) {
        if (pivotPos > k) {
            right = pivotPos - 1;
            pivotPos = partition(arr, left, right);
        } else {
            left = pivotPos + 1;
            pivotPos = partition(arr, left, right);
        }
    }
    return arr[pivotPos];
}

int main() {
    int n = 0;
    std::cin >> n;
    int* arr = new int[n];
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }
    std::cout << kthStatistic(arr, n / 10, 0, n - 1) << std::endl;
    std::cout << kthStatistic(arr, n / 2, 0, n - 1) << std::endl;
    std::cout << kthStatistic(arr, 9 * n / 10, 0, n - 1) << std::endl;
    delete[] arr;
    return 0;
}