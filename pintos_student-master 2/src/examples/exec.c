//testing exec system call

#include<syscall.h>

int main(void)
{
    char buf[64];
    //create("file.txt", 64);
    //int fd = open("file.txt");
    write(1, "Hello World", 64);
    //read(1, buf, 64);
    exec("syscall_test");
    halt();
}