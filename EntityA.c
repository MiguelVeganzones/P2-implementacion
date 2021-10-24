#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include "message.h"
#include "msgq.h"



//Defining the instances of the created structured and the identifiers

//struct Messages_queue* mesg_queue_ptr; 
struct Memory_shared  *mesg_mem_shared_ptr;//ptr to message structure, which is in the shared memory segment 

int	shmid, clisem, servsem;	/* shared memory and semaphore IDs */
	
//Semaphore structure (explain what means each line)

union semun {
	int val;
	struct semid_ds* buf;
	ushort* array;
} semctl_arg;

//Semaphore functions

void set_free_sem(int num) {
	struct sembuf operasem;
	operasem.sem_num = num;
	operasem.sem_op = 1; //liberate the semaphore  
	operasem.sem_flg = 0;
	if (semop(sem, &operasem, 1) < 0) {
		
	}
}

void take_sem(int num) {
	struct sembuf operasem;
	operasem.sem_num = num;
	operasem.sem_op = -1; //take the semaphore
	operasem.sem_flg = 0;
	if (semop(sem, &operasem, 1) < 0) {
		
	}
}

int main() {

	//Declaramos una variable para guardar el ID de la cola
	int interfaz1;

	// Making messages queue for user1
	if ((interfaz1 = msgget(MKEYQ1, PERMS | IPC_CREAT)) < 0) 
		err_sys("EntityA: can't make messages queue");

	/*
	 * Get the shared memory segment and attach it.
	 * The server must have already created it.
	 */

	if ((shmid = shmget(SHMKEY, sizeof(struct Memory_shared), 0)) < 0)
		err_sys("EntityA: can't get shared memory segment");

	//Pointer to memory

	/*
	 Para hacer uso de la memoria compartida los procesos deben ligar (attach) el segmento de memoria compartida. Esto se hace con la función shmat()
	 void * shmat (int shmid, void *shmaddr, int shmflg);

     shmaddr es la dirección a la cual la memoria debe ser ligada. Se recomienda especificar 0 (y el sistema elige la mejor dirección).
     Si shmflg contiene el flag SHM_RDONLY, la memoria es de sólo lectura, en otro caso es lectura y escritura.
	  
     Para desligar (detach) la memoria compartida:
	 int shmdt( void *shmaddr);

     Donde shmaddr es el valor retornado por shmat().
	
	*/

	if (( mesg_mem_shared_ptr=(struct Memory_shared*)shmat(shmid, (char*)0, 0)) == (struct Memory_shared*)-1)
		err_sys("client: can't attach shared memory segment");

	/*
	 * Open the two semaphores. The server must have created them already.
	 */

	if ((clisem = sem_open(SEMKEY1)) < 0)
		err_sys("client: can't open client semaphore");

	if ((servsem = sem_open(SEMKEY2)) < 0)
		err_sys("client: can't open server semaphore");
	
	client();

	/*
	 * Detach and remove the shared memory segment and
	 * close the semaphores.
	 */

	if (shmdt((char*)mesg_mem_shared_ptr) < 0)
		err_sys("client: can't detach shared memory");

	if (shmctl(shmid, IPC_RMID, (struct shmid_ds*)0) < 0)
		err_sys("client: can't remove shared memory");


	sem_close(clisem);	/* will remove the semaphore */
	sem_close(servsem);	/* will remove the semaphore */



	exit(0);

}