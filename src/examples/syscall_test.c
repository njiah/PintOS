//testing of system calls

#include<syscall.h>
#include<stdio.h>

int main(void)
{
    char buf[128];
    char *str = "Operating System Resit by Group 7";
    
    create("file.txt", 300);
    remove("file.txt");
    //int fd = open("file.txt");
    //write(1, "Operating System Resit by Group 7", 128);
    //read(1, buf, 128);
    //close(fd);
    
    halt();

    return 1;
}