#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void printhelp(char progname[]) {
    printf("%s [-s] [-m] integer ...\n",progname);
    printf("-s sums all the integers]n"
           "-m multiplies all the integers]n"
           "This program takes any number of integer"
           "values and either add or multiply them.\n"
           "For example: %s -m 5 5 5\n", progname);
}

/*
    ┌─[gtp@VM-24-8-centos] - [~/work/Linux-System-Programming-Techniques/ch1] - [2175]
    └─[$] ./new-sum -m 4 3 2                                                                                                          [11:13:46]
    Total: 24
    ┌─[gtp@VM-24-8-centos] - [~/work/Linux-System-Programming-Techniques/ch1] - [2176]
    └─[$] ./new-sum -s 4 3 2                                                                                                          [11:13:54]
    Total: 9
 */
int main(int argc, char* argv[]) {
    if (argc == 1) {
        printhelp(argv[0]);
        return 1;
    }

    int i = 0, opt = 0, sum = 0;
    while ((opt = getopt(argc, argv, "smh")) != -1) {
        switch (opt) {
            case 's': /* sum the integers*/
                sum = 0;
                for (i = 2; i < argc; ++i) {
                    sum = sum + atoi(argv[i]);
                }
                break;
            case 'm': /* multiply the integers */
                sum = 1;
                for (i = 2; i < argc; i++) {
                    sum = sum * atoi(argv[i]);
                }
                break;
            case 'h': /* -h for help*/
                printhelp(argv[0]);
                return 0;
            default: /* in case of invalid options */
                printhelp(argv[0]);
                return 1;
        }
    }

    printf("Total: %i\n", sum);

    return 0;
}