#include "kernel/types.h"
#include "user/user.h"

void filter(int fd[2]) {
    int prime;
    int new_fd[2];

    close(fd[1]);  // 关闭写端
    if (read(fd[0], &prime, sizeof(prime)) == 0) {
        close(fd[0]);  // 关闭读端
        exit(0);  // 没有更多的数据
    }
    
    printf("prime %d\n", prime);
    
    pipe(new_fd);
    
    if (fork() == 0) {
        // 子进程递归处理
        close(fd[0]);
        filter(new_fd);
    } else {
        // 父进程负责筛选并将数据写入新管道
        int num;
        close(new_fd[0]);  // 关闭新管道的读端
        while (read(fd[0], &num, sizeof(num)) > 0) {
            if (num % prime != 0) {    //筛选
                write(new_fd[1], &num, sizeof(num));
            }
        }
        close(fd[0]);  // 关闭父进程的读端
        close(new_fd[1]);  // 关闭父进程的写端
        wait(0);  // 等待子进程完成
        exit(0);
    }
}

int main() {
    int fd[2];
    pipe(fd);

    if (fork() == 0) {
        // 子进程开始筛选素数
        filter(fd);
    } else {
        // 父进程写入初始数据
        close(fd[0]);  // 关闭读端
        for (int i = 2; i <= 35; i++) {
            write(fd[1], &i, sizeof(i));
        }
        close(fd[1]);  // 关闭写端
        wait(0);  // 等待所有子进程完成
        exit(0);
    }
    return 0;
}
