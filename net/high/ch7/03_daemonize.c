#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <fcntl.h>

bool daemonize() {
    // 创建子进程，关闭父进程，这样可以使程序在后台运行
    pid_t pid = fork();
    if (pid < 0) {
        return false;
    } else if (pid > 0) {
        exit(0);
    }

    umask(0);
    pid_t sid = setsid();
    if (sid < 0) {
        return false;
    }
    if ((chdir("/")) < 0) {
        return false;
    }
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    open("/dev/null", O_RDONLY);
    open("/dev/null", O_RDWR);
    open("/dev/null", O_RDWR);

    return true;
}

int main(int argc, char* argv[]) {
    daemonize();
    while (true) {
        sleep(1);
        printf("___\n");;
    }
    return 0;
}