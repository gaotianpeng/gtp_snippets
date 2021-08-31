#include <iostream>
#include <thread>
using namespace std;

/*
    join() 等待线程完成任务
    join()行为会清理所有与该线程使用的相关存储，thread对象不再与任何线程相关联
    join()只能被调用一次
*/

void Print() {
	cout << "hello thread" << endl;
}

void JoinTest() {
	thread th(Print);
	if (th.joinable()) {
		th.join();
		cout << "first joinable!" << endl;
	}
	else {
		cout << "first not joinable!" << endl;
	}

	if (th.joinable()) {
		th.join();
		cout << "second joinable!" << endl;
	} 
	else {
		cout << "second not joinable!" << endl;
	}
}

void DoSomething(int& i) {
	++i;
}

struct Func {
	int& i_;
	Func(int& i): i_(i) {}

	void operator()() {
		for (unsigned j = 0; j < 1000000; ++j) {
			DoSomething(i_);
		}
	}
};

void DoSomethingInCurrentThread() {
}

void JoinException() {
	int local_state = 0;
	Func my_func(local_state);
	std::thread th(my_func);
	try {
		DoSomethingInCurrentThread();
	} 
	catch(...) {
		th.join();
		throw;
	}
	th.join();
}

int main(int argc, char* argv[]) {
	JoinTest();
	JoinException();
	return 0;
}

