#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>


size_t readn(int fd, void* buffer, size_t size) {
    char* buffer_pointer = (char*)buffer;
    int length = size;

    while (length > 0) {
        int result = read(fd, buffer_pointer, length);

        if (result < 0) {
            if (errno == EINTR) {
                continue;
            } else {
                return -1;
            }
        } else if (result == 0) { // 表示套接字关闭
            break;
        }

        length -= result;
        printf("read %d bytes, left %d\n", result, length);   
        buffer_pointer += result;
    }

    return size - length;
}

void read_data(int sockfd) {
    ssize_t n;
    char buf[1024];

    int time = 0;
    for (;;) {
        fprintf(stdout, "block in read\n");
        if ((n = readn(sockfd, buf, 1024)) == 0) {
            return;
        }

        time++;
        fprintf(stdout, "1K read for %d \n", time);
        usleep(20000);
    }

}

int main(int argc, char* argv[]) {
    int listenfd, connfd;
    socklen_t client_len;
    sockaddr_in client_addr, serv_addr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(12345);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(listenfd, 1024);

    for (;;) {
        client_len = sizeof(client_addr);
        connfd = accept(listenfd, (struct sockaddr*)&client_addr, &client_len);
        read_data(connfd);

        close(connfd);
    }


    return 0;
}