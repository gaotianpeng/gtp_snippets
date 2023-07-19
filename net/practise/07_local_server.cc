#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define MAXLINE     1024

void error(int status, int err, const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    if (err) {
        fprintf(stderr, ": %s (%d)\n", strerror(err), err);
    }

    if (status) {
        exit(status);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        error(1, 0, "usage unixstreamserver <local_path>");
    }

    int listenfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (listenfd < 0) {
        error(1, errno, "socket create failed!");
    }

    char* local_path = argv[1];
    unlink(local_path);

    struct sockaddr_un serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sun_family = AF_LOCAL;
    strcpy(serv_addr.sun_path, local_path);

    if (bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        error(1, errno, "bind failed");
    }

    if (listen(listenfd, 1024) < 0) {
        error(1, errno, "listen failed");
    }

    sockaddr_un client_addr;
    socklen_t client_len = sizeof(client_addr);
    int connfd;
    if ((connfd = accept(listenfd, (struct sockaddr*)&client_addr, &client_len)) < 0) {
        if (errno == EINTR) {
            error(1, errno, "accept failed");
        } else {
            error(1, errno, "accept failed");
        }
    }

    char buf[BUFFER_SIZE];
    while (1) {
        bzero(buf, sizeof(buf));
        if (read(connfd, buf, BUFFER_SIZE) == 0) {
            printf("client quit");
            break;
        }
        printf("Receive: %s", buf);

        char send_line[MAXLINE];
        int n_bytes = sizeof(send_line);
        bzero(send_line, n_bytes);
        sprintf(send_line, "hi, %s", buf);

        if (write(connfd, send_line, n_bytes) != n_bytes) {
            error(1, errno, "write error");
        }

    }

    close(listenfd);
    close(connfd);

    return 0;
}