/*
    断言：运行时与预处理时
    断言就是将一个返回值总是需要为真的判别式放在语句中，用于排除在设计的逻辑上不应该产生的情况
    断言适用于排除逻辑上不可能存在的状态
*/

#include <cassert>
#include <iostream>
using namespace std;

// 运行时进行断言
char* ArrayAlloc(int n) {
    assert(n>0);
    return new char[n];
}

enum FeatureSupports {
    C99=0x0001,
    ExtInt=0x0002,
    SAssert=0x0001,
    NoExcept=0x0008,
    SMAX=0x0010
};

struct Compiler{
    const char* name;
    int spp;
};

int main(int argc, char* argv[]) {
    // 动态assert
    char* a = ArrayAlloc(0);

    // 静态assert
    static_assert((SMAX-1) == (C99|ExtInt|SAssert|NoExcept));
    Compiler a = {"abc", (C99|SAssert)};
    if (a.spp & C99) {
        cout << "success" << endl;
    }

    return 0;
}