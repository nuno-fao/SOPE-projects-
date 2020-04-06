#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include "funcs.h"

int main(int argc, char **argv, char **envp)
{
  
  if(argc<3){
  	write(STDOUT_FILENO,"Insufficient arguments.\nUsage: simpledu -l [path] [-a] [-b] [-B size] [-L] [-S] [--max-depth=N]\nOrder isn't important\n",strlen("Insufficient arguments.\nUsage: simpledu -l [path] [-a] [-b] [-B size] [-L] [-S] [--max-depth=N]\nOrder isn't important\n"));
	exit(1);  
  }

  struct FLAGS flags;

  if(readFlags(argv,argc, &flags)){
  	write(STDOUT_FILENO,"Bad arguments.\nUsage: simpledu -l [path] [-a] [-b] [-B size] [-L] [-S] [--max-depth=N]\nOrder isn't important\n",strlen("Bad arguments.\nUsage: simpledu -l [path] [-a] [-b] [-B size] [-L] [-S] [--max-depth=N]\nOrder isn't important\n"));
  	exit(2);
  }

  list(&flags);

  return 0;
  
}