#include <iostream>
using namespace std;

/*
    当你采用不同的模块或程序库时，经常会出现名称冲突现象，这是因为不同的模块和程序库可能对不同的对象使用相同的标识符(identifier)
    namespace正是用来解决这个问题的
    所谓namespace是指标识符的某种可见范围
    可以使用一个namespace来定义若干组件，而它们可散布于多个实质模块内
    C++标准库中的所有标识符都被定义于一个名为std的namespace内
    以下 namespace 嵌套于 std 内
        std::rel_ops
        std::chrono
        std::placeholders
        std::regex_constants
        std::this_thread
*/

int main(int argc, char* argv[]) {
    /*
        基于namespace带来的概念，欲使用C++标准库的任何标识符，你有三种选择
    */
    // 1:直接指定标识符
    std::cout << std::hex << 3.4 << std::endl;

    // 2: using declaration
    using std::cout; 
    using std::endl;
    cout << std::hex << 3.4 << endl;

    // 3. 使用using directive
    using namespace std;
    cout << hex << 3.4 << endl;
    
    return 0;
}