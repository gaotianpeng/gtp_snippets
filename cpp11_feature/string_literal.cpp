#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    /*
        row string：允许我们定义字符序列，省下很多用来装饰特殊字符的符号
    */
    cout << R"(\\n)" << endl;

    /*
        R"delim(...)delim"
    */
    cout << R"nc(a\b\nc())nc" << endl;

    /*
        Encode String Literal
            u8定义一个UTF-8编码(const char)
            u定义一个 char16_t string literal
            U定义一个 char32_t string literal
            L定义一个 wchar_t wide string literal
     */
    cout << sizeof(u8"hello world") << endl;
    cout << sizeof(u"hello world") << endl;
    cout << sizeof(U"hello world") << endl;
    cout << sizeof(L"hello world") << endl;

    return 0;
}