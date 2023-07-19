/*
4.3 Планировщик процессов
В операционной системе Technux есть планировщик процессов.
Каждый процесс характеризуется :
приоритетом P
временем, которое он уже отработал t
временем, которое необходимо для завершения работы процесса T
Планировщик процессов выбирает процесс с минимальным значением P* (t + 1),
выполняет его время P и кладет обратно в очередь процессов.
Если выполняется условие t >= T, то процесс считается завершенным и удаляется из очереди.
Требуется посчитать кол - во переключений процессора.
Формат входных данных:  Сначала вводится кол-во процессов. После этого процессы в формате P T
Формат выходных данных: Кол-во переключений процессора.
*/

#include <cassert>
#include <iostream>

// Процесс
class Process {
 private:
    double P;
    double T;

 public:
    double t;
    double getP() const { return P; };
    double getT() const { return T; };
    friend std::istream& operator>>(std::istream& input, Process& proc);
    Process(double P = 0, double T = 0) : P(P), t(0), T(T){};
};

std::istream& operator>>(std::istream& input, Process& proc) {
    input >> proc.P >> proc.T;
    return input;
}

class ProcessComparator {
 public:
    bool operator()(const Process& l, const Process& r) { return l.getP() * (l.t + 1) > r.getP() * (r.t + 1); }
};

// АТД Динамический массив
template <typename T>
class Array {
 private:
    T* buffer;
    int bufferSize;
    int realSize;  // Количество элементов в массиве
 public:
    Array() : buffer(nullptr), bufferSize(0), realSize(0){};
    ~Array() { delete[] buffer; };

    Array(const Array& arr);
    Array<T> operator=(const Array& arr) = delete;

    Array(Array&& arr) = delete;
    Array<T> operator=(Array&& arr) = delete;

    int getSize() const { return realSize; };
    void add(T element);
    void popBack();
    T operator[](int index) const;
    T& operator[](int index);

 private:
    void grow();
};

template <typename T>
void Array<T>::grow() {
    int newSize = std::max(2 * bufferSize, 1);
    T* newBuffer = new T[newSize];
    for (int i = 0; i < realSize; ++i) newBuffer[i] = buffer[i];
    delete[] buffer;
    buffer = newBuffer;
    bufferSize = newSize;
}

template <typename T>
Array<T>::Array(const Array& arr) {
    bufferSize = realSize = arr.getSize();
    buffer = new T[bufferSize];
    for (int i = 0; i < bufferSize; ++i) {
        buffer[i] = arr[i];
    }
}

template <typename T>
void Array<T>::add(T element) {
    if (realSize == bufferSize) grow();
    buffer[realSize++] = element;
}

template <typename T>
void Array<T>::popBack() {
    assert(realSize > 0);
    --realSize;
}

template <typename T>
T Array<T>::operator[](int index) const {
    assert(0 <= index && index < realSize);
    return buffer[index];
}

template <typename T>
T& Array<T>::operator[](int index) {
    assert(0 <= index && index < realSize);
    return buffer[index];
}

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
    void add(T element);
    T extractTop();
    T top() const { return arr[0]; };

 private:
    void buildHeap();
    void siftUp(int index);
    void siftDown(int index);
};

template <typename T, typename Comparator>
void Heap<T, Comparator>::add(T element) {
    arr.add(element);
    siftUp(arr.getSize() - 1);
}

template <typename T, typename Comparator>
T Heap<T, Comparator>::extractTop() {
    T value = arr[0];
    arr[0] = arr[arr.getSize() - 1];
    arr.popBack();
    siftDown(0);
    return value;
}

template <typename T, typename Comparator>
void Heap<T, Comparator>::siftUp(int index) {
    int parent = (index - 1) / 2;
    if (cmp(arr[parent], arr[index])) {
        std::swap(arr[parent], arr[index]);
        siftUp(parent);
    }
}

template <typename T, typename Comparator>
void Heap<T, Comparator>::siftDown(int index) {
    int left = 2 * index + 1, right = 2 * index + 2;
    int pushedIndex = index;
    if (left < arr.getSize() && cmp(arr[pushedIndex], arr[left])) pushedIndex = left;
    if (right < arr.getSize() && cmp(arr[pushedIndex], arr[right])) pushedIndex = right;
    if (pushedIndex != index) {
        std::swap(arr[pushedIndex], arr[index]);
        siftDown(pushedIndex);
    }
}

template <typename T, typename Comparator>
void Heap<T, Comparator>::buildHeap() {
    for (int i = arr.getSize() / 2 - 1; i >= 0; --i) siftDown(i);
}

int main() {
    int N = 0;
    std::cin >> N;
    Array<Process> arr;
    Process proc;
    for (int i = 0; i < N; ++i) {
        std::cin >> proc;
        arr.add(proc);
    }

    Heap<Process, ProcessComparator> heap(arr);

    int switchCounter = 0;
    while (heap.getSize() != 0) {
        ++switchCounter;
        Process proc = heap.extractTop();
        proc.t += proc.getP();
        if (proc.t < proc.getT()) heap.add(proc);
    }
    std::cout << switchCounter;
    return 0;
}