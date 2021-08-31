#include <iostream>
#include <thread>
using namespace std;

/*
    join() 等待线程完成任务
    join()行为会清理所有与该线程使用的相关存储，thread对象不再与任何线程相关联
    join()只能被调用一次
*/

void print() {
	cout << "hello thread" << endl;
}

int main(int argc, char* argv[]) {
	thread th(print);
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
	} {
		cout << "second not joinable!" << endl;
	}

	return 0;
}

