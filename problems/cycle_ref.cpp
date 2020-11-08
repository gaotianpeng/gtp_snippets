#include <iostream>
#include <memory>
using namespace std;

struct A {
    shared_ptr<A> shrd_ptr;
};

struct ClassB;
struct ClassA {
    std::shared_ptr<ClassB> b_ptr;
    ~ClassA() { std::cout << "~A()" << std::endl; }
};

struct ClassB{
    std::shared_ptr<ClassA> a_ptr;
    ~ClassB() { std::cout << "~B()" << std::endl; }
};

void UseAB() {
    auto a = make_shared<ClassA>();
    auto b = make_shared<ClassB>();
    a->b_ptr = b;
    b->a_ptr = a;
}

int main(int argc, char* argv[]) {
    // cyclic reference eg 1
    shared_ptr<A> ptr_a(make_shared<A>());
    shared_ptr<A> ptr_b(make_shared<A>());
    ptr_a->shrd_ptr = ptr_b;
    ptr_b->shrd_ptr = ptr_a;

    // cyclic reference eg 2
    UseAB();
    return 0;
}