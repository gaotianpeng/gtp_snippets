/*
    读入一个行字符串（可能包含空格），输出这个字符串字符的数量
*/
#include <stdio.h>

int main(int argc, char* argv[]) {
    char str[100] = {0};
    int ret = 0;
    while (scanf("%[^\n]s", str) != -1) {
        getchar();
        printf(" has %d chars!\n", printf("%s", str));;
    }
    return 0;
}