#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdarg.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>

#define SERV_PORT 12345
#define INIT_SIZE 128
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

int tcp_server_listen(int port) {
    int listenfd;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int on = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    int ret = bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (ret < 0) {
        error(1, errno, "bind, failed");
    }

    ret = listen(listenfd, 1024);
    if (ret < 0) {
        error(1, errno, "listen failed");
    }

    signal(SIGPIPE, SIG_IGN);

    return listenfd;
}

int main(int argc, char* argv[]) {
    int listenfd, connfd;
    int ready_number;
    ssize_t n;
    char buf[MAXLINE];

    struct sockaddr_in client_addr;
    listenfd = tcp_server_listen(SERV_PORT);

    struct pollfd event_set[INIT_SIZE];
    event_set[0].fd = listenfd;
    event_set[0].events = POLLRDNORM;

    int i;
    for (i = 1; i < INIT_SIZE; i++) {
        event_set[i].fd = -1;
    }

    for (;;) {
        if ((ready_number = poll(event_set, INIT_SIZE, -1)) < 0) {
            error(1, errno, "poll failed");
        }

        if (event_set[0].revents & POLLRDNORM) {
            socklen_t client_len = sizeof(client_addr);
            connfd = accept(listenfd, (struct sockaddr*)&client_addr, &client_len);

            for (i = 1; i < INIT_SIZE; i++) {
                if (event_set[i].fd < 0) {
                    event_set[i].fd = connfd;
                    event_set[i].events = POLLRDNORM;
                    break;
                }
            }

            if (i == INIT_SIZE) {
                error(1, errno, "can not hold so many clients");
            }

            if (--ready_number <= 0) {
                continue;
            }
        }

        for (i = 1; i < INIT_SIZE; i++) {
            int socket_fd;
            if ((socket_fd = event_set[i].fd) < 0)
                continue;
            if (event_set[i].revents & (POLLRDNORM | POLLERR)) {
                if ((n = read(socket_fd, buf, MAXLINE)) > 0) {
                    if (write(socket_fd, buf, n) < 0) {
                        error(1, errno, "write error");
                    }
                } else if (n == 0 || errno == ECONNRESET) {
                    close(socket_fd);
                    event_set[i].fd = -1;
                } else {
                    error(1, errno, "read error");
                }

                if (--ready_number <= 0)
                    break;
            }
        }
    }

    return 0;
}