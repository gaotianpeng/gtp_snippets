#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char* message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char* argv[]) {
    int sock;
    char msg1[] = "Hi!";
    char msg2[] = "I'm another UDP host";
    char msg3[] = "Nice to meet you";

    struct sockaddr_in your_addr;
    socklen_t your_addr_sz;
    if (argc != 3) {
        printf("Usage: %s <IP> <port> \n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        error_handling("socket() error");
    }

    memset(&your_addr, 0, sizeof(your_addr));
    your_addr.sin_family = AF_INET;
    your_addr.sin_addr.s_addr = inet_addr(argv[1]);
    your_addr.sin_port = htons(atoi(argv[2]));

    sendto(sock, msg1, sizeof(msg1), 0, (struct sockaddr*)&your_addr, sizeof(your_addr));
    sendto(sock, msg2, sizeof(msg2), 0, (struct sockaddr*)&your_addr, sizeof(your_addr));
    sendto(sock, msg3, sizeof(msg3), 0, (struct sockaddr*)&your_addr, sizeof(your_addr));
    close(sock);

    return 0;
}