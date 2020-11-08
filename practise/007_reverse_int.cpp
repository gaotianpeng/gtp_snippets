#include <iostream>
#include <climits>
using namespace std;
/*
给出一个 32 位的有符号整数，你需要将这个整数中每位上的数字进行反转
*/
class Solution{
public:
    int reverse(int x) {
        int ret = 0;
        while (x != 0) {
            int pop = x % 10;
            x /= 10;
            if (ret > INT_MAX/10 || (ret == INT_MAX/10 && pop > 7)) {
                return 0;
            }
            if (ret < INT_MIN/10 || (ret == INT_MAX/10 && pop < -8)) {
                return 0;
            }
            ret = ret*10 + pop;
        }
        return ret;
    }
};

void test_case() {
    Solution sol;
    cout << sol.reverse(100) << endl;
}

int main(int argc, char* argv[]) {
    test_case();
    return 0;
}