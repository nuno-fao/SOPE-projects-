#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include "funcs.h"

clock_t startTime;
int totalTime;

void *threadFunction(void *arg){

  char* request = (char *) arg;
  char pvFifoname[256];
  clock_t execTime;
  pid_t pid;
  int i, tid, dur, pl, pvfd;
  struct Reply answer;

  sscanf(request,"[%d, %d, %d, %d, %d]",&i, &pid, &tid, &dur, &pl);

  answer.i=i;
  answer.pid=getpid();
  answer.tid=tid;
  answer.dur=dur;

  sprintf(pvFifoname, "/tmp/%d.%d", pid, tid);
  
  pvfd = open(pvFifoname,O_WRONLY);   //open private fifo to answer the request
  if(pvfd==-1){
    perror("Error opening client fifo for the answer");
    pthread_exit((void*) 1);
  }

  if(elapsedTime(&startTime,&execTime)<totalTime){  //checks if bathroom is closed
    answer.pl=1;    //to be changed later
    write(pvfd,&answer,sizeof(answer));   //answer the client
    usleep(dur*1000);   //wait while client is using the bathroom
  }
  else{
    answer.pl=-1;    //to be changed later
    write(pvfd,&answer,sizeof(answer));   //answer the client
  }
  
  close(pvfd);

	return NULL;
}

int main(int argc, char **argv, char **envp)
{
  struct FLAGS flags;
  clock_t execTime;
  char request[256];
  int fd;

  if(readFlags(argv,argc, &flags)){		//reads arguments passed on function call
  	write(STDOUT_FILENO,"Bad arguments. Usage: Qn <-t nsecs> [-l nplaces] [-n nthreads] fifoname\n",strlen("Bad arguments. Usage: Qn <-t nsecs> [-l nplaces] [-n nthreads] fifoname\n"));
    exit(1);
  }

  if(mkfifo(flags.fifoname,0660)==-1){		//create fifo given in the arguments
  	perror("Error making fifo");
  	exit(2);
  }

  fd = open(flags.fifoname, O_RDONLY | O_NONBLOCK);	//open fifo for reading

  time(&startTime);	//initiate time counting
  totalTime=flags.nsecs;


  while ( elapsedTime(&startTime,&execTime) < flags.nsecs ) {
  	while (read(fd, &request, 256) <= 0) {
      	usleep(10000);
  	}
  	pthread_t tid;
  	pthread_create(&tid,NULL,threadFunction,&request);
  }

  close(fd);
  unlink(flags.fifoname);
  
  return 0;
  
}