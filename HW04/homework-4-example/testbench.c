#include <stdio.h>
#include "testbench.h"

int main(int argc, char* argv[])
{
  int result;

  printf("Starting toplevel_fnc\n");

  result=toplevel_fnc();

  printf("toplevel_fnc returns %d\n", result);
  
  return 0;
}
