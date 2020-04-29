#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include "funcs.h"


void *threadFunction(void *arg){

  char* request = (char *) arg;
  char pvFifoname[256];
  pid_t pid;
  int i, tid, dur, pl, pvfd;
  struct Reply answer;
  
  answer.i=i;
  answer.pid=getpid();
  answer.tid=tid;
  answer.dur=dur;

  sscanf(request,"[%d, %d, %d, %d, %d]",&i, &pid, &tid, &dur, &pl);
  sprintf(pvFifoname, "/tmp/%d.%d", pid, tid)
  
  pvfd = open(pvFifoname,O_WRONLY);   //open private fifo to answer the request
  if(pvfd==-1){
    perror("Error opening client fifo for the answer");
    pthread_exit((void*) 1);
  }

  answer.pl=1;    //to be changed later
  write(pvFifoname,&answer,sizeof(answer));   //answer the client
  usleep(dur*1000);   //wait while client is using the bathroom
  close(pvFifoname);

	return NULL;
}

int main(int argc, char **argv, char **envp)
{
  struct FLAGS flags;
  clock_t startTime, execTime;
  char request[256];
  int fd;
  bool running = true;

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


  while ( elapsedTime(&startTime,&execTime) < flags.nsecs ) {
  	while (read(fd, &request, 256) <= 0) {
      	usleep(10000);
  	    if(elapsedTime(&startTime,&execTime) > flags.nsecs){
          running = false;
          break;
  	    }
  	}
    if(!running){
      break;
    }
  	pthread_t tid;
  	pthread_create(&tid,NULL,threadFunction,&request);
  }

  close(fd);
  unlink(flags.fifoname);
  
  return 0;
  
}