#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include "message.h"
#include "msgq.h"

#define IPC_CREAT 1;


//Defining the instances of the created structured and the identifiers

shared_mem  *mesg_mem_shared_ptr;//ptr to message structure, which is in the shared memory segment 

int	shm_id, sem;	/* shared memory and semaphore IDs */
	
//Semaphore structure 
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

void set_free_sem(int num) {
	struct sembuf operasem;
	operasem.sem_num = num;
	operasem.sem_op = 1; //liberate the semaphore  
	operasem.sem_flg = 0;
	if (semop(sem, &operasem, 1) < 0) {
		perror("semop");
	}
}

void take_sem(int num) {
	struct sembuf operasem;
	operasem.sem_num = num;
	operasem.sem_op = -1; //take the semaphore
	operasem.sem_flg = 0;
	if (semop(sem, &operasem, 1) < 0) {
		perror("semop");
	}
}

//Declaramos una variable para guardar el ID de la cola
int id_cola1;

int main() {
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

	//Associate pointer from virtual memory to physical memory
	if ((mesg_mem_shared_ptr = (shared_mem*)shmat(shm_id, (char*)0, 0)) == (struct shared_mem*)-1) {
		perror("client: can't attach shared memory segment");
		exit(1);
	}

	//get semaphor ID
	if ((sem = semget(SEMKEY, 2, 0)) < 0) {
		perror("client: can't open client semaphore");
		exit(1);
	}

	EntityA();

	std::cout << "\n\nFin de la comunicacion\n";
	exit(EXIT_SUCCESS);
}

void EntityA()
{
	/* Mensaje de confirmación para que comprobar correcto funcionamiento */
	printf("\n\nLa Entidad A se encuentra preparada.");

	// Tenemos que recibir por cola del Usuario 1
	printf("\nEstado : En espera... ");
	auto messages = entity_read_queue_msg(id_cola1);
	printf("\nDatos recibidos de usuario 1.");
	printf("\n");

	//Escribimos en memoria compartida lo recibido por la interfaz
	printf("\nEscribiendo en memoria compartida...");

	std::vector<message> return_messages;

	for (int i = 0; i < messages.size() + 1; ++i)
	{
		take_sem(0);

		if (mesg_mem_shared_ptr->err_count >= n_retry) { //si ha habido demasiados errores se envia un mensaje de error al usuario
			entity_send_queue_msg(id_cola1, { message::error_message(mesg_mem_shared_ptr->_message) });
			exit(EXIT_FAILURE);
		}

		if (i != 0) { //not first iter
			auto read_m = entity_read_from_shared(mesg_mem_shared_ptr); 

			if (read_m.type == 1) {
				entity_send_to_shared(mesg_mem_shared_ptr, messages[i]);
				return_messages.push_back(read_m);
			}
			else if (read_m.type == 2) {
				entity_send_to_shared(mesg_mem_shared_ptr, messages[--i]);
			}
		}

		if (i != messages.size() ) { //not last iter as we added an extra one to read the response from entity B
			entity_send_to_shared(mesg_mem_shared_ptr, messages[i]);
		}

		set_free_sem(1);
	}

	printf("\nYa ha escrito en memoria compartida.");

	entity_send_queue_msg(id_cola1, return_messages);

	printf("\n\nLeyendo de la memoria y enviando por cola a Usuario 1....");

}