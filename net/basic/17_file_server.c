#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void error_handling(char* message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char* argv[]) {
    int serv_sock, clnt_sock;
    FILE* fp;
    char buf[BUF_SIZE];
    int read_cnt;

    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_sz;

    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1); 
    }

    fp = fopen("file_server.c", "rb");
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(serv_sock, 5);

    clnt_addr_sz = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_sz);
    
    while (1) {
        read_cnt = fread((void*)buf, 1, BUF_SIZE, fp);
        if (read_cnt < BUF_SIZE) {
            write(clnt_sock, buf, read_cnt);
            break;
        }

        write(clnt_sock, buf, BUF_SIZE);
    }
    
    shutdown(clnt_sock, SHUT_WR);
    read(clnt_sock, buf, BUF_SIZE);
    printf("Mesage from client: %s\n", buf);

    fclose(fp);
    close(clnt_sock);
    close(serv_sock);

    return 0;
}