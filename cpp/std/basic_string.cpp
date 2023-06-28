#include <iostream>
#include <string>
using namespace std;
/*
    C++标准库对string的设计思维是，让它的行为尽可能像基础类型
*/

int main(int argc, char* argv[]) {
    /*
        shrink_to_fit: shrink capacity to fit the current size
     */
    string str (100,'x');
    cout << "1. capacity of str: " << str.capacity() << '\n';

    str.resize(10);
    cout << "2. capacity of str: " << str.capacity() << '\n';

    str.shrink_to_fit();
    cout << "3. capacity of str: " << str.capacity() << '\n';

    /*
        data()和c_str()：以字符数组的形式返回string内容，该数组在位置[size()]上有一个end-of-string
            字符，所以其结果是一个内含'\0'字符的有效C-String
        data()和c_str()返回的字符数组由该string拥有, 调用者绝不可以改动它或释放其内存
        copy()将string内容复制到“调用者提供的字符数组”中，其末尾不添加'\0'字
     */
    string s("123456");
    cout << s.c_str() << endl;
    const char* const_s = s.c_str();
    cout << const_s << endl;

    char buffer[100];
    s.copy(buffer, 100);

    /*
     Size and Capacity
        size()和length()返回string的现有字符数。两个函数等效
        max_size()返回“string最多能够包含的字符数”
        capacity()重新分配内存之前，string所能包含的最大字符数
        reverse()缩减实际容量
     */
    string str_1 = "gtp";
    cout << (str_1.size() == str_1.length()) << endl;
    cout << str_1.max_size() << endl;
    cout << str_1.capacity() << endl;
    str_1.resize(100);
    cout << str_1.capacity() << endl;
    str_1.reserve(50) ;
    cout << str_1.capacity() << endl;
    return 0;
}