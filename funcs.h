#pragma once
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>
#include <sys/wait.h>

struct FLAGS{

	bool all;
	bool bytes;
	bool link;
	bool dereference;
	bool separate;
	int blockSize;
	int maxDepth;
	char* dir;
};

//reads flags and checks if they are valid or not
bool readFlags(char **argv, int argc, struct FLAGS* flags);

//will list files and directories in the directory in flags.dir
int list(struct FLAGS* flags,char* path, int depth);