#include "funcs.h"

bool invalidArgs(char **argv, int argc ){
	char *link1 = "-l\0";
	char *link2 = "--count-links\0";
	char *all1 = "-a\0";
	char *all2 = "--all\0";
	char *bytes1 = "-b\0";
	char *bytes2 = "--bytes\0";
	char *block1 = "-B\0";
	char *block2 = "--block-size=";
	char *dereference1 = "-L\0";
	char *dereference2 = "--dereference\0";
	char *separate1 = "-S\0";
	char *separate2 = "--separate-dirs\0";
	char *max_depth = "--max-depth=";

	if(strncmp(link1,*(argv+1),3)!=0 && strncmp(link2,*(argv+1),13)!=0){
		return true;
	}

	for(char **arg=argv+3;*arg!=0;arg++){
		char *thisarg=*arg;
		if(strncmp(all1,thisarg,3)==0||strncmp(all2,thisarg,6)==0){
			all=true;
			printf("check 1\n");
			continue;
		}
		else if(strncmp(bytes1,thisarg,3)==0||strncmp(bytes2,thisarg,8)==0){
			bytes=true;
			printf("check 2\n");
			continue;
		}
		else if(strncmp(block1,thisarg,3)==0||strncmp(block2,thisarg,13)==0){
			if(strncmp(block1,thisarg,3)==0){
				arg++;
				blockSize=atoi(*arg);
			}
			else{
				thisarg+=13;
				blockSize=atoi(thisarg);
			}
			continue;
		}
		else if(strncmp(dereference1,thisarg,3)==0||strncmp(dereference2,thisarg,14)==0){
			dereference=true;
			printf("check 3\n");
			continue;
		}
		else if(strncmp(separate1,thisarg,3)==0||strncmp(dereference2,thisarg,16)==0){
			separate=true;
			printf("check 4\n");
			continue;
		}
		else if(strncmp(max_depth,thisarg,12)==0){
			thisarg+=12;
			maxDepth=atoi(thisarg);
			continue;
		}
		return true;
	}

	return false;
}