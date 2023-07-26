#include <stdio.h>
#include <math.h>
/*
    gcc interest.c -o interest -lm
    ╰─$ ldd interest
            linux-vdso.so.1 (0x00007fff3813f000)
            libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007f4dd0081000)
            libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f4dcfe59000)
            /lib64/ld-linux-x86-64.so.2 (0x00007f4dd0175000)
 */
int main(int argc, char* argv[]) {
    int years = 15;
    int savings = 99000;
    float interest = 1.5;
    printf("The total savings after %d years"
           "is %.2f\n", years,
           savings * pow(1 + (interest/100), years));
    return 0;
}