#include <iostream>
using namespace std;

class noncopyable {
protected:
    constexpr noncopyable() = default;
    ~noncopyable() = default;

    noncopyable(const noncopyable& ) = delete;
    noncopyable& operator=(const noncopyable&) = delete;
};


class Test: public noncopyable {

};



int main(int argc, char* argv[]) {
    Test test_a;
    Test test_b(test_a); // error
    Test test_c = test_a; // error
    return 0;
}