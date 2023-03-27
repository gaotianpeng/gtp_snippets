#include <iostream>

class IntArrayIterator {
public:
    IntArrayIterator(int* begin, int* end)
        : current(begin), end(end) {}

    IntArrayIterator& operator++() {
        ++current;
        return *this;
    }

    bool operator!=(const IntArrayIterator& other) const {
        return current != other.current;
    }

    int& operator*() const {
        return *current;
    }

private:
    int* current;
    int* end;
};

class IntArray {
public:
    IntArray(int* data, size_t size)
        : data(data), size(size) {}

    IntArrayIterator begin() const {
        return IntArrayIterator(data, data + size);
    }

    IntArrayIterator end() const {
        return IntArrayIterator(data + size, data + size);
    }

private:
    int* data;
    size_t size;
};

int main() {
    int arr[] = { 1, 2, 3, 4, 5 };
    IntArray intArray(arr, 5);

    for (int i : intArray) {
        std::cout << i << ' ';
    }
    std::cout << std::endl;

    return 0;
}
