#include <iostream>
#include <typeinfo>
#include <string>

using namespace std;

#define DEF_CLASS(T, U, RET, ClassName, SNEAK) \
template<typename T, typename U, typename Ret, T ptr> \
struct ClassName { \
    friend Ret & SNEAK(U& u) { \
        cout << typeid(ptr).name() << endl; \
        return u.*ptr; \
    } \
};

#define DEFINE_TUNNEL(CLASS_NAME, MEMBER_NAME, RET_TYPE, Tunnel, sneak) \
template struct Tunnel<decltype(&CLASS_NAME::MEMBER_NAME), \
CLASS_NAME, RET_TYPE, &CLASS_NAME::MEMBER_NAME>; \
RET_TYPE& sneak(CLASS_NAME& u);

class ClassA {
public:
    int i = 0;
    bool j = true;
};
class ClassB {
private:
    int j = 1;
};

DEF_CLASS(T, U, R, TunnelInt, sneakInt)
DEFINE_TUNNEL(ClassA, i, int, TunnelInt, sneakInt)

DEF_CLASS(T, U, R, TunnelBool, sneakBool)
DEFINE_TUNNEL(ClassA, j, bool, TunnelBool, sneakBool)
    
int main(int arg, char* argv[]) {
    {
        ClassA a;
        cout << sneakInt(a) << endl;
        cout << sneakBool(a) << endl;
    }

    return 0;
}