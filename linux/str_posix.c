#define _XOPEN_SOURCE 700
#include <string.h>
#include <stdio.h>

/*
    gcc -Wall -Wextra -pedantic -std=c99 str_posix.c -o str_posix
        str_posix.c: In function ‘main’:
        str_posix.c:9:9: warning: implicit declaration of function ‘strdup’; did you mean ‘strcmp’? [-Wimplicit-function-declaration]
            9 |     b = strdup(a);
            |         ^~~~~~
            |         strcmp
    ./str_posix 
    [1]    82726 segmentation fault (core dumped)  ./str_posix

    gcc -Wall -Wextra -pedantic -std=c99 -D_XOPEN_SOURCE=700 str_posix.c -o str_posix
*/
int main(int argc, char* argv[]) {
    char a[] = "Hello";
    char* b;
    b = strdup(a);
    printf("b = %s\n", b);
    return 0;
}