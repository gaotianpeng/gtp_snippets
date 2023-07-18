#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <syslog.h>

typedef struct {
    u_int32_t type;
    char data[1024];
} messageObject;

#define MSG_PING 1
#define MSG_PONG 2
#define MSG_TYPE1 11
#define MSG_TYPE2 21

#define    SERV_PORT      43211
#define    LISTENQ        1024

static int count;

static void sig_int(int signo) {
    printf("\n received %d datagrams\n", count);
    exit(0);
}

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
/*
    gcc 02_pingserver.cc -o server
    ./server 10
*/
int main(int argc, char* argv[]) {
    if (argc != 2) {
        error(1, 0, "usage: tcpserver <sleepingtime>");
    }

    int sleep_time = atoi(argv[1]);

    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERV_PORT);

    int ret = bind(listenfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        error(1, errno, "bind failed ");
    }

    ret = listen(listenfd, LISTENQ);
    if (ret < 0) {
        error(1, errno, "listen failed");
    }

    signal(SIGINT, sig_int);
    signal(SIGPIPE, SIG_IGN);

    int connfd;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    
    if ((connfd = accept(listenfd, (struct sockaddr*)&client_addr, &client_len)) < 0) {
        error(1, errno, "accepe failed");
    }

    messageObject message;
    count = 0;

    for (;;) {
        int n = read(connfd, (char*)&message, sizeof(messageObject));
        if (n < 0) {
            error(1, errno, "error read");
        } else if (n == 0) {
            error(1, 0, "client closed \n");
        }

        printf("receviced %d bytes\n", n);
        ++count;

        switch (ntohl(message.type)) {
            case MSG_TYPE1:
                printf("process MSG_TYPE1 \n");
                break;
            case MSG_TYPE2:
                printf("process MSG_TYPE2\n");
                break;
            case MSG_PING: {
                messageObject pong_message;
                pong_message.type = MSG_PONG;
                sleep(sleep_time);
                ssize_t rc = send(connfd, (char*)&pong_message, sizeof(pong_message), 0);
                if (rc < 0) {
                    error(1, errno, "send failure");
                }
                break;
            }

            default:
                error(1, 0, "unknow message type (%d)\n", ntohl(message.type));
        } 
    }

    return 0;
}