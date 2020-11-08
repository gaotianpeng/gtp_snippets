#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <cstdio>
using namespace std;

/*
shared_ptr
    多个smart pointer可以指向相同对象，该对象和其相关资源会在最后
    一个reference被销毁时被释放
unique_ptr
    保证同一时间内只有一个smart pointer可以指向该对象，可以移交拥有权
*/

class FileDeleter {
public:
    FileDeleter(const std::string& fn):file_name_(fn) {}

    void operator() (std::ofstream* fp) {
        fp->close();
        std::remove(file_name_.c_str());
    }
private:
    std::string file_name_;
};

int main(int argc, char* argv[]) {
    shared_ptr<string> p_nice(new string("nice"));
    shared_ptr<string> p_jutta(new string("jutta"));

    (*p_nice)[0] = 'N';
    cout << *p_nice << endl;
    p_jutta->replace(0, 2, "JJ");
    cout << *p_jutta << endl;

    vector<shared_ptr<string>> ptrs;
    ptrs.push_back(p_jutta);
    ptrs.push_back(p_jutta);
    ptrs.push_back(p_nice);
    ptrs.push_back(p_jutta);
    ptrs.push_back(p_nice);

    for (auto ptr : ptrs) {
        cout << *ptr << " ";
    }
    cout << endl;
    *p_nice = "Nicolai";
    for (auto ptr : ptrs) {
        cout << *ptr << " ";
    }
    cout << endl;
    cout << "use_conut:" << ptrs[0].use_count() << endl;

    shared_ptr<string> ptr1{ new string("ptr1") };
    shared_ptr<string> ptr2{ new string("ptr2") };
    shared_ptr<string> ptr3;
    ptr3.reset(new string("ptr3"));

    // 定义一个Delete
    shared_ptr<string> ptr4(new string("ptr4"), 
        [](string* p) {
        cout << "delete " << *p << endl;
        delete p;
    });
    ptr4 = nullptr;

    // 对付Array
    std::shared_ptr<int> ptr5(new int[10], [](int* p) {
        delete[] p;
    });

    // 对于unique_ptr必须明确给予第二个template实参
    std::unique_ptr<int, void(*)(int*)> ptr6(new int[10], [](int* p) {
        delete[] p;
    });

    // file deleter
    std::shared_ptr<std::ofstream> fp(new std::ofstream("tmp_file.txt"),
                FileDeleter("tmp_file.txt"));

    return 0;
}