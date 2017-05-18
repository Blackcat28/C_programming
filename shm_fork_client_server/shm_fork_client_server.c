/*
 * This program start like one process and allocate a shared memory
 * space, with a randomly generated key, and a semaphore with a 
 * randomly generated id.
 * After this initialization the process fork:
 * The father is the server that put the alphabet on the shared memory
 * The child is the client that read the alphabet on the shared memory
 * The semaphore is used for sync the two process
 */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


#define SHMSZ 27

/* 
 * Define union semun 
 */

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
    struct seminfo *__buf;
};

/* 
 * allocate the semaphore
 */
int binary_semaphore_allocation(key_t key, int sem_flags) {
		return semget(key, 1, sem_flags);	
}

/* 
 * initialize the semaphore 
 */
int binary_semaphore_initialize(int semid) {
    union semun argument;
    unsigned short values[1];
		values[0] = 1;
		argument.array = values;
    return semctl(semid, 0, SETALL, argument);
}

/*
 * deallocate the semaphore
 */
int binary_semaphore_deallocate(int semid)
{
    union semun ignored_argument;
    return semctl(semid, 1, IPC_RMID, ignored_argument);
}

/*
 * Semaphore wait for lock a critical section
 */
int binary_semaphore_wait(int semid) {
	struct sembuf operations[1];
		operations[0].sem_num = 0;
		operations[0].sem_op = -1;
		operations[0].sem_flg = SEM_UNDO;
	return semop(semid, operations, 1);
}

/*
 * Semaphore post for unlock a critical section
 */
int binary_semaphore_post(int semid) {
	struct sembuf operations[1];
		operations[0].sem_num = 0;
		operations[0].sem_op = 1;
		operations[0].sem_flg = SEM_UNDO;	
	return semop(semid, operations, 1);
}

/* 
 * From here the main start
 */ 

int main() {
    char c;
    int shmid;
    key_t key;
    char *shm, *s;
	int semid;

    /* 
     * For better security the key and the semid 
     * are pseudo-randomly generated 
     */
    srand(time(NULL));
    key = rand();
    semid = rand();
    
    /*
     * Now we can allocate the semaphore
     */
	binary_semaphore_allocation(key, IPC_NOWAIT);
     
    /*
     * Now we can initialize the semaphore
     */ 
	binary_semaphore_initialize(semid);

    /*
     * Create the segment.
     */
    if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    } 
    
    /* Now the program fork for create two process:
     * Father: Server
     * Son: Client
     * we don't have any problem for key sharig, cuz the fork clone
     * the two process that change them work only after the fork
     */
     pid_t pid = fork();
     

	/*  Child/Client Process */
	if (pid == 0) {  
     
    /*
    * Now we attach the segment to our data space.
    */
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        printf("%s\n", shm);
        perror("shmat");
        exit(1);
    }
	
	/* 
	 * Insert a sleep for been sure that the server take the firt lock
	 */ 
	sleep(1);
	
	/* 
	 * Try to Locking the shared memory for starting reading it 
	 */
	 binary_semaphore_wait(semid);
	
    /*
     * Now read what the server put in the memory.
     */
    for (s = shm; *s != 0x0; s++){
        putchar(*s);
	}
    putchar('\n');
   
	/*
	 * Now i can release the lock
	 */
	binary_semaphore_post(semid);
   exit(0);
   }
    

    /* Father/Server Process */
    else if (pid > 0)
    {
	
	/*
     * Now we attach the segment to our data space.
     */
    if ((shm = shmat(shmid, 0, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }


	/* 
	 * Locking the shared memory for starting filling it 
	 */
	 binary_semaphore_wait(semid);

    /*
     * Filling the Shared memory
     */
    s = shm;
    for (c = 'a'; c <= 'z'; c++) {
        *s++ = c;
	}
	
	/*
	 * Filling complete, release the shared memory lock
	 */
	 binary_semaphore_post(semid);
	
	/* After release the lock, the client take the lock and do what
	 * it must do, and after finished, the server can deallocate 
	 * the shared memory before die 
	 */

	binary_semaphore_wait(semid);
           
    /* deallocate the shared memory segment */
	shmctl(shmid, IPC_RMID, 0);

	/*
	 * Now i can release the lock, deallocate the semaphore and 
	 * close the server
	 */
	binary_semaphore_post(semid);

	/*
	 * Deallocate the semaphore 
	 */
	binary_semaphore_deallocate(semid);
   exit(0);	
   }
   
   /* 
    * Fork failed condition warning 
    */
    else
    {
        printf("fork() failed!\n");
        return -1;
    }
    

return 0;    
} /* close the main */
