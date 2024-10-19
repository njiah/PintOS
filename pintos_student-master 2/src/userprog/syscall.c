#include "userprog/syscall.h"
#include "userprog/process.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "lib/stdio.h"
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "threads/palloc.h"
#include "devices/shutdown.h"
#include "devices/input.h"
#include "filesys/filesys.h"
#include "filesys/file.h"
#include "lib/user/syscall.h"
#include "lib/string.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

void halt(void)
{
  printf("halt! \n");
  shutdown_power_off();
}

pid_t exec(const char *cmd_line)
{
  // Split cmd_line into file name and arguments
  char *file_name = palloc_get_page(0);
  strlcpy(file_name, cmd_line, PGSIZE);
  char *save_ptr;
  file_name = strtok_r(file_name, " ", &save_ptr);
  // Remove file name from cmd_line
  cmd_line += strlen(file_name) + 1;
  pid_t pid = process_execute(file_name, cmd_line);
  if (pid == TID_ERROR)
    return -1;
  return pid;
}




void exit(int status) {
  if (status == EXIT_FAILURE || status > 0)
  {
    printf("%s: exit failure(%d)\n", thread_name(), status);
  }
  struct thread *cur = thread_current();
  cur->exit_status = status;
  printf("exit sytem call");
  thread_exit();
}



bool create (const char *file, unsigned initial_size){
  filesys_create(file, initial_size);
  printf("file created\n");
}

bool remove (const char *file){
  filesys_remove(file);
  printf("file removed\n");
}

int open(const char *file){
  struct file *data = filesys_open(file);
  if (data == NULL)
   return -1;
  int fd = 2;
  // Find first available fd, 0/1 are reserved for stdin/stdout
  while (fd < 128)
  {
    if (thread_current()->fd_table[fd] == NULL)
    {
      thread_current()->fd_table[fd] = data;
      return fd;
      printf("Open system call. \n");
    }
    fd++;
  }
  return -1;
}



int read(int fd, void *buffer, unsigned size) {
  ASSERT(buffer != NULL);
  ASSERT(size>0);
  if (fd == STDOUT_FILENO)
  {
    for (unsigned int i=0; i< size; i++)
    {
      ((char*)buffer)[i] = input_getc();
    }
    return size;
  }
  else
  {
    struct file *f = thread_current()->fd_table[fd];
    if (f == NULL)
      return -1;
    int bytes = file_read(f, buffer, size);
    //printf("Reading %d bytes from the file %s\n", bytes, f->name);
    printf("Read system call.\n");
    return bytes;
  }
  return -1;
}

int write(int fd, const void *buffer, unsigned size) {
  ASSERT(buffer != NULL);
  ASSERT(size>0);
  if (fd == STDOUT_FILENO) {
    putbuf(buffer, size);
    return size;
    printf("Write system call. \n");

  }
  else
  {
    struct file *f = thread_current()->fd_table[fd];
    //printf("write file %s\n", f->name);
    if (f == NULL)
      return -1;

    return file_write(f, buffer, size);
  }
  return -1;
}


void close (int fd){
  struct thread *cur = thread_current();
  struct file *f = cur->fd_table[fd];
  printf("%s: close(%d)\n", cur->name, fd);
  file_close(f);
  printf("Close system call. \n");
}



static void
syscall_handler (struct intr_frame *f UNUSED)
{
  int *p = f->esp;
  int sys_code = *(int*)f->esp;
  switch(sys_code)
  {
    case SYS_HALT:
      halt();
      break;

    case SYS_EXEC:
      int ret = exec((const char *)*(p + 1));
      f->eax = ret;
      break;
    
    case SYS_EXIT:
      exit(*(p+1));
      break;
    case SYS_CREATE:
      f->eax = create((const char *)*(p+1), (int32_t)*(p+2));
      break;
    case SYS_WRITE:
      int ret_write = write(*(p + 1), (const void *)*(p + 2), (unsigned)*(p + 3));
      f->eax = ret_write;
      break;
    case SYS_READ:
      int ret_read = read(*(p + 1), (void *)*(p + 2), (unsigned)*(p + 3));
      f->eax = ret_read;
      break;
    case SYS_REMOVE:
      f->eax = remove((const char *)*(p+1));
      break;
    case SYS_OPEN:
      int ret_open = open((const char*)*(p+1));
      f->eax = ret_open;
      break;
    case SYS_CLOSE:
      close(*(p+1));
      break;
    default:
      printf("Default system call!\n");
      break;
  }
  //thread_exit ();
}


