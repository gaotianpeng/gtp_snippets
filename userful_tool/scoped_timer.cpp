#include <iostream>
#include <functional>
#include <chrono>
using namespace std;

class ScopedTimer {
public:
	ScopedTimer(function<void(int)> cb):t0(std::chrono::high_resolution_clock::now()), cb_(cb) {}

	~ScopedTimer() {
		auto t1 = std::chrono::high_resolution_clock::now();
		auto nanos = std::chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();
		cb_(nanos);
	}

private:
	std::chrono::high_resolution_clock::time_point  t0;
	function<void(int)> cb_;
};

void PrintElaps(int n) {
	std::cout << "cost time:" << n <<  " ms" << endl;
}

int main(int argc, char* argv[]) {
	int N = 100000000;
	long double sum = 0;
	ScopedTimer timer(PrintElaps);
	for (int i = 0; i < N; ++i) {
		sum += i * 3.14159;
	}

	cout << sum << endl;

	return 0;
}