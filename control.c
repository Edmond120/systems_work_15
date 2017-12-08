#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "keys.h"
#include <unistd.h>

int descriptor = -1;
void flag_c(){//inits everything
	if(descriptor == -1 && access("story.txt",F_OK)){
		descriptor = semget(SEM,1,IPC_CREAT | 0600);
		semctl(descriptor,0,SETVAL,1);
		printf("semaphore created: %d\n",descriptor);
		FILE * file = fopen("story.txt","w");
		fclose(file);
		printf("story created\n");
	}
	else{
		printf("semaphore or story already exists\n");
		return;
	}
}
void flag_v(){//views story
	if(descriptor == -1){
		printf("semaphore is not created yet\n");
		return;
	}
	else if(access("story.txt",F_OK)){
		printf("story does not exist\n");
		return;
	}
	else{
		printf("story\n-----\n\n");
		FILE * file = fopen("story.txt","r");
		int c;
		while((c = getc(file)) != EOF){
			putchar(c);
		}
		fclose(file);
	}
}
void flag_r(){//removes everything
	if(access("story.txt",F_OK)){
		printf("story can't be removed sincei it does not exist\n");
	}
	else{
		unlink("story.txt");
		printf("story removed\n");
	}
	if(descriptor == -1){
		printf("semaphore can't be removed since it does not exist\n");
		return;
	}
	else{
		if(semctl(descriptor,0,IPC_RMID) == 0){
			printf("semaphore removed\n");
			return;
		}
		else{
			printf("error removing semaphore\n");
		}
	}
}

int main(int argc, char ** argv){
	if(argc <= 1){
		printf("invalid number of arguments\n");
		return 0;
	}
	descriptor = semget(SEM,1,0);
	if(!strcmp(argv[1],"-c")){
		flag_c();
	}
	else if(!strcmp(argv[1],"-v")){
		flag_v();
	}
	else if(!strcmp(argv[1],"-r")){
		flag_r();
	}
	else{
		printf("unknown flag: %s\n",argv[1]);
	}
	return 0;
}
