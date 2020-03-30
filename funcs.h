#pragma once
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

extern bool all;
extern bool bytes;
extern bool dereference;
extern bool separate;
extern int blockSize;
extern int maxDepth;

bool invalidArgs(char **argv, int argc);