#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "funcs.h"
#include "registers.h"

struct timeval startTime;
int privFIFO;
int totalTime;
int i;

void *threadFunction(void *arg){

  int publicFIFO, wcTime;
  struct timeval execTime;
  struct stat statbuf;
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

  op_reg_message(elapsedTime(&startTime,&execTime), i, getpid(), pthread_self(), wcTime, -1, "IWANT");

  if(fstat(privFIFO,&statbuf)==-1){
  	op_reg_message(elapsedTime(&startTime,&execTime), i, getpid(), pthread_self(), wcTime, -1, "CLOSD");
  	return NULL;
  }
  else{
  	write(privFIFO,&request,sizeof(request));
  }
  
  while(read(publicFIFO,&reply,sizeof(reply))<=0){
  	usleep(5000);
  	if(elapsedTime(&startTime,&execTime) >= totalTime){
          op_reg_message(elapsedTime(&startTime,&execTime), i, getpid(), pthread_self(), wcTime, -1, "FAILD");
          close(publicFIFO);
          unlink(pubFIFOPath);
          return NULL;
       }
  }

  op_reg_message(elapsedTime(&startTime,&execTime), i, getpid(), pthread_self(), wcTime, -1, "IAMIN");

  close(publicFIFO);
  unlink(pubFIFOPath);


  return NULL;
}

int main(int argc, char **argv, char **envp)
{
  struct FLAGS flags;
  struct timeval execTime;

  i=0;

  srand(time(NULL));

  if(readFlags(argv,argc, &flags)){
  	write(STDOUT_FILENO,"Bad arguments. Usage: Un <-t nsecs> fifoname\n", strlen("Bad arguments. Usage: Un <-t nsecs> fifoname\n"));
    exit(1);
  }

  totalTime = flags.nsecs;

  do{
  	privFIFO = open(flags.fifoname,O_WRONLY);
  	if(privFIFO==-1){
  		usleep(500000);
  	}
  }while(privFIFO==-1);

  gettimeofday(&startTime,NULL);

  while ( elapsedTime(&startTime,&execTime) < (double)flags.nsecs ) {
  	i++;
  	pthread_t tid;
  	pthread_create(&tid,NULL,threadFunction,NULL);
  	pthread_detach(tid);
    usleep(5000);
  }
  
  close(privFIFO);
  return 0;
  
}