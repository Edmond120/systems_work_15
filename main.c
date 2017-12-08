#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

union semun {

	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo *__buf;
};

char* read_from_story(int size){
	
	
}

int main(){

	struct sembuf sb;
	sb.sem_op = -1;	
	int semd = semget(SEM, 1, 0777);
	semop(sem, &sb, 1);
	int shmd = shmget(SHM, sizeof(int), IPC_CREAT);
	int last_size = *shmat(shmd, 0, 0);
	//char* last_line = read_from_story(last_size);
	printf("Last Line: %s\n", last_line);
	
}


