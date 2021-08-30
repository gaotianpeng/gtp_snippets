#include <iostream>
using namespace std;

template <typename T>
using myfunc_M = int(*)(T, T);

int AddToSum(int i, int j) {
    return i + j;
}

int main(int argc, char* argv[]) {
    myfunc_M<int> func = AddToSum;
    cout << func(1, 2) << endl;

    return 0;
}