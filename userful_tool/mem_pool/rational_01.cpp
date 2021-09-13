#include <iostream>
using namespace std;

class NextOnFreeList {
public:
	NextOnFreeList* next = nullptr;
};

class Rational {
public:
	Rational(int a = 0, int b = 1) : n(a), d(b) {}

	inline void* operator new(size_t size) {
		if (nullptr == freeList) {
			expandTheFreeList();
		}

		NextOnFreeList* head = freeList;
		freeList = head->next;

		return head;
	}

	inline void operator delete(void* doomed, size_t size) {
		NextOnFreeList* head = static_cast<NextOnFreeList*>(doomed);
		head->next = freeList;
		freeList = head;
	}

	static void newMemPool() { expandTheFreeList(); }
	static void deelteMemPool() {
		NextOnFreeList* nextPtr;
		for (nextPtr = freeList; nextPtr != NULL; nextPtr = freeList) {
			freeList = freeList->next;
			delete[] nextPtr;
		}
	}

private:
	static NextOnFreeList* freeList;

	static void expandTheFreeList() {
		size_t size = (sizeof(Rational) > sizeof(NextOnFreeList*)) ? sizeof(Rational) : sizeof(NextOnFreeList*);
		void* p = new char[size];
		NextOnFreeList* runner = static_cast<NextOnFreeList*>(p);
		
		freeList = runner;
		for (int i = 0; i < EXPANSION_SIZE; i++) {
			void* p = new char[size];
			runner->next = static_cast<NextOnFreeList*>(p);
			runner = runner->next;
		}

		runner->next = nullptr;
	}

	enum { EXPANSION_SIZE = 32 };

	int n;
	int d;
};

NextOnFreeList* Rational::freeList = nullptr;

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
	Rational::deelteMemPool();

	return 0;
}