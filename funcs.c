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

	if(!strncmp(link1,*(argv+1),2)&&!strncmp(link2,*(argv+1),13)){
		return true;
	}

	for(char **arg=argv+2;*arg!=0;arg++){
		char *thisarg=*arg;
		if(strncmp(bytes1,thisarg,2)||strncmp(bytes2,thisarg,7)){
			//something
			continue;
		}
		else if(strncmp(block1,thisarg,2)||strncmp(block2,thisarg,13)){
			//something
			continue;
		}
		else if(strncmp(dereference1,thisarg,2)||strncmp(dereference2,thisarg,13)){
			//something
			continue;
		}
		else if(strncmp(separate1,thisarg,2)||strncmp(dereference2,thisarg,15)){
			//something
			continue;
		}
		else if(strncmp(maxDepth,thisarg,12)){
			//something
			continue;
		}
		return true;
	}

	return false;
}