#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
    cout << __cplusplus << endl;
    // windows check memory leak
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    int* p = new int;

    std::cout << "Hello World!\n";
    return 0;
}
