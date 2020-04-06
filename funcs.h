#pragma once
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

extern bool all;
extern bool bytes;
extern bool dereference;
extern bool separate;
extern int blockSize;
extern int maxDepth;
extern char* dir;

bool invalidArgs(char **argv, int argc);

int list();