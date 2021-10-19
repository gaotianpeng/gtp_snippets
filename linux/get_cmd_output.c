#include <stdio.h>
#include <string.h>

#define CMD_RESULT_BUF_SIZE 1024

/*
 * cmd：    待执行命令
 * result:  命令输出结果
 * 函数返回:  0 成功, -1 失败
 */
int ExecCmd(const char* cmd, char* result) {
    int ret = -1;
    char buf_ps[CMD_RESULT_BUF_SIZE] = {0};
    char ps[CMD_RESULT_BUF_SIZE] = {0};
    FILE* ptr;

    strcpy(ps, cmd);
    if ((ptr = popen(ps, "r")) != NULL) {
        while (fgets(buf_ps, sizeof(buf_ps), ptr) != NULL) {
            strcat(result, buf_ps);
            if (strlen(result) > CMD_RESULT_BUF_SIZE) {
                break;
            }
        }
        pclose(ptr);
        ptr = NULL;
        ret = 0;
    } else {
        printf("popen %s error\n", ps);
        ret = -1;
    }

    return ret;
}

int main(int argc, char* argv[]) {
    char result[CMD_RESULT_BUF_SIZE] = {0};
    ExecCmd("ls -l", result);

    printf("This is an example\n\n");
    printf("%s", result);
    printf("\n\nThis is end");
    return 0;
}