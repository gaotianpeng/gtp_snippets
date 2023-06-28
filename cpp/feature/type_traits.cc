#include <iostream>
#include <type_traits>

using namespace std;

/*
    通过 type_traits 可以实现编译期计算、查询、判断、转换和选择
    type_traits 选择功能，可以在一定程度上消除冗长的 switch-case 或 if-else 语句
    type_traits 类型判断功能，在编译期可以检查出是否是正确的类型，以编写更安全的代码
    type_traits 和可变参数模板结合，实现更强大的编译期计算功能

*/
namespace TypeTraits {

// c++11 之前，在类中定义编译期常量的方法
template <typename Type>
struct GetLeftSize {
    static const int value = 1;
};
template <typename Type>
struct GetLeftSize1 {
    enum { value = 1};
};

// c++11 定义编译期常量
template<typename Type>
struct GetLeftSize2: std::integral_constant<int, 1> {
};
/*
template <typename T, T v>
struct integral_constant {
    static const T value = v;
    typedef T value_type;
    typedef integral_constant<T, V> type;
    operator value_type() {return value; }
};
*/

class A {};
class B : public A{};
class C {};

void test() {
    {
        cout << GetLeftSize<int>::value << endl;
        cout << GetLeftSize1<int>::value << endl;
        cout << GetLeftSize2<int>::value << endl;
    }

    {
        cout <<  "int: " << boolalpha << std::is_const<int>::value << std::endl;
        cout <<  "const int: " << boolalpha << std::is_const<const int>::value << std::endl;
        cout <<  "const int&: " << boolalpha << std::is_const<const int&>::value << std::endl;
        cout <<  "const int*: " << boolalpha << std::is_const<const int*>::value << std::endl;
        cout <<  "int* const: " << boolalpha << std::is_const<int* const>::value << std::endl;
    }

    {
        // is_same 用来在编译期判断两种类型是否相同
        cout << boolalpha << std::is_same<int, int>::value << endl;
        cout << boolalpha << std::is_same<int, unsigned int>::value << endl;
        cout << boolalpha << std::is_same<int, signed int>::value << endl;
    }
    {
        // is_base_of 用来在编译期判断两种类型是否为继承关系
        cout << boolalpha << std::is_base_of<A, B>::value << endl;
        cout << boolalpha << std::is_base_of<B, A>::value << endl;
        cout << boolalpha << std::is_base_of<C, B>::value << endl;
    }
    {
        // is_convertible 用来判断前面的模板参数能否转换为后面的模板参数类型
        bool b2a = std::is_convertible<B*, A*>::value;
        bool a2b = std::is_convertible<A*, B*>::value;
        bool b2c = std::is_convertible<B*, C*>::value;
        cout << std::boolalpha;
        cout << b2a << endl;
        cout << a2b << endl;
        cout << b2c << endl;
    }
}


}  // namespace TypeTraits

int main(int argc, char* argv[]) {
    cout << "hello world" << endl;
    TypeTraits::test();

    return 0;
}