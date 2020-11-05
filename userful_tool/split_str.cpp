#include <iostream>
#include <vector>
#include <string>
using namespace std;

void SplitStr(const std::string& str, std::vector<std::string>& outs, char pattern = '$') {
    auto end = str.cend();
    auto start = end;

    for (auto it = str.cbegin(); it != end; ++it) {
        if (*it != pattern) {
            if (start == end) {
                start = it;
            }
            continue;
        }
        if (start != end) {
            outs.emplace_back(start, it);
            start = end;
        }
    }

    if (start != end) {
        outs.emplace_back(start, end);
    }
}

void test_case() {
    string empty_str;
    vector<string> empty_strs;
    SplitStr(empty_str, empty_strs);
    cout << (empty_strs.size() == 0) << endl;

    string test_1 = "gtp$";
    vector<string> strs_1;
    SplitStr(test_1, strs_1);
    cout << (strs_1.size() == 1) << ":" << (strs_1.at(0) == "gtp")  << endl;

    string test_2 = "$gtp$";
    vector<string> strs_2;
    SplitStr(test_2, strs_2);
    cout << (strs_2.size() == 1) << ":" << (strs_2.at(0) == "gtp")  << endl;

    string test_3 = "$gtp$1234";
    vector<string> strs_3;
    SplitStr(test_3, strs_3);
    cout << (strs_3.size() == 2) << ":" << (strs_3.at(0) == "gtp")
            << ":" << (strs_3.at(1) == "1234") << endl;

    std::string str_4 = "4567$gtp$1234";
    std::vector<std::string> strs_4;
    SplitStr(str_4, strs_4);
    cout << (strs_4.size() == 3) << ":" << (strs_4.at(0) == "4567")
         << ":" << (strs_4.at(1) == "gtp")
         << ":" << (strs_4.at(2) == "1234") << endl;

    // and so on ...
}

int main(int argc, char* argv[]) {
    test_case();
    return 0;
}