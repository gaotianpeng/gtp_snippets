#include <stdio.h>
#include <stddef.h>

typedef struct {
    int a;
    long double b;
} TestStruct1;

typedef struct {
    char a;
    double b;
    char c;
} TestStruct2;

int main(int argc, char* argv[]) {
    printf("\nsizeof(TestStruct1): %d\nsizeof(char): %d\n"
           "sizeof(long double): %d\n\n",
           sizeof(TestStruct1), sizeof(char), sizeof(long double));
    printf("offsetof(TestStruct1, a): %d\n"
           "offsetof(TestStruct1, b): %d\n\n",
           offsetof(TestStruct1, a), offsetof(TestStruct1, b));

    printf("\nsizeof(TestStruct2): %d\nsizeof(char): %d\n"
           "sizeof(double): %d\n\n",
           sizeof(TestStruct2), sizeof(char), sizeof(double));
    printf("offsetof(TestStruct2, a): %d\n"
           "offsetof(TestStruct2, b): %d\n"
           "offsetof(TestStruct2, c): %d\n\n",
           offsetof(TestStruct2, a), offsetof(TestStruct2, b), offsetof(TestStruct2, c));

    TestStruct2 data;
    printf("TestStruct2->a: %u\nTestStruct2->b: %u\n"
           "TestStruct2->c: %u\n\n",
           &data.a, &data.b, &data.c);
    return 0;
}







