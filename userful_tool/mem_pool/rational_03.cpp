/*
*	单线程可变大小内存管理器
*/
#include <iostream>
using namespace std;

class MemoryChunk {
public:
    // 首先确定内存块的适当大小，然后从堆上分配私有存储空间
    MemoryChunk(MemoryChunk* nextChunk, size_t reqSize) {
        chunkSize_ = reqSize > DEFAULT_CHUNK_SIZE ? reqSize : DEFAULT_CHUNK_SIZE;
        next_ = nextChunk;
        bytesAlreadyAllocated_ = 0;
        mem_ = static_cast<void*>(new char[chunkSize_]);
    }

    ~MemoryChunk() { delete [] mem_; }

    inline void* alloc(size_t requestSize) {
        void* addr = reinterpret_cast<void*>(
            reinterpret_cast<size_t>(mem_) + bytesAlreadyAllocated_ );
        bytesAlreadyAllocated_ += requestSize;

        return addr;
    }

    inline void free(void* someElement) {}

    MemoryChunk* nextMemChunk() { return next_;  }

    size_t spaceAvaliable() {
        return chunkSize_ - bytesAlreadyAllocated_;
    }

    enum {DEFAULT_CHUNK_SIZE = 4096};

    private:
    MemoryChunk* next_;
    void* mem_;

    size_t chunkSize_;
    size_t bytesAlreadyAllocated_;
};

class ByteMemoryPool {
public:
    ByteMemoryPool(size_t initSize = MemoryChunk::DEFAULT_CHUNK_SIZE) {
        expandStorage(initSize);
    }

    ~ByteMemoryPool() {
        MemoryChunk* memChunk = listOfMemoryChunks_;
        while (memChunk) {
            listOfMemoryChunks_ = memChunk->nextMemChunk();
            delete memChunk;
            memChunk = listOfMemoryChunks_;
        }
    }

    inline void* alloc(size_t requestSize) {
        size_t space = listOfMemoryChunks_->spaceAvaliable();
        if (space < requestSize) {
            expandStorage(requestSize);
        }

        return listOfMemoryChunks_->alloc(requestSize);
    }

    inline void free(void* doomed) {
        listOfMemoryChunks_->free(doomed);
    }

private:
    MemoryChunk* listOfMemoryChunks_;
    void expandStorage(size_t reqSize) {
        listOfMemoryChunks_ = new MemoryChunk(listOfMemoryChunks_, reqSize);
    }
};

class Rational {
public:
    Rational(int a = 0, int b = 1):n(a), d(b) {}

    void* operator new(size_t size) {
        return memPool_->alloc(size);
    }

    void operator delete(void* doomed, size_t size) {
        memPool_->free(doomed);
    }

    static void newMemPool() { memPool_ = new ByteMemoryPool; }
    static void deleteMemPool() { delete memPool_; }

private:
    int n;
    int d;
    static ByteMemoryPool* memPool_;
};

ByteMemoryPool* Rational::memPool_ = nullptr;

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