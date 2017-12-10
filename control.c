#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "keys.h"
#include <unistd.h>

int descriptor = -1;
void flag_c(){//inits everything
	int id = shmget(SHM,sizeof(int),600 | IPC_CREAT | IPC_EXCL);
	if(descriptor == -1 && access("story.txt",F_OK) && id != -1){
		printf("shared memory created\n");
		descriptor = semget(SEM,1,IPC_CREAT | 0600);
		semctl(descriptor,0,SETVAL,1);
		printf("semaphore created: %d\n",descriptor);
		FILE * file = fopen("story.txt","w");
		fclose(file);
		printf("story created\n");	
	}
	else{
		printf("semaphore or story or shared memory already exists\n");
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
	int id = shmget(SHM,sizeof(int),0600);
	if(id == -1){
		printf("shared memory can't be removed since it does not exist\n");
	}
	else{
		shmctl(id,IPC_RMID,NULL);
		printf("shared memory marked for removal\n");
	}
	struct sembuf s;
	s.sem_op = -1;
	if(descriptor != -1){
		semop(descriptor,&s,1);
	}
	if(access("story.txt",F_OK)){
		printf("story can't be removed since it does not exist\n");
	}
	else{
		if(descriptor == -1){
			printf("semaphore does not exist, but story does1?!?!\n");
		}
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
