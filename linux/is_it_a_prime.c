#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prime.h"
/*
 * gcc -L${PWD} is_it_a_prime.c -o is_it_a_prime -lprime
 * export LD_LIBRARY_PATH=${PWD}:{LD_LIBRARY_PATH}
 * ╰─$ ldd is_it_a_prime
        linux-vdso.so.1 (0x00007fffd6bfc000)
        libprime.so => /home/ub/remote/linux_system/ch3/libprime.so (0x00007fd9f7081000)
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007fd9f6e53000)
        /lib64/ld-linux-x86-64.so.2 (0x00007fd9f708d000)

 */
int main(int argc, char* argv[]) {
    long int num;
    if (argc != 2) {
        fprintf(stderr, "Usage: %s number\n", argv[0]);
        return 1;
    }

    if (strspn(argv[1], "0123456789") != strlen(argv[1])) {
        return 1;
    }

    num = atol(argv[1]);
    if (isprime(num)) {
        printf("%ld is a prime\n", num);
    } else {
        printf("%ld is not a prime\n", num);
    }

    return 0;
}