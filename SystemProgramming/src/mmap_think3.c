#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
/*
open为O_RDONLY
mmap为PROT_WRITE|PROT_READ
invalid arguments
*/

void sys_err(const char* str){
    perror(str);
    exit(1);
}

int main(int argc, char** argv)
{
    char* p = NULL;
    int fd;

    fd = open("../data/testmap", O_RDONLY|O_CREAT|O_TRUNC, 0644);
    if(fd == -1){
        sys_err("open error");
    }
    //也可以使用truncate来拓展文件大小
    ftruncate(fd, 10);
    //获取文件长度
    int len = lseek(fd, 0, SEEK_END);

    // mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset);
    p = mmap(NULL, len, PROT_WRITE|PROT_READ, MAP_SHARED, fd, 0);
    if(p == MAP_FAILED){
        sys_err("mmap error");
    }
    //使用p对文件进行读写操作
    strcpy(p, "hello mmap");  //写操作
    printf("----%s\n", p);

    int ret = munmap(p, len);
    if(ret == -1){
        sys_err("munmap error");
    }
    close(fd);
    return 0;
}