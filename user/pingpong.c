#include "kernel/types.h"
#include "user/user.h"

#define MSGSIZE 16

int main(){
    int fd[2];
    char buf[MSGSIZE];

    pipe(fd);  //fd[0]读  fd[1]写  

    int pid = fork();

    if(pid==0){
        //子进程
        int ret = read(fd[0],buf,MSGSIZE);
        if(ret<0){
            exit(1);
        }
        printf("%d: received ping\n",getpid());
        write(fd[1],"pong",MSGSIZE);
        exit(0);        
    }else if(pid>0){
        //父进程
        write(fd[1],"ping",MSGSIZE);
        wait(0);
        int ret = read(fd[0],buf,MSGSIZE);
        if(ret<0){
            exit(1);
        }
        printf("%d: received pong\n",getpid());
    }else{
        exit(1);
    }
    exit(0);  //成功退出
}