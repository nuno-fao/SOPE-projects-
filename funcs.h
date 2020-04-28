#pragma once
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

struct FLAGS{
	int nsecs;
	int nplaces;
	int nthreads;
	char* fifoname;
};

//reads flags and checks if they are valid or not
bool readFlags(char **argv, int argc, struct FLAGS* flags);

void printFlags(struct FLAGS* flags);
