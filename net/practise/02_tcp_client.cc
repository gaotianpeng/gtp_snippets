#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MESSAGE_SIZE 102400

void error(int status, int err, const char *fmt, ...) {
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

void send_data(int sockfd) {
    char* message = NULL;
    message = (char*)malloc(MESSAGE_SIZE + 1);
    for (int i = 0; i < MESSAGE_SIZE; ++i) {
        message[i] = 'a';
    }
    message[MESSAGE_SIZE] = '\0';

    const char* pos = message;
    ssize_t remaining = MESSAGE_SIZE;
    while (remaining > 0) {
        int n_written = send(sockfd, pos, remaining, 0);
        fprintf(stdout, "send into socket buffer %d\n", n_written);
        if (n_written <= 0) {
            error(1, errno, "send failed");
            return;
        }
        remaining -= n_written;
        pos += n_written;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("usage %s <ip> <port>\n", argv[0]);
        exit(0);
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);
    serv_addr.sin_port = htons(atoi(argv[2]));

    connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    send_data(sockfd);
    close(sockfd);
    return 0;
}