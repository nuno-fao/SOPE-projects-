#include "funcs.h"

bool readFlags(char **argv, int argc, struct FLAGS* flags){
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

	flags->all = flags->bytes = flags->dereference = flags->link = flags->separate=false;
	flags->blockSize = flags->maxDepth = 0;
	flags->dir = "";

	for(char **arg=argv+1;*arg!=0;arg++){
		char *thisarg=*arg;

		//will check each flag and if it has been already used
		if((strncmp(all1,thisarg,3)==0||strncmp(all2,thisarg,6)==0)&&flags->all==false){
			flags->all=true;
			continue;
		}
		else if((strncmp(link1,thisarg,3)==0 || strncmp(link2,thisarg,14)==0) && flags->link==false){
			flags->link=true;
			continue;
		}
		else if((strncmp(bytes1,thisarg,3)==0||strncmp(bytes2,thisarg,8)==0)&&flags->bytes==false){
			flags->bytes=true;
			continue;
		}
		else if((strncmp(block1,thisarg,3)==0||strncmp(block2,thisarg,13)==0)&&flags->blockSize==false){
			if(strncmp(block1,thisarg,3)==0){
				arg++;
				flags->blockSize=atoi(*arg);
			}
			else{
				thisarg+=13;
				flags->blockSize=atoi(thisarg);
			}
			continue;
		}
		else if((strncmp(dereference1,thisarg,3)==0||strncmp(dereference2,thisarg,14)==0)&&flags->dereference==false){
			flags->dereference=true;
			continue;
		}
		else if((strncmp(separate1,thisarg,3)==0||strncmp(separate2,thisarg,16)==0)&&flags->separate==false){
			flags->separate=true;
			continue;
		}
		else if((strncmp(max_depth,thisarg,12)==0)&&flags->maxDepth==0){
			thisarg+=12;
			flags->maxDepth=atoi(thisarg);
			continue;
		}
		else if(strlen(flags->dir)==0){
			flags->dir=thisarg;
			continue;
		}
		return true;	//returns true if any input is wrong
	}

	if(flags->link==false) return true; //links is a must

	return false; //read flags with no errors
}

int list(struct FLAGS* flags,char* path){
	struct dirent* newFile;
	struct stat statBuffer;

    DIR* source_dir = opendir(flags->dir);

    if (source_dir == NULL) return -1;

    while ((newFile = readdir(source_dir)) != NULL){

        if(stat(newFile->d_name, &statBuffer) == -1){
            printf("Could not read from %s\n",newFile->d_name);
            continue;
        }

    	char fullPath[1024]="";
    	strcat(fullPath,path);
    	strcat(fullPath,"/");
    	strcat(fullPath,newFile->d_name);

        if(S_ISREG(statBuffer.st_mode)){

        	printf("%li\t%s\n",statBuffer.st_size,fullPath);


        }

		else if(S_ISDIR(statBuffer.st_mode)){

        	printf("%li\t%s\n",statBuffer.st_size,fullPath); 	//isto é para sair, just testing
		}
    }

    closedir(source_dir);
    return 0;
}