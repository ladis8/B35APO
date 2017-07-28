/* Read number and produce "data" of given length */

#include "testbench.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

DATA_ATTR char out_file_name[] = "data";

FUNC_ATTR int subroutine_fnc(int fd, int numeric_base)
{
  
  //fd v s4 //numeric_base v s3

  int val = 0; //s0
  int err_res;
  int res;
  char ch;
  
  do {

    res = INTERNAL_SYSCALL(read, err_res, 3, fd, &ch, 1);
    //if the call is not succesfull res is set to -1

    if (res != 1)
      INTERNAL_SYSCALL(exit, err_res, 1, 1);

    if((ch<'0') || (ch>='0'+numeric_base))
      break;

    val *= numeric_base;
    val += ch - '0';
  } while(1);

  return val;
}


FUNC_ATTR int toplevel_fnc(void)
{
  int ret_val;
  int err_res;
  int count;
  int result;
  char ch = 'A';
  int fd;

  count = subroutine_fnc(0, 10);

  result = count;


  fd = INTERNAL_SYSCALL(open, err_res, 3, out_file_name, O_RDWR|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
  if(fd<0)
    INTERNAL_SYSCALL(exit, err_res, 1, 1);

  while(count--) {
    ret_val = INTERNAL_SYSCALL(write, err_res, 3, fd, &ch, 1);
    if(ret_val<0)
      INTERNAL_SYSCALL(exit, err_res, 1, 1);
  }

  ret_val = INTERNAL_SYSCALL(close, err_res, 1, fd);
  if(ret_val<0)
    INTERNAL_SYSCALL(exit, err_res, 1, 1);

  return result;
}
