#include "funcs.h"

bool invalidArgs(char **argv, int argc ){
	char *link1 = "-l";
	char *link2 = "--count-links";
	char *bytes1 = "-b";
	char *bytes2 = "--bytes";
	char *block1 = "-B";
	char *block2 = "--block-size=";
	char *dereference1 = "-L";
	char *dereference2 = "--dereference";
	char *separate1 = "-S";
	char *separate2 = "--separate-dirs";
	char *maxDepth = "--max-depth=";

	if(strncmp(link1,(*argv)[1],2)||strncmp(link2,(*argv)[1],13)){
		return false;
	}
	return true;
}