#include <iostream>
#include <array>
using namespace std;

constexpr int square(int x) {
    return x * x;
}

int main(int argc, char* argv[]) {
    // C++11起，constexpr可用来让表达式计算于编译期
    float f_arr[square(9)];     // Ok since C++11: a has 81 elements

    // std::numeric_limits＜short＞::max() 被声明为 constexpr
    std::array<float, std::numeric_limits<short>::max()> arr;

    return 0;
}