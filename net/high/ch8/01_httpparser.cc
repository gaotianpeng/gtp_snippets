#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <libgen.h>

#define BUFFER_SIZE 4096

enum CHECK_STATE {
    CHECK_STATE_REQUESTLINE = 0, // 正在分析请求行
    CHECK_STATE_HEADER, // 正在分析头部信息
    CHECK_STATE_CONTENT
};

enum LINE_STATUS {
    LINE_OK = 0, // 读取到完整一行
    LINE_BAD,    // 行出错
    LINE_OPEN    // 行数据尚且不完整
};

enum HTTP_CODE {
    NO_REQUEST, // HTTP请求不完整，需要继续读取客户数据
    GET_REQUEST, // 获得了一个完整的用户请求
    BAD_REQUEST, // 客户请求有语法错误
    FORBIDDEN_REQUEST, // 客户对资源没有足够的访问权限
    INTERNAL_ERROR, // 服务器内部错误
    CLOSED_CONNECTION // 客户端已经关闭连接了
};

static const char* szret[] = {
    "I get a correct result\n",
    "Something wrong\n"
};

// 从状态机，用于解析出一行内容
LINE_STATUS parse_line(char* buffer, int& checked_index, int& read_index) {
    char temp;
    /*
        checked_index 指向buffer(应用程序的读缓冲区)中当前正在分析的字节
        read_index 指向buffer中客户数据的尾部的下一字节
        buffer中第 0 ~ checked_index 字节已经分析完毕
        buffer中第 checked_index ~(read_index - 1) 字节由下面的循环挨个分析
    */
    for (; checked_index < read_index; ++checked_index) {
        temp = buffer[checked_index]; // 获取当前要分析的字节
        if (temp == '\r') { // 如果当前的字节是'\r'，即回车符，则说明可能读取到一个完整的行
            /*
                如果'\r'字符碰巧是目前buffer中最后一个已经被读入的客户数据，那么这次分析没有
                读取到一个完整的行，返回LINE_OPEN 以表示还需要继续读取客户数据才能进一步分析
            */
            if ( (checked_index + 1) == read_index) {
                return LINE_OPEN;
            } else if (buffer[checked_index + 1] == '\n') { // 如果下一个字符是'\n', 则说明我们成功读取到一个守整的行
                buffer[checked_index++] = '\0';
                buffer[checked_index++] = '\0';
                return LINE_OK;
            }
            // 否则的话，说明客户发送的HTTP请求存在语法问题
            return LINE_BAD;
        } else if (temp == '\n ') { // 如果当前字符是‘\m'，即换行符，则说明可能读取到一个完整的行
            if ((checked_index > 1) && buffer[checked_index - 1] == '\r') {
                buffer[checked_index - 1] = '\0';
                buffer[checked_index++] = '\0';
                return LINE_OK;
            }
            return LINE_BAD;
        }
    }
    // 如果所有内容分析完也没遇到'\r'字符，则返回LINE_OPEN, 表示还需要继续读取客户数据才能进一步分析
    return LINE_OPEN;
}

// 分析请求行
HTTP_CODE parse_requestline(char* temp, CHECK_STATE& checkstate) {
    char* url = strpbrk(temp, " \t");
    // 如果请求行中没有空白字符或"\t" 字符，则HTTP请求必有问题
    if (!url) {
        return BAD_REQUEST;
    }
    *url++ = '\0';

    char* method = temp;
    if (strcasecmp(method, "GET") == 0) { // 仅支持GET方法
        printf("The request method is GET\n");
    } else {
        return BAD_REQUEST;
    }

    url += strspn(url, " \t");
    char* version = strpbrk(url, " \t");
    if (!version) {
        return BAD_REQUEST;
    }
    *version++ = '\0';
    version += strspn(version, " \t");
    // 仅支持HTTP/1.1
    if (strcasecmp(version, "HTTP/1.1") != 0) {
        return BAD_REQUEST;
    }

    // 检查UTL是否合法
    if (strncasecmp(url, "http://", 7) == 0) {
        url += 7;
        url = strchr(url, '/');
    }
    if (!url || url[0] != '/') {
        return BAD_REQUEST;
    }

    printf("The request URL is: %s\n", url);
    // HTTP 请求行处理完毕，状态转移到头部字段的分析
    checkstate = CHECK_STATE_HEADER;
    return NO_REQUEST;
}

// 分析头部字段
HTTP_CODE parse_headers(char* temp) {
    // 遇到一个空行，说明我们得到了一个正确的HTTP请求
    if (temp[0] == '\0') {
        return GET_REQUEST;
    } else if (strncasecmp(temp, "Host:", 5) == 0) { // 处理HOST头部字段
        temp += 5;
        temp += strspn(temp, " \t");
        printf("the request host is: %s\n", temp);
    } else { // 其他头部字段都不处理
        printf("I can not handle this header\n");
    }

    return NO_REQUEST;
}

// 分析HTTP请求的入口函数


int main(int argc, char* argv[]) {
    if (argc <= 2) {
        printf("usage: %s ip_address port_number\n", basename(argv[0]));
        return 1;
    }

    const char* ip = argv[1];
    int port = atoi(argv[2]);
    
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(listenfd >= 0);

    int ret = bind(listenfd, (struct sockaddr*)&address, sizeof(address));
    assert(ret != -1);

    ret = listen(listenfd, 5);
    assert(ret != 1);

    struct sockaddr_in client_address;
    socklen_t client_addrlength = sizeof(client_address);
    int fd = accept(listenfd, (struct sockaddr*)&client_address, 
            &client_addrlength);
    
    if (fd < 0) {
        printf("errno is : %d\n", errno);
    } else {
        char buffer[BUFFER_SIZE];
        memset(buffer, '\0', BUFFER_SIZE);
        int data_read = 0;
        int read_index = 0;
        int checked_index = 0;
        int start_line = 0;
        // CHECK_STATE checkstate = CHECK_STATE_REQUESTLINE;

    }


    return 0;
}