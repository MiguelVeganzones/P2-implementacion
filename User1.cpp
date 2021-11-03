#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "message.h"
#include "msgq.h"


void main(int comando, char* comando_p[])
{

	//Declaramos una variable para guardar el ID
	int id_cola1;

	// Primeramente hay que conectarse a la cola creada por la Entidad B
	if ((id_cola1 = msgget(MKEYQ1, 0)) < 0) { //cola bloqueante
		printf("El Usuario 1 no se ha podido conectar a la cola 1");
		exit(EXIT_FAILURE);
	}	

	//Escribimos y enviamos el mensaje a la cola de mensajes
	user_send_message(0, id_cola1, getpid());

	// Leemos mensaje de respuesta 
	printf("\nEsperando mensaje...");
	user_read_queue_msg(id_cola1);

	//Finalizacion del programa
	exit(EXIT_SUCCESS);
}


