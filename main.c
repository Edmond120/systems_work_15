#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "keys.h"
/*
union semun {

	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo *__buf;
}; */

char* read_from_story(int size){

	return "Hello";
}

int write_to_story(char* line){
	
	int fd = open("story.txt", O_CREAT|O_APPEND|O_WRONLY, 0777);
	char s[100];
	printf("Line: %s\n", line);
	strcpy(s, line);
	//strcpy(s, line);
	printf("S: %s\n", s);
	write(fd, line, strlen(s));
	return close(fd);
}

int main(){

	struct sembuf sb;
	sb.sem_op = -1;
	int semd = semget(SEM, 1, 0777);
	semop(semd, &sb, 1);
	int shmd = shmget(SHM, sizeof(int), IPC_CREAT);
	int* last_size_p = shmat(shmd, 0, 0);
	int last_size = *last_size_p;
	char* last_line = read_from_story(last_size);
	printf("Last Line: %s\n\n", last_line);
	printf("Enter the next line: ");
	char next_line[200];
	fgets(next_line, sizeof(next_line), stdin);
	last_size = sizeof(next_line);
	write_to_story(next_line);
	shmdt(&last_size);
	sb.sem_op = 1;
	semop(semd, &sb, 1);
	return 0;
	write_to_story("Hi\n");
	write_to_story("This is a test\n");
	return 0;
}
