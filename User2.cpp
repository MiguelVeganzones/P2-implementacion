#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Bibliotecas creadas con las estructuras y las llaves
#include "msg.h"
#include "msgq.h"

//Definimos la instancia de la estructura creada en msg.h
struct data_queue data_queue;

void main()
{

	//Declaramos una variable para guardar el ID
	int interfaz2;

	//Conectamos a la cola
	if ((interfaz2 = msgget(567L, 0)) < 0) {
		perror("El Usuario 2 no ha podido conectarse a la cola 2");
		exit(EXIT_FAILURE);
	}

	// Sentido Entidad 2 -> Usuario 2
	//data_queue.queue_direction = getpid();

	//Mostramos el pid servidos para que sepamos a que servidor conectarnos
	printf("El pid del usuario 2 es: %d", getpid());

	while (1) {
		// Esperaremos a que recibamos datos
		printf("\n\nEsperando...");
		msgrcv(interfaz2, (data_queue*)&data_queue, sizeof(data_queue) - sizeof(long), (long)getpid(), 0);
		printf("\n\nDatos recibidos de entidad B.");

		//Tras recibir, invertimos la cola para enviar
		data_queue.queue_direction = data_queue.origin;
		data_queue.origin = data_queue.destination;
		data_queue.destination = data_queue.queue_direction;
		data_queue.queue_direction = 2L;

		// Enviaremos los datos y mostramos en que modo estamos
		msgsnd(interfaz2, (data_queue*)&data_queue, sizeof(data_queue) - sizeof(long), 0);
		printf("\nDatos enviados a la entidad B.");

		//Ponemos el flag a 1
		//mesg_cola.cola_final = 1;
		//strcpy(mesg_cola.cola_datos, "");
		//msgsnd(interfaz2, (mesg_Cola*)&mesg_cola, sizeof(mesg_Cola) - sizeof(long), 0);

	}
}