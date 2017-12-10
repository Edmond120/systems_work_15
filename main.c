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
#include <errno.h>
#include "keys.h"

/*
union semun {

	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo *__buf;
}; */

int print_error(int result){
  if(result == -1){
    printf("Error: %s\n", strerror(errno));
    return -1;
  }
  return 0;
}

char* read_from_story(int size){
	int fd = open("story.txt", O_RDONLY, 0777);
	if(print_error(fd)){
		printf("\nBe sure to run ./control -c first\n");
		return NULL;
	}
	int seek = lseek(fd, (-1 * size), SEEK_END);
	if(print_error(seek)) return NULL;
	char* c = (char*) calloc(size, sizeof(char));
	int readd = read(fd, c, size);
	if(print_error(readd)) return NULL;
	return c;
}

int write_to_story(char* line){

	int fd = open("story.txt", O_APPEND|O_WRONLY, 0777);
	if(print_error(fd)){
		printf("\nBe sure to run ./control -c first\n");
		return -1;
	}
	char s[strlen(line) + 1];
	//printf("Line: %s\n", line);
	strcpy(s, line);
	//printf("S: %s\n", s);
	write(fd, line, strlen(s));
	return close(fd);
}

int main(){

	//Down the semaphore
	struct sembuf sb;
	sb.sem_op = -1;

	//Get the semaphore
	int semd = semget(SEM, 1, 0777);
	if(print_error(semd)) return -1;
	semop(semd, &sb, 1);

	//get shared memory
	int shmd = shmget(SHM, sizeof(int), 0777|IPC_CREAT);
	if(print_error(shmd)) return -1;
	int* last_size_p = shmat(shmd, 0, 0);

	//read correct number bytes (gets last line)
	char* last_line = read_from_story(*last_size_p);

	//Prints last line and prompts user to enter next line
	printf("Last Line: %s\n\n", last_line);
	printf("Enter the next line: ");
	char next_line[200];
	fgets(next_line, sizeof(next_line), stdin);
	*last_size_p = strlen(next_line);
	write_to_story(next_line);

	//Detatch Shared Memory
	shmdt(&last_size_p);

	//Up the semaphore
	sb.sem_op = 1;
	semop(semd, &sb, 1);
	return 0;
}
