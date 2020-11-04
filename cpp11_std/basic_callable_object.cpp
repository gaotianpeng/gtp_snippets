#include <iostream>
#include <functional>
#include <future>
using namespace std;
/*
Callable Object: 可被某种方式调用其某些函数”的对象, 有以下4种类型
	函数，接受额外传入的 args 作为实参(argument)
	指向成员函数的指针
	函数对象(function object,该对象拥有operator()),附带 args 被传递作为实参
	lambda，一种函数对象
*/

void Func(int x, int y) {
	// ...
}

auto lam_func = [](int x, int y) {
	// ...
};

class FuncClass {
public:
	void operator() (int x, int y) const {
		// ...
	}

	void MemFunc(int x, int y) const {
		// ...
	}
};

int main(int argc, char* argv[]) {
	FuncClass cls;
	std::shared_ptr<FuncClass> sp(new FuncClass);
	
	// bind() uses callable objects to bind arguments
	std::bind(Func,77, 33)();							// calls: Func(77, 33)
	std::bind(lam_func, 77, 33)();						// calls: lam_func(77, 33)
	std::bind(FuncClass(), 77, 33)();					// calls: FuncClass::operator()(77, 33)
	std::bind(&FuncClass::MemFunc, cls, 77, 33)();		// calls: cls.MemFunc(77, 33)
	std::bind(&FuncClass::MemFunc, sp, 77, 33)();		// calls: sp->MemFunc(77, 33)

	// async() uses callable objects to start (background) tasks
	std::async(Func, 77, 33);							// calls: Func(77, 33)
	std::async(lam_func, 77, 33);						// calls: lam_func(77, 33)
	std::async(FuncClass(), 77, 33);					// calls: FuncClass::operator()(77, 33)
	std::async(&FuncClass::MemFunc, cls, 77, 33);		// calls: cls.MemFunc(77, 33)
	std::async(&FuncClass::MemFunc, sp, 77, 33);		// calls: sp->MemFunc(77, 33)

 	return 0;
}