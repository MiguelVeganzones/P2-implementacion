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
static int sem;

struct shared_mem	*mesg_mem_shared_ptr;		
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

	// Se crea la cola con el Usuario 2
	if ((id_cola2 = msgget(MKEYQ2, 060 | IPC_CREAT)) < 0) {
		perror("Servidor: No puedo obtener id de la cola");
		exit(1);
	}

	//crear la memoria compartida
	if ((shm_id = shmget(SHMKEY, sizeof(shared_mem), PERMS | IPC_CREAT)) < 0)
		perror("shmget");

	// Asociar el puntero a la memoria compartida
	if ((mesg_mem_shared_ptr = (shared_mem*)shmat(shm_id, (char*)0, 0)) == (shared_mem*)-1) {
		printf("No se puede asociar el puntero al segmento de memoria compartida");
		perror("shmat");
	}

	// Semaforos. Hay dos semaforos
	if ((sem = semget(SEMKEY, 2, PERMS | IPC_CREAT)) < 0) {
		perror("semget");
		exit(1);
	}

	// Inicializacion de los semaforos 
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

	entityB();

	std::cout << "\n\nFin de la comunicacón\n";
	exit(0);
}

void entityB()
{
	// Mensaje de confirmaci�n para que comprobar correcto funcionamiento 
	printf("\nLa Entidad B se encuentra preparada.");

	//vector de mensajes para mandar a la colad e mensajes
	std::vector<message> messages;

	while (1) {
		//Bloqueamos acceso a la memoria, para acceder nosotros
		take_sem(1);
		message m = entity_read_from_shared(mesg_mem_shared_ptr);

		if (m.type != 0) { std::cout << "Entidad B ha leido su propio mensaje\n";  exit(EXIT_FAILURE); } //reading its own message
		if (check_redundance(m)) {
			auto mutated = mutate(m);
			mutated.type = 1;
			messages.push_back(mutated);

			//ack
			entity_send_to_shared(mesg_mem_shared_ptr, mutated);
		}
		else { //nak
			m.type = 2;
			entity_send_to_shared(mesg_mem_shared_ptr, m);
		}

		// Leemos de la memoria compartida lo que recibiremos de Entidad A
		printf("\n\nLeyendo de memoria compartida...");
		set_free_sem(1);

		if (m.length != data_size) { break; }
	}

	entity_send_queue_msg(id_cola2, messages);
}
