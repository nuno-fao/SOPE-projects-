#include <stdio.h>
#include "funcs.h"


int main(int argc, char **argv, char **envp)
{
  struct FLAGS flags;

  if(readFlags(argv,argc, &flags)){
  	write(STDOUT_FILENO,"Bad arguments. Usage: Un <-t nsecs> fifoname\n", strlen("Bad arguments. Usage: Un <-t nsecs> fifoname\n"));
    exit(1);
  }

  printFlags(&flags);
  
  return 0;
  
}