#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include "funcs.h"

clock_t startTime;
int privFIFO;
int i;

void *threadFunction(void *arg){

  int publicFIFO, wcTime;
  char pubFIFOPath[256];
  char request[256];
  struct Reply reply;

  wcTime=(rand()%(10 - 1 + 1))+1;

  sprintf(pubFIFOPath, "/tmp/%d.%ld", getpid(), pthread_self());
  if(mkfifo(pubFIFOPath,0660)==-1){
  	perror("Error creating public fifo for client");
  	exit(1);
  }
  publicFIFO=open(pubFIFOPath,O_RDONLY|O_NONBLOCK);
  if(publicFIFO==-1){
  	perror("Error opening public fifo for client");
  	exit(1);
  }

  sprintf(request,"[ %d, %d, %lu, %d, -1 ]", i, getpid(), pthread_self(), wcTime);
  if(write(privFIFO,&request,sizeof(request))==-1){
  	perror("Error writing client request to private fifo");
  	exit(1);
  }

  while(read(publicFIFO,&reply,sizeof(reply))<=0){
  	usleep(10000);
  }

  close(publicFIFO);
  unlink(pubFIFOPath);


  return NULL;
}

int main(int argc, char **argv, char **envp)
{
  struct FLAGS flags;
  clock_t execTime;

  i=0;

  srand(time(NULL));

  if(readFlags(argv,argc, &flags)){
  	write(STDOUT_FILENO,"Bad arguments. Usage: Un <-t nsecs> fifoname\n", strlen("Bad arguments. Usage: Un <-t nsecs> fifoname\n"));
    exit(1);
  }

  do{
  	privFIFO = open(flags.fifoname,O_WRONLY);
  	if(privFIFO==-1){
  		usleep(500000);
  	}
  }while(privFIFO==-1);

  time(&startTime);

  while ( elapsedTime(&startTime,&execTime) < flags.nsecs ) {
  	i++;
  	pthread_t tid;
  	pthread_create(&tid,NULL,threadFunction,NULL);
    usleep(100000);
  }
  
  return 0;
  
}