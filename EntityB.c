#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/types.h>
#include	<sys/ipc.h>
#include	<sys/shm.h>

#include	<sys/sem.h>

#include	"shm.h"


// Variable para la memoria compartida
	int shm;
//Para los semaforos
	int sem, sem2;

struct data_queue data_queue;
struct shared_mem	*Memory_ptr;		
/* ptr to message structure, which is
in the shared memory segment */



union semun {
	int val;
	struct semid_ds* buf;
	ushort* array;
} semctl_arg;

//Funciones del semaforo
void set_free_sem(int num) {
	struct sembuf operasem;
	operasem.sem_num = num;
	operasem.sem_op = 1; //Libera el sem
	operasem.sem_flg = 0;
	if (semop(sem, &operasem, 1) < 0) {
		perror("semop");
	}
}

void take_sem(int num) {
	truct sembuf operasem;
	operasem.sem_num = num;
	operasem.sem_op = -1;   //Toma el sem
	operasem.sem_flg = 0;
	if (semop(sem, &operasem, 1) < 0) {
		perror("semop");
	}
}



main()
{

	// Declaramos una variable para guardar el ID de la cola
	int interfaz2;
	
	//int id_shared;
	
	

	// Se crea la cola con el Usuario 2
	if ((interfaz2 = msgget(MKEYQ2, 060 | IPC_CREAT)) < 0) {
		perror("Servidor: No puedo obtener id de la cola");
		exit(1);
	}

	/*/if ((id_shared = SHMKEY) == (key_t)-1)
	{
		printf(" Se ha producido un error al crear la llave de la memoria compartida");
		exit(1);
	}
	*/
	

	if ((shm = shmget(SHMKEY, sizeof(shared_mem), PERMS | IPC_CREAT)) < 0)
		perror("shmget");

	// Puntero a la memoria
	if ((Memory_ptr = (shared_mem*)shmat(shm, (char*)0, 0)) == (shared_mem*)-1) {
		printf("No se puede asociar el puntero al segmento de memoria compartida");
		perror("shmat");
	}

	// Semaforos. Hay dos semaforos
	if ((sem = semget(SEMKEY, 2, PERMS | IPC_CREAT)) < 0) {
		perror("semget");
		exit(1);
	}

	// Inicialización de los semaforos 
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

	exit(0);
}

void entityB()
{
	// Mensaje de confirmación para que comprobar correcto funcionamiento 
	printf("\nLa Entidad B se encuentra preparada.");

	//Bloqueamos acceso a la memoria, para acceder nosotros
	take_sem(1);

	// Leemos de la memoria compartida lo que recibiremos de Entidad A
	printf("\n\nLeyendo de memoria compartida...");

	//leer memoria compartida

	message m = entity_read_from_shared(Memory_ptr);
	if (check_redundance(m))
	{

	}

	//Bloqueamos acceso a la memoria, para acceder nosotros
	take_sem(1);

	// Leemos de la memoria compartida lo que recibiremos de Entidad A
	printf("\n\nLeyendo de memoria compartida...");

	//Aqui habría que comprobar que el mensaje es correcto!!!

	/*
	mesg_cola.cola_sentido = mesg_ptr->memo_destino;
	mesg_cola.cola_origen = mesg_ptr->memo_origen;
	mesg_cola.cola_destino = mesg_ptr->memo_destino;
	mesg_cola.cola_cliente = mesg_ptr->memo_cliente;
	strcpy(mesg_cola.cola_datos, mesg_ptr->memo_datos);
	strcpy(mesg_cola.cola_comando, mesg_ptr->memo_comando);*/
	printf("\nYa ha leido de memoria compartida.");
	

	//En caso de que el mensaje sea correcto:
	//Enviamos los datos leidos por interfaz al Usuario 2
	printf("\nEnviando a Usuario 2...");
	msgsnd(interfaz2, (shared_mem*)&shared_mem, sizeof(shared_mem) - sizeof(long), 0);


	
	//Escribimos en memo comp lo recibido por la interfaz
	printf("\n\nEscribiendo en memoria compartida...");

	/*
	printf("\n\nEscribiendo en memoria compartida...");
	mesg_ptr->memo_origen = mesg_cola.cola_origen;
	mesg_ptr->memo_destino = mesg_cola.cola_destino;
	strcpy(mesg_ptr->memo_datos, mesg_cola.cola_datos);
	set_free_sem(0);
	*/
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
		if (shmctl(shm, IPC_RMID, NULL) < 0) {
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
