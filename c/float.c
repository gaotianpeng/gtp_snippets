#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
    float data;
    unsigned long buff;
    int i = 0;
    char s[35];

    data = (float)0.75;

    memcpy(&buff, &data, 4);
    for (i = 33; i >= 0; i--) {
        if (i == 1 || i == 10) {
            // 在符号部分、指数部分和尾数部分之间插入连字符
            s[i] = '-';
        } else {
            // 判断各比特是'0', 还是'1'
            if (buff % 2 == 1) {
                s[i] = '1';
            } else {
                s[i] = '0';
            }
            buff /= 2;
        }
    }

    s[34] = '\0';

    printf("%s\n", s);

    return 0;
}