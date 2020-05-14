#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include "funcs.h"
#include "registers.h"

struct timeval startTime;
int totalTime;
int realPL;
int totalstalls;
char* argu= 'test';


void *threadFunction(void *arg, int stalls[], int totalSt ){

  char* request = (char *) arg;
  char pvFifoname[256];
  struct timeval execTime;
  pid_t pid;
  int i, dur, pl, pvfd;
  long unsigned int tid;
  struct Reply answer;

  sscanf(request,"[%d, %d, %ld, %d, %d]",&i, &pid, &tid, &dur, &pl);

  op_reg_message(elapsedTime(&startTime,&execTime), i, getpid(), tid, dur, pl, "RECVD");

  answer.i=i;
  answer.pid=getpid();
  answer.tid=pthread_self();
  answer.dur=dur;
  sprintf(pvFifoname, "/tmp/%d.%ld", pid, tid);
  int stallnumber=0; 
  
  do{
    pvfd = open(pvFifoname,O_WRONLY);   //open private fifo to answer the request
    if(pvfd==-1){
      usleep(5000);
    }
    if(elapsedTime(&startTime,&execTime)>=totalTime){
      op_reg_message(elapsedTime(&startTime,&execTime), i, getpid(), tid, dur, -1, "GAVUP");
      close(pvfd);
      return NULL;
    }
  }while(pvfd==-1);

  

  if( ( (elapsedTime(&startTime,&execTime)<totalTime)) || checkstallvacancy(stalls, totalSt)){  //checks if bathroom is closed
    stallnumber=checkstallvacancy(stalls, totalSt);
    op_reg_message(elapsedTime(&startTime,&execTime), i, getpid(), tid, dur, stallnumber, "ENTER");
    answer.pl=1;    //to be changed later
    write(pvfd,&answer,sizeof(answer));   //answer the client
    usleep(dur*1000);   //wait while client is using the bathroom
    stalls[stallnumber]=0;     //stall becomes free after client uses bathroom
    op_reg_message(elapsedTime(&startTime,&execTime), i, getpid(), tid, dur, stallnumber, "TIMUP");
  }
  else{
    op_reg_message(elapsedTime(&startTime,&execTime), i, getpid(), tid, dur, -1, "2LATE");
    answer.pl=-1;    //to be changed later
    write(pvfd,&answer,sizeof(answer));   //answer the client
  }
  
  close(pvfd);

	return NULL;
}

int main(int argc, char **argv, char **envp)
{
  struct FLAGS flags;
  struct timeval execTime;
  char request[256];
  int fd;
  realPL = 0;

  if(readFlags(argv,argc, &flags)){		//reads arguments passed on function call
  	write(STDOUT_FILENO,"Bad arguments. Usage: Qn <-t nsecs> [-l nplaces] [-n nthreads] fifoname\n",strlen("Bad arguments. Usage: Qn <-t nsecs> [-l nplaces] [-n nthreads] fifoname\n"));
    exit(1);
  }
  

  if(mkfifo(flags.fifoname,0660)==-1){		//create fifo given in the arguments
  	perror("Error making fifo");
  	exit(2);
  }

  fd = open(flags.fifoname, O_RDONLY | O_NONBLOCK);	//open fifo for reading

  gettimeofday(&startTime,NULL);	//initiate time counting
  totalTime=flags.nsecs;
  realPL = flags.nthreads; //counter for available threads
  totalstalls = flags.nplaces; //total stalls available
  int stallgrid[totalstalls]; //array of stalls
  memset(stallgrid, 0, sizeof(stallgrid)); //sets zero value to each stall, meaning unoccupied





  while ( ((elapsedTime(&startTime,&execTime) < (double)flags.nsecs )) || (realPL > 0)) {
  	while (read(fd, &request, 256) <= 0) {
      	usleep(5000);
        if( ((elapsedTime(&startTime,&execTime) >= (double)flags.nsecs)) || (realPL == 0)) {
          close(fd);
          unlink(flags.fifoname);
          pthread_exit((void*)0);
        }
  	}
  	pthread_t tid;
    realPL--;
  	pthread_create(&tid,NULL,threadFunction(argu, stallgrid, totalstalls),&request);
    pthread_detach(tid);
  }

  close(fd);
  unlink(flags.fifoname);
  pthread_exit((void*)0);
  
  return 0;
  
}