#include <iostream>
#include <string>
#include <boost/smart_ptr.hpp>
using namespace std;
using namespace boost;


/*
scoped_ptr 包装了new操作符在堆上分配的动态对象，能够保证动态创建的对象在任何时候都可以被正确地删除
scoped_ptr 的所有权更加严格，不能转让，一旦获取了对象的管理权，就无法再从它那里收回来
只能在本作用域里使用，不希望被转让
 */

/*
template <class T>
class scoped_ptr{
  private:
    T* ptr;                     // 原始指针
    scoped_ptr(scoped_ptr const&);  // 拷贝构造函数私有化
    scoped_ptr& operator==(scoped_ptr const&) const; // 赋值操作私有化
    void operator!=(scoped_ptr const&); const;  // 不等操作私有化

 public:
    explicit scoped_ptr(T* p = 0);  // 显式构造函数
    ~scoped_ptr();

    void reset(T* p = 0);        // 重置智能指针

    T& operator*() const;       // 操作符重载
    T* operator->() const;      // 操作符重载
    T* get() const;           // 获取原始指针

    explicit operator bool() const; // 显式bool值转型
    void swap(scoped_ptr& b);       // 交换指针
};

template <typename T> inline
bool operator==(scoped_ptr<T> const& p, boost::detail::sp_nullptr_t);
*/
int main(int argc ,char* argv[]) {
  scoped_ptr<string> sp(new string("text"));  // 构造一介scoped_ptr对象
  assert(sp);      // 使用显式bool转换
  assert(sp != nullptr);  // 空指针比较操作

  cout << *sp << endl;  // operator* 取字符串的内容
  cout << sp->size() << endl; // operator->

  return 0;
}