#include <iostream>
#include <algorithm>
#include <utility>
using namespace std;

bool int_ptr_less(int *a, int *b) {
    return *a < *b;
}
/*
namespace std {
    template <typename T>
    inline void swap(T&a, T&b) {
        T tmp(std::move(a));
        a = std::move(b);
        b = std::move(tmp);
    }
}
*/

int main(int argc, char* argv[]) {
    int x = 17;
    int y = 42;
    int z = 33;
    int* px = &x;
    int* py = &y;
    int* pz = &z;

    int* pmax = std::max(px, py, int_ptr_less);
    cout << *pmax << endl;

    std::pair<int*, int*> extremes = std::minmax({px, py, pz}, int_ptr_less);
    cout << *extremes.first << " " << *extremes.second << endl;

    int i = 1, j = 2;
    swap(i, j);
    cout << i << " " << j << endl;
    
    return 0;
}