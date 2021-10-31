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
	if ((interfaz2 = msgget(MKEYQ2, 0)) < 0) {
		perror("El Usuario 2 no ha podido conectarse a la cola 2");
		exit(EXIT_FAILURE);
	}

	//Mostramos el PID del usuario 2 por qu elo va asolicitar el usuario 1
	printf("El pid del usuario 2 es: %d", getpid());

	// Esperaremos a que recibamos datos
	printf("\n\nEsperando...");
	user_read_queue_msg(interfaz2);

	//Finalizamos el programa user2
	exit(EXIT_SUCCESS);



}