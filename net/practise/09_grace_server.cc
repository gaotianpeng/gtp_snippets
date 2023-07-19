#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <errno.h>
#include <signal.h>

#include <sys/socket.h>
#include <arpa/inet.h>

static int count;

#define SERV_PORT 12345
#define MAXLINE 1024

void error(int status, int err, const char* fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    if (err) {
        fprintf(stderr, "%s, %d", strerror(err), err);
    }

    if (status) {
        exit(status);
    }
}

static void sig_int(int signo) {
    printf("\nreceived %d datagrams\n", count);
    exit(0);
}

int main(int argc, char* argv[]) {
    int listenfd;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SERV_PORT);

    int ret = bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (ret < 0) {
        error(1, errno, "bind failed");   
    }

    ret = listen(listenfd, 1024);
    if (ret < 0) {
        error(1, errno, "listen failed");
    }

    signal(SIGINT, sig_int);
    signal(SIGPIPE, SIG_IGN);

    int connfd;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    if ((connfd = accept(listenfd, (struct sockaddr*)&client_addr, &client_len)) < 0) {
        error(1, errno, "bind failed");
    }

    char message[MAXLINE];
    count = 0;
    for (;;) {
        bzero(message, sizeof(message));
        int n = read(connfd, message, MAXLINE);
        if (n < 0) {
            error(1, errno, "error read");
        } else if (n == 0) {
            error(1, 0, "client closed \n");
        }

        message[n] = 0;
        printf("received %d bytes: %s\n", n, message);
        count++;
        
        char send_line[MAXLINE];
        bzero(send_line, sizeof(send_line));
        sprintf(send_line, "Hi, %s", message);
        sleep(5);

        int write_nc = send(connfd, send_line, strlen(send_line), 0);
        printf("send bytes: %zu \n", write_nc);
        if (write_nc < 0) {
            error(1, errno, "error write");
        }
    }

    return 0;
}