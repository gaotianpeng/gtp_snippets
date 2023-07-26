#include "prime.h"
/*
 * 位置无关的代码(PIC)
 * 1) 编译生成目标文件
 *      gcc -Wall -Wextra -pedantic -fPIC -c prime.c
 * 2）将目标文件打包成一个共享库
 * gcc -shared -Wl,-soname,libprime.so -o libprime.so prime.o

 */
int isprime(long int number) {
    long int j;
    int prime = 1;
    for (j = 2; j < number; ++j) {
        if (number % j  == 0) {
            prime = 0;
        }
    }
    if (prime == 1) {
        return 1;
    } else {
        return 0;
    }
}
