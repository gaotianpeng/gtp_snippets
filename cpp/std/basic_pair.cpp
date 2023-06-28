#include <iostream>
#include <utility>
#include <functional>
#include <tuple>
#include <string>
using namespace std;

class Foo {
public:
    Foo(tuple<int, float>) {
        cout << "Foo::Foo(tuple)" << endl;
    }

    template<typename... Args> 
    Foo(Args... args) {
        cout << "Foo::Foo(args...)" << endl;
    }
};

int main(int argc, char* argv[]) {
    tuple<int, float> tu(1, 2.22);
    pair<int, Foo> p1(42, tu);
    // pass three elements of the tuple to the constructor of Foo
    pair<int, Foo> p2(piecewise_construct, make_tuple(42), tu);

    std::string s, t;
    auto p3 = std::make_pair(std::move(s), std::move(t));
    // s and t are no longer used

    int i = 0;
    auto p4 = std::make_pair(std::ref(i), std::ref(i));	// creates pair<int&, itn&>
    ++p4.first;
    ++p4.second;
    cout << "i:" << i << endl;

    std::pair<char, char> p5 = std::make_pair('x', 'y');
    char c;
    std::tie(std::ignore, c) = p5;	// extract second value into c(ignore first one)
    cout << c << endl;
    return 0;
}