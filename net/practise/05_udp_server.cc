// c std
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

// c std linux
#include <unistd.h>

// linux
#include <sys/socket.h>
#include <netinet/in.h>

static int count;

#define SERV_PORT 12345
#define MAXLINE 1024

static void recvfrom_int(int signo) {
    printf("\n received %d datagrams\n", count);
    exit(0);
}

int main(int argc, char* argv[]) {
    int sockfd;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SERV_PORT);

    bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    socklen_t client_len;
    char message[MAXLINE];
    count = 0;
    
    signal(SIGINT, recvfrom_int);

    struct sockaddr_in client_addr;
    client_len = sizeof(client_addr);
    for (;;) {
        int n = recvfrom(sockfd, message, MAXLINE, 0, (struct sockaddr*)&client_addr,
                &client_len);
        message[n] = 0;

        printf("received %d bytes: %s\n", n, message);
        char send_line[MAXLINE];
        sprintf(send_line, "Hi, %s", message);
        sendto(sockfd, send_line, strlen(send_line), 0, (struct sockaddr*)&client_addr,
                client_len);
        ++count;
    }

    return 0;
}