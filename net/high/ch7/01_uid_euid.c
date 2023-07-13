#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
/*
    gcc 01_uid_euid.c -o test_uid  
    sudo chown root:root test_uid   # 修改目标文件的所有者为 root 
    sudo chmod +s test_uid          # 设置目标文件的 set-user-id 标志
    ./test_uid
    userid is 1001, effective userid is : 0
*/

int main(int argc, char* argv[]) {
    uid_t uid = getuid();
    uid_t euid = geteuid();

    printf("userid is %d, effective userid is : %d\n", uid, euid);

    return 0;
}