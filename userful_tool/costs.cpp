#include <iostream>
#include <chrono>
using namespace std;

class Costs {
public:
    Costs(const std::string& msg):t0(std::chrono::high_resolution_clock::now()), msg_(msg) {}

    void CostTime(const std::string& msg) {
        auto t1 = std::chrono::high_resolution_clock::now();
        auto nanos = std::chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();
        cout << msg << ": cost time " <<  nanos << " ms" << endl;
    }

    ~Costs() {
        auto t1 = std::chrono::high_resolution_clock::now();
        auto nanos = std::chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();
        cout << msg_ << ": cost time " <<  nanos << " ms" << endl;
    }

private:
    std::chrono::high_resolution_clock::time_point  t0;
    std::string msg_;
};

int main(int argc, char* argv[]) {
    int N = 100000000;
    long double sum = 0;
    Costs timer("test");
    for (int i = 0; i < N; ++i) {
        sum += i * 3.14159;
    }
    timer.CostTime("haha ");
    for (int i = 0; i < N; ++i) {
        sum += i * 3.14159;
    }

    cout << sum << endl;

    return 0;
}