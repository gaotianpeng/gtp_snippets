#include <iostream>
#include <limits>
#include <climits>

using namespace std;

void test() {
    {
        long long x = 65536;
        long long x1 = 65536LL;

        long long x2 = 65536 << 16;
        cout << "x2 = " << x2 << std::endl;

        long long x3 = 65536LL << 16;
        cout << "x3 = " << x3 << std::endl; 
    }

    {
        cout << "LL MAX " << LLONG_MAX << endl;
        cout << "LL MIN " << LLONG_MIN << endl;

        cout << "LL MAX " << std::numeric_limits<long long>::max() << endl;
        cout << "LL MIN " << std::numeric_limits<long long>::min() << endl;
    }
}

int main(int argc, char* argv[]) {
    test();

    return 0;
}