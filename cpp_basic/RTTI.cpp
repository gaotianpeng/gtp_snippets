#include <iostream>
using namespace std;
/*
运行时类型识别(run-time type identification, RTTI)
  typeid运算符，用于返回表达式的类型
  dynamic_cast运算符，用于将基本的指针或引用安全地转换成派生类的指针或引用
当将这两个运算符用于某种类型的指针或引用，并且该类型含有虚函数时，运算符将使用指针或引用所绑定对象的动态类型
两个运算符特别适用于以下情况
  想使用基类对象的指针或引用执行某个派生类操作并且该操作不是虚函数
*/
/*
dynamic_cast
  dynamic_cast<type*>(e)
  dynamic_cast<type&>(e)
  dynamic_cast<type&&>(e)
type必须是一个类类型，通常该类型应该含有虚函数
e的类型必须符合以下三个条件中的任意一个
  e的类型是目标type的公有派生类
  e的类型是目标type的公有基类
  e的类型就是目标type的类型
如果符合，转换成功，否则失败
  如果转换目标类型是指针类型且失败了，则结果为0
  如果转换目标是引用类型且失败了，则抛出一个bad_cast异常
 */

/*
typeid: 允许程序向表达式提问：你的对象是什么类型
  typeid(e)
    e可以是任意表达式或类型的名字
    typeid操作结果是一个常量对象的引用，类型是type_info或type_info的公有派生类
    当typeid作用于数组或函数时，不会执行向指针的标准类型转换
*/
class A{
 public:
    virtual ~A() {}
};

class B: public A{
 public:
    virtual ~B(){}
};

class C: public B{
  public:
    virtual ~C(){}
};

class D: public B, public A{
  public:
    virtual ~D(){}
};

void func_ptr_test(A* a) {
  if (C* pc = dynamic_cast<C*>(a)) {
    // todo
  } else {
    // 处理类型转换失败的情况
  }
}

void func_ref_test(const A& a) {
  try {
    const B &b = dynamic_cast<const B &>(a);
    // todo
  } catch(bad_cast) {
    // 处理类型转换失败的情况
  }
}

class Base{
 public:
  virtual ~Base(){}
};

class Derived : public Base {
 public:
  virtual ~Derived(){}
};

int main(int argc, char* argv[]) {
    A* pa = new C;
    B* pb = dynamic_cast<B*>(pa); // success

    A* pa1 = new D;
    B* pb2 = dynamic_cast<B*>(pa2); // cast failed

    Derived* dp = new Derived();
    Base* bp = dp;  // 两个指针都指向Derived对象
    // 运行时比较两个对象的类型
    if (typeid(*bp) == typeid(*dp)) {
      // bp 和 dp指向同一个对象
    }
    // 检查运行时类型是否是某种特定的类型
    if (typeid(*bp) == typeid(Derived)) {
      // bp实际指向Derived对象
    }
    // 下面检查永远是失败的，bp的类型是指向Base的指针
    if (typeid(bp) == typeid(Derived)) {
      // 此处的代码永远不会执行
      // typeid应该作用于对象
    }

    // nullptr 抛出一个bad_typeid异常
    try {
      Derived* bp = nullptr;
      if (typeid(*bp) == typeid(Derived)) {
        // typeid success
      }
    } catch(bad_typeid) {
      // typeid failure
    }
    
    return 0;
}