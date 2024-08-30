#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#include "kernel/stat.h"
#include "kernel/fs.h"


// 在使用 fmtname 之前声明它 否则会译器会隐式地假设函数的返回类型为 int
char* fmtname(char *path);

// 递归查找函数
void find(char *path, char *target) {
    char buf[512], *p;   //文件路径
    int fd;             //文件描述符
    struct dirent de;   //目录结构
    struct stat st;     //文件信息结构

    //打开文件路径  打不开就报错
    if ((fd = open(path, O_RDONLY)) < 0) {
        printf("find: cannot open %s\n", path);
        return;
    }

    //打得开就把path的所有信息都装载到st内
    if (fstat(fd, &st) < 0) {
        printf("find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type) {
    case T_FILE:   //是文件
        if (strcmp(fmtname(path), target) == 0) {
            printf("%s\n", path);
        }
        break;
    
    //若是目录，则在目录中递归查找文件
    case T_DIR:        //还是文件夹
        // 检查路径长度是否超过buf的限制
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
            printf("find: path too long\n");
            break;
        }
        strcpy(buf, path);  //将path复制到buf中
        p = buf + strlen(buf);   //将p指向buf的末尾
        *p++ = '/';
        //以上内容都是和找文件名相关的

        //扫描目录中的每一个文件
        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            if (de.inum == 0)
                continue;
                //跳过 . 和 .. 
            if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                continue;

            memmove(p, de.name, strlen(de.name));   //拼接路径
            p[strlen(de.name)] = 0;                 //添加字符串结束符

            if (stat(buf, &st) < 0) {
                printf("find: cannot stat %s\n", buf);
                continue;
            }
            find(buf, target);   //递归深入目录树,进入下一级目录
        }
        break;
    }
    close(fd);
}


//格式化路径，提取文件名部分
//提取路径中最后一个斜杠后的文件名部分。
char* fmtname(char *path) {
    static char buf[DIRSIZ+1];
    char *p;

    // 找到路径中最后一个斜杠的位置
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // 返回文件名
    if (strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    buf[strlen(p)] = 0;
    return buf;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: find <path> <filename>\n");
        exit(0);
    }
    find(argv[1], argv[2]);
    exit(0);
}
