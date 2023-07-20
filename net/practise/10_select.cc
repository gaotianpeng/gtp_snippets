#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define SERV_PORT 12345
#define MAXLINE 1024

void error(int status, int err, const char* fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    fprintf(stderr, fmt, ap);
    va_end(ap);

    if (err) {
        fprintf(stderr, "%s, %d", strerror(err), err);
    }

    if (status) {
        exit(status);
    }
}

int tcp_client(char* address, int port) {
    int sockfd;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    inet_pton(AF_INET, address, &serv_addr.sin_addr);

    socklen_t serv_len = sizeof(serv_addr);
    int ret = connect(sockfd, (struct sockaddr*)&serv_addr, serv_len);
    if (ret < 0) {
        error(1, errno, "connect failed");
    }

    return sockfd;
} 

int main(int argc, char* argv[]) {
    if (argc != 2) {
        error(1, 0, "usage: select01 <IPAddress>");
    }

    int sockfd = tcp_client(argv[1], SERV_PORT);

    char recv_line[MAXLINE], send_line[MAXLINE];
    int n;

    fd_set readmask;
    fd_set allreads;

    FD_ZERO(&allreads);
    FD_SET(0, &allreads);
    FD_SET(sockfd, &allreads);

    for (;;) {
        readmask = allreads;
        int rc = select(sockfd + 1, &readmask, NULL, NULL, NULL);
        if (rc <= 0) {
            error(1, errno, "select failed");
        }

        if (FD_ISSET(sockfd, &readmask)) {
            n = read(sockfd, recv_line, MAXLINE);
            if (n < 0) {
                error(1, errno, "read error");
            } else if (n == 0) {
                error(1, 0, "server terminated \n");
            }
            
            recv_line[n] = 0;
            fputs(recv_line, stdout);
            fputs("\n", stdout);
        }

        if (FD_ISSET(STDIN_FILENO, &readmask)) {
            if (fgets(send_line, MAXLINE, stdin) != NULL) {
                int i = strlen(send_line);
                if (send_line[i - 1] == '\n') {
                    send_line[i - 1] = 0;
                }

                printf("now sending %s\n", send_line);
                ssize_t rt = write(sockfd, send_line, strlen(send_line));
                if (rt < 0) {
                    error(1, errno, "write failed");
                }

                printf("send bytes: %zu \n", rt);
            }
        }
    }

    return 0;
}