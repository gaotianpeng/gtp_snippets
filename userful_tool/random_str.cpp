#include <iostream>
#include <algorithm>
#include <string>
using namespace std;

std::string GenRandomString( size_t len ) {
    auto rand_char = []() -> char {
        const char charset[] =
                "0123456789"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[rand() % max_index ];
    };

    std::string ret_str(len, 0);
    std::generate_n(ret_str.begin(), len, rand_char);
    return ret_str;
}

int main(int argc, char* argv[]) {
    // test case
    for (int i = 0; i < 100; ++i) {
        cout << GenRandomString(10) << endl;
    }

    return 0;
}