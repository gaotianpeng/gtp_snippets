/*
*	固定大小对象的内存池
*/
#include <iostream>
#include <chrono>
using namespace std;

template <typename T>
class MemoryPool {
public:
    MemoryPool(size_t  size = EXPANSION_SIZE);
    ~MemoryPool();

    inline void* alloc(size_t size);
    inline void free(void* someElement);

private:
    MemoryPool<T>* next_ = nullptr;
	enum { EXPANSION_SIZE = 32};
	void expandTheFreeList(int howMany = EXPANSION_SIZE);
};

template <typename T>
MemoryPool<T>::MemoryPool(size_t size) {
    expandTheFreeList(size);
}

template <typename T>
MemoryPool<T>::~MemoryPool() {
    MemoryPool<T>* nextPtr = next_;
    for (nextPtr = next_; nextPtr != nullptr; nextPtr = next_) {
        next_ = next_->next_;
        delete[] nextPtr;
    }
}

template <typename T>
inline void* MemoryPool<T>::alloc(size_t size) {
    if (!next_) {
        expandTheFreeList();
    }

    MemoryPool<T>* head = next_;
    next_ = head->next_;

    return head;
}

template <typename T>
inline void MemoryPool<T>::free(void* doomed) {
    MemoryPool<T>* head = static_cast<MemoryPool<T>*>(doomed);

    head->next_ = next_;
    next_ = head;
}

template<typename T>
void MemoryPool<T>::expandTheFreeList(int howMany) {
    size_t size = (sizeof(T) > sizeof(MemoryPool<T>*)) ? sizeof(T) : sizeof(MemoryPool<T>*);

    void* p = new char[size];
    MemoryPool<T>* runner = static_cast<MemoryPool<T>*>(p);
    next_ = runner;
    for (int i = 0; i < howMany; i++) {
        void* p = new char[size];
        runner->next_ = static_cast<MemoryPool<T>*>(p);
        runner = runner->next_;
    }

    runner->next_ = nullptr;
}

class Rational {
public:
    Rational(int a = 0, int b = 1):n(a), d(b) {}

    void* operator new(size_t size) {
        return memPool->alloc(size);
    }

    void operator delete(void* doomed, size_t size) {
        memPool->free(doomed);
    }

    static void newMemPool() {
        memPool = new MemoryPool<Rational>();
    }

    static void deleteMemPool() {
        delete memPool;
        memPool = nullptr;
    }

private:
    int n;
    int d;

    static MemoryPool<Rational>* memPool;
};

MemoryPool<Rational>* Rational::memPool = nullptr;

int main(int argc, char* argv[]) { 
    Rational* array[1000];
    Rational::newMemPool();
    for (int j = 0; j < 1000; j++) {
        for (int i = 0; i < 1000; i++) {
            array[i] = new Rational(i);
        }

        for (int i = 0; i < 1000; i++) {
            delete array[i];
        }
    }

    return 0;
}