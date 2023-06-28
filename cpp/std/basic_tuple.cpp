#include <tuple>
#include <iostream>
#include <complex>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
    tuple<string, int, int, complex<double>> t;

    tuple<int, float, string> t1(41, 6.3, "nice");

    cout << get<0>(t1) << " ";
    cout << get<1>(t1) << " ";
    cout << get<2>(t1) << " ";
    cout << endl;

    auto t2 = make_tuple(22, 44, "nice");
    // assign second value in t2 to t1
    get<1>(t1) = get<1>(t2);
    cout << get<1>(t1) << endl;

    // comparison and assignment
    if (t1 < t2) {
        t1 = t2;
    }

    cout << get<0>(t1) << "," << get<1>(t1) << "," << get<2>(t1) << std::endl;
    cout << get<0>(t2) << "," << get<1>(t2) << "," << get<2>(t2) << std::endl;

    string str; 
    tuple<string&> t3(str);	// first element refers to s
    // tuple不是寻常的容器，不允许迭代元素，必须在编译期知道你打算处理的元素的索引值
    get<0>(t3) = "hello";
    cout << str << endl;

    auto t4 = make_tuple(22, 44, "nice");

    string s1;
    auto t5 = make_tuple(ref(s1));
    get<0>(t5) = "my value";
    cout << get<0>(t5) << endl;

    std::tuple<int, float, std::string> t6(77, 1.1, "more light");
    int i{};
    float f{};
    std::string s2;
    std::tie(i, f, s2) = t6;
    cout << i << "," << f << "," << s2 << endl;

    int j{};
    string s3;
    std::tie(j, std::ignore, s3) = t6;
    cout << j << "," << s2 << endl;

    // tuple and initializer list
    std::tuple<int, double> t7(42, 3.14);
    std::tuple<int, double> t8{ 42, 3.14 };
    std::tuple<int, double> t9 = {42. 3.14 };	// error

    return 0;
}