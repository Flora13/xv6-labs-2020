#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"


int main(int argc, char* argv[]){
    //将argv的参数保存在args中
    char* args[MAXARG];  //题目要求用这个
    int i;
    for(i = 0; i < argc; i++){
        args[i] = argv[i];   //先把xargs及其本身的参数保存起来
    }

    char buf[256];

    while(1){
        //从标准输入读入一行
        int j = 0;
        while((0!=read(0, buf+j, sizeof(char))) && (buf[j]!= '\n')){
            j++;
        }

        if(j == 0) break;  //如果读入的是一个空行 则退出循环
        buf[j] = 0;//添加字符串结束符
        //将标准输入传进的一行参数附加到args中
        args[i] = buf;   //i==argc 
        //执行args中的命令
        args[i+1] = 0;  //args的最后一个参数是0 表示结束
        //执行命令
        if(fork() == 0){
            //args[0]是xargs,args[1]是要执行的命令，剩余的是参数
            exec(args[1], args+1);
            exit(0);
        }else{
            wait(0);  //子进程执行命令，父进程等待
        }
    }
    exit(0);
}