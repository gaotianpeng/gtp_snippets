#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 4096
#define MAXLINE     4096

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
        error(1, 0, "usage: unixstreamclient <localpath>");
    }

    int sockfd;
    struct sockaddr_un serv_addr;
    
    sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error(1, errno, "create socket failed");
    }

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sun_family = AF_LOCAL;
    strcpy(serv_addr.sun_path, argv[1]);

    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        error(1, errno, "connected failed");
    }

    char send_line[MAXLINE];
    bzero(send_line, MAXLINE);
    char recv_line[MAXLINE];

    while (fgets(send_line, MAXLINE, stdin) != NULL) {
        int nbytes = sizeof(send_line);
        if (write(sockfd, send_line, nbytes) != nbytes) {
            error(1, errno, "write error");
        }

        if (read(sockfd, recv_line, MAXLINE) == 0) {
            error(1, errno, "server terminated prematurely");
        }

        fputs(recv_line, stdout);
    }

    return 0;
}

