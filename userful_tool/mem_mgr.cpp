#include <iostream>
#include <chrono>
#include <string>
#include<ext/pool_allocator.h>
using namespace std;

void mem_test() {
	// C
	void* p1 = malloc(512);
	*(int*)p1 = 100;
	cout << *(int*)p1 << endl;
	free(p1);

	// C++ 表达式
	int* p2 = new int(10);
	cout << *p2 << endl;
	delete p2;

	// C++ 函数
	void* p3 = ::operator new(512);
	*(int*)p3 = 103;
	cout << *(int*)p3 << endl;
	::operator delete(p3);

	printf("hello gcc %d\n", __GNUC__);
	// C++ 标准库
	int* p4 = allocator<int>().allocate(7);
	*p4 = 9;
	cout << *p4 << endl;
    int* p5 = __gnu_cxx::__pool_alloc<int>().allocate(9);
    *p5 = 10;
    cout << *p5 << endl;
    __gnu_cxx::__pool_alloc<int>().deallocate((int*)p5, 9);

}

int main(int argc, char* argv[]) {
	mem_test();
	return 0;
}