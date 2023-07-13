#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include <pwd.h>

// 以root 身份启动的进程，切换为以一个普通用户身份运行
static bool switch_to_user(uid_t user_id, gid_t gp_id) {
    // 先确保目标用户不是 root
    if ((user_id == 0) && (gp_id == 0)) {
        return false;
    }

    // 确保当前用户是合法用户：root 或者目标用户
    gid_t gid = getgid();
    uid_t uid = getuid();
    if ( ((gid != 0) || (uid != 0)) && ((gid != gp_id) || (uid != user_id))) {
        return false;
    }

    // 如果不是root ，则已经是目标用户
    if (uid != 0) {
        return true;
    }

    // 切换到目标用户
    if ((setgid(gp_id) < 0) || (setuid(user_id) < 0)) {
        return false;
    }

    return true;
}

uid_t getUserIdFromUsername(const char* username) {
    struct passwd* pwd = getpwnam(username);
    if (pwd == NULL) {
        printf("Failed to retrieve user information for username: %s\n", username);
        exit(1);
    }

    return pwd->pw_uid;
}

gid_t getGroupIdFromUsername(const char* username) {
    struct passwd* pwd = getpwnam(username);
    if (pwd == NULL) {
        printf("Failed to retrieve user information for username: %s\n", username);
        exit(1);
    }

    return pwd->pw_gid;
}

int main(int argc, char* argv[]) {
    const char* username = "gtp";
    uid_t uid = getUserIdFromUsername(username);
    gid_t gid = getGroupIdFromUsername(username);
    printf("UID for user '%s' is: %d\n", username, uid);
    printf("GID for user '%s' is: %d\n", username, gid);

    switch_to_user(uid, gid);
    return 0;
}
