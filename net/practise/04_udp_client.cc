// c std
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

// c std linux
#include <unistd.h>

// linux
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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

int main(int argc, char* argv[]) {
    if (argc != 2) {
        error(1, errno, "usage: udpclient <IPAddress>");
    }

    int sockfd;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &serv_addr.sin_addr.s_addr);

    socklen_t serv_len = sizeof(serv_addr);
    struct sockaddr* reply_addr;
    reply_addr = (sockaddr*)malloc(serv_len);

    char send_line[MAXLINE], recv_line[MAXLINE + 1];
    socklen_t len;
    int n;
    while (fgets(send_line, MAXLINE, stdin) != NULL) {
        int i = strlen(send_line);
        if (send_line[i - 1] == '\n') {
            send_line[i - 1] = 0;
        }

        printf("now sending %s\n", send_line);
        size_t rt = sendto(sockfd, send_line, strlen(send_line), 0,
            (struct sockaddr*)&serv_addr, serv_len);
        if (rt < 0) {
            error(1, errno, "send faield");
        }

        printf("send bytes: %zu\n", rt);
        
        len = 0;
        n = recvfrom(sockfd, recv_line, MAXLINE, 9, reply_addr, &len);
        if (n < 0) {
            error(1, errno, "recvfrom failed");
        }

        recv_line[n] = 0;
        fputs(recv_line, stdout);
        fputs("\n", stdout);
    }

    return 0;
}