#include <stdio.h>

void byteorder() {
    union {
        short value;
        char union_types[sizeof(short)];
    } test;

    test.value = 0x0102;
    if ((test.union_types[0] == 1) && test.union_types[1] == 2) {
        printf("big endian\n");
    } else if ((test.union_types[0] == 2) && (test.union_types[1] == 1)) {
        printf("little endian\n");
    } else {
        printf("unknow...\n");   
    }
}

int main(int argc, char* argv[]) {
    byteorder();
    return 0;
}