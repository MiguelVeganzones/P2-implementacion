#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include <sys/ipc.h>
//#include <sys/shm.h>
//#include <sys/sem.h>

#include "message.h"
#include "msgq.h"

#define IPC_CREAT 1;


//Defining the instances of the created structured and the identifiers

//struct Messages_queue* mesg_queue_ptr; 

shared_mem  *mesg_mem_shared_ptr;//ptr to message structure, which is in the shared memory segment 

int	shm_id, sem1, sem2;	/* shared memory and semaphore IDs */
	
//Semaphore structure (explain what means each line)

union semun {
	int val;
	struct semid_ds* buf;
	unsigned short* array;
} semctl_arg;

//Semaphore functions

struct sembuf {
	int sem_num;
	int sem_op;
	int sem_flg;

};

int semop(int, void*, int){}
int msgget(int, int) {}
int shmget(int, size_t, long) {}

void set_free_sem(int num) {
	struct sembuf operasem;
	operasem.sem_num = num;
	operasem.sem_op = 1; //liberate the semaphore  
	operasem.sem_flg = 0;
	if (semop(sem1, &operasem, 1) < 0) {
		
	}
}

void take_sem(int num) {
	struct sembuf operasem;
	operasem.sem_num = num;
	operasem.sem_op = -1; //take the semaphore
	operasem.sem_flg = 0;
	if (semop(sem1, &operasem, 1) < 0) {
		
	}
}

int main() {

	//Declaramos una variable para guardar el ID de la cola
	int id_cola1;

	// Making messages queue for user1
	if ((id_cola1 = msgget(MKEYQ1, PERMS | IPC_CREAT)) < 0) {
        perror("EntityA: can't make messages queue");
		exit(1);
	}
		
	//Get the shared memory segment and attach it.
	//The server must have already created it.
	if ((shm_id = shmget(SHMKEY, sizeof(shared_mem), 0)) < 0) {
		perror("EntityA: can't get shared memory segment");
		exit(1);
	}

	//Pointer to memory
	if ((mesg_mem_shared_ptr = (shared_mem*)shmat(shm_id, (char*)0, 0)) == (struct shared_mem*)-1) {
		perror("client: can't attach shared memory segment");
		exit(1);
	}

	//Semaforos
	if ((sem1 = semget(SHMKEY, 2, 0)) < 0) {
		perror("client: can't open client semaphore");
		exit(1);
	}
	EntityA();
	EntityA();
}

void EntityA()
{
	/* Mensaje de confirmación para que comprobar correcto funcionamiento */
	printf("\n\nLa Entidad A se encuentra preparada.");

	// Tenemos que recibir por cola del Usuario 1
	printf("\nEstado : En espera... ");

	if ((msgrcv(id_cola1, &data_queue, sizeof(data_queue) - sizeof(long), 1L, 0)) < 0)
	{
		perror("msgrcv");
		printf("Error al recibir por la cola");
		exit(1);
	}

	printf("\nDatos recibidos de usuario 1.");
	printf("\n");

	//Escribimos en memoria compartida lo recibido por la interfaz
	printf("\nEscribiendo en memoria compartida...");

	//HAY QUE CAMBIAR ESTO!!!!
	mesg_ptr->memo_origen = mesg_cola.cola_origen;
	mesg_ptr->memo_destino = mesg_cola.cola_destino;
	mesg_ptr->memo_cliente = mesg_cola.cola_cliente;
	strcpy(mesg_ptr->memo_datos, mesg_cola.cola_datos);
	printf("\nYa ha escrito en memoria compartida.");


	// Se libera el acceso a la memoria
	set_free_sem(1);

	printf("\n\nLeyendo de la memoria y enviando por cola a Usuario 1....");

	// Lectura de memoria la memoria compartida
	while (mesg_ptr->memo_final != 1) {

		// Bloqueamos acceso a la memoria, para acceder nosotros
		take_sem(0);

		// Comprueba el patron y si no es correcto, da error (sale)
		int patron = mesg_ptr->memo_patron;
		// Patron que hemos seleccionado 10101010
		if (patron != 170) {
			printf("Error: el patron no es correcto\n");
			exit(EXIT_FAILURE);
		}

		mesg_cola.cola_origen = mesg_ptr->memo_origen;
		mesg_cola.cola_destino = mesg_ptr->memo_destino;
		mesg_cola.cola_sentido = mesg_ptr->memo_destino;
		strcpy(mesg_cola.cola_datos, mesg_ptr->memo_datos);

		// Enviamos por interfaz, la lectura de la memoria

		msgsnd(id_cola1, &data_queue, sizeof(data_queue) - sizeof(long), 0);


		if (mesg_ptr->memo_final != 1) {
			set_free_sem(1);
		}

	}
	printf("\nEnviado por la cola a Usuario 1.");

}