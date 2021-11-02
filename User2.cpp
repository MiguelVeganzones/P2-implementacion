#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Bibliotecas creadas con las estructuras y las llaves
#include "msg.h"
#include "msgq.h"
#include "message.h"

//Definimos la instancia de la estructura creada en msg.h

void main()
{

	//Declaramos una variable para guardar el ID
	int id_cola2;

	//Conectamos a la cola
	if ((id_cola2 = msgget(MKEYQ2, 0)) < 0) { //cola bloqueante
		perror("El Usuario 2 no ha podido conectarse a la cola 2");
		exit(EXIT_FAILURE);
	}

	//Mostramos el PID del usuario 2 por qu elo va asolicitar el usuario 1
	printf("El pid del usuario 2 es: %d", getpid());

	// Esperaremos a que recibamos datos
	printf("\n\nEsperando...");
	user_read_queue_msg(id_cola2);

	//Finalizamos el programa user2
	exit(EXIT_SUCCESS);

}