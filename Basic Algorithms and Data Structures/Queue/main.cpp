/*
3_1. Реализовать очередь с динамическим зацикленным буфером.
Требования: Очередь должна быть реализована в виде класса.
*/

#include <iostream>

// АТД очередь
class Queue {
 private:
    int* buffer;
    int bufferSize;
    int realSize;  // Количество элементов в очереди
    int head, tail;

 public:
    Queue() : head(0), tail(0), bufferSize(0), realSize(0), buffer(nullptr){};
    ~Queue() { delete[] buffer; };

    Queue(const Queue& queue) = delete;
    Queue& operator=(const Queue& queue) = delete;

    Queue(Queue&& queue) = delete;
    Queue& operator=(Queue&& queue) = delete;

    bool isEmpty() const { return realSize == 0; };
    bool isFilled() const { return realSize == bufferSize; };

    void enqueue(int element);
    int dequeue();

 private:
    void grow();
};

void Queue::grow() {
    int newSize = std::max(2 * bufferSize, 1);
    int* newBuffer = new int[newSize];
    for (int i = 0; i < bufferSize; i++) newBuffer[i] = buffer[(head + i) % bufferSize];
    head = 0;
    tail = bufferSize - 1;
    delete[] buffer;
    buffer = newBuffer;
    bufferSize = newSize;
}

void Queue::enqueue(int element) {
    if (isFilled()) grow();
    tail = (tail + 1) % bufferSize;
    buffer[tail] = element;
    realSize++;
}

int Queue::dequeue() {
    if (isEmpty()) return -1;
    int element = buffer[head];
    head = (head + 1) % bufferSize;
    realSize--;
    return element;
}

int main() {
    Queue queue;
    int operationCount = 0;
    std::cin >> operationCount;
    for (int i = 0; i < operationCount; i++) {
        int operation = 0, value = 0;
        std::cin >> operation >> value;
        switch (operation) {
            case 2:
                if (queue.dequeue() != value) {
                    std::cout << "NO";
                    return 0;
                }
                break;
            case 3:
                queue.enqueue(value);
                break;
        }
    }
    std::cout << "YES";
    return 0;
}