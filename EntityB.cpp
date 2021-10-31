#pragma once
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/types.h>
#include	<sys/ipc.h>
#include	<sys/shm_id.h>
#include	"message.h"
#include	"msgq.h"

#include	<sys/sem.h>


// Variable para la memoria compartida
static int shm_id;
//Para los semaforos
static int sem2;

struct data_queue data_queue;
struct shared_mem	*Memory_ptr;		
/* ptr to message structure, which is
in the shared memory segment */

union semun {
	int val;
	struct semid_ds* buf;
	unsigned short int* array;
} semctl_arg;

//Funciones del semaforo
void set_free_sem(int num) {
	sembuf operasem;
	operasem.sem_num = num;
	operasem.sem_op = 1; //Libera el sem
	operasem.sem_flg = 0;
	if (semop(sem, &operasem, 1) < 0) {
		perror("semop");
	}
}

void take_sem(int num) {
	sembuf operasem;
	operasem.sem_num = num;
	operasem.sem_op = -1;   //Toma el sem
	operasem.sem_flg = 0;
	if (semop(sem, &operasem, 1) < 0) {
		perror("semop");
	}
}

// Declaramos una variable para guardar el ID de la cola
int id_cola2;

main()
{	
	//int id_shared;
	
	

	// Se crea la cola con el Usuario 2
	if ((id_cola2 = msgget(MKEYQ2, 060 | IPC_CREAT)) < 0) {
		perror("Servidor: No puedo obtener id de la cola");
		exit(1);
	}

	/*/if ((id_shared = SHMKEY) == (key_t)-1)
	{
		printf(" Se ha producido un error al crear la llave de la memoria compartida");
		exit(1);
	}
	*/
	

	if ((shm_id = shmget(SHMKEY, sizeof(shared_mem), PERMS | IPC_CREAT)) < 0)
		perror("shmget");

	// Puntero a la memoria
	if ((Memory_ptr = (shared_mem*)shmat(shm_id, (char*)0, 0)) == (shared_mem*)-1) {
		printf("No se puede asociar el puntero al segmento de memoria compartida");
		perror("shmat");
	}

	// Semaforos. Hay dos semaforos
	if ((sem2 = semget(SEMKEY, 2, PERMS | IPC_CREAT)) < 0) {
		perror("semget");
		exit(1);
	}

	// Inicializaci�n de los semaforos 
	semctl_arg.val = 1;
	if (semctl(sem, 0, SETVAL, semctl_arg) < 0) {
		perror("semctl");
		exit(1);
	}
	semctl_arg.val = 0;
	if (semctl(sem, 1, SETVAL, semctl_arg) < 0) {
		perror("semctl");
		exit(1);
	}

	while(1)
		entityB();

	exit(0);
}

void entityB()
{
	std::vector<message> message_queue;
	// Mensaje de confirmaci�n para que comprobar correcto funcionamiento 
	printf("\nLa Entidad B se encuentra preparada.");

	//Bloqueamos acceso a la memoria, para acceder nosotros
	take_sem(1);

	// Leemos de la memoria compartida lo que recibiremos de Entidad A
	printf("\n\nLeyendo de memoria compartida...");

	//leer memoria compartida

	message m = entity_read_from_shared(Memory_ptr);

	if(m.type != 0){return;} //reading its own message

	if (check_redundance(m))
	{
		m.type = 1; //ACK
		printf("\n\nEscribiendo en memoria compartida mesaje ACK");
		entity_send_to_shared(Memory_ptr, mutate(m));
		meessage_queue.push_back(m);
		//if last message
		if(m.lenght != data_size);
		printf("\nEnviando a Usuario 2...");
		entity_send_queue_msg(id_cola2, message_queue);
	}
	else{
		m.type = 2; //NAK
		printf("\n\nEscribiendo en memoria compartida mesaje NAK");
		entity_send_to_shared(Memory_ptr, m)
		return;
	}

	set_free_sem(0);
	
	printf("\nYa ha escrito en memoria compartida.");


	/*
	if (finish process) {
		sleep(2);
		printf("\nSaliendo del proceso...");
		// Libera memoria

		if (shmdt((char*)mesg_ptr) < 0) {
			perror("No se puede eliminar la memoria");
			exit(1);
		}

		// Se elimina memoria
		if (shmctl(shm_id, IPC_RMID, NULL) < 0) {
			perror("No se puede eliminar la memoria");
			exit(1);
		}

		/// Se elimina la cola utilizada (cola 2)
		if (msgctl(interfaz2, IPC_RMID, (struct msqid_ds*)NULL) < 0) {
			perror("Error");
			exit(1);
		}

		// Se eliminan semaforos
		if (semctl(sem, 2, IPC_RMID, semctl_arg) < 0) {
			printf("\nNo se ha podido borrar el semaforo\n");
			exit(1);
		}
		exit(0);
	}*/
}
