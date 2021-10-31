#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "message.h"
#include "msgq.h"


//We define the structure instance created in message.h

struct data_queue data_queue;

/*getpid devuelve el identificador de proceso del proceso actual. (Esto es usado normalmente
	   por rutinas que generan nombres únicos de  ficheros  temporales.)   getppid  devuelve  el
	   identificador de proceso del padre del proceso actual.
*/

void main(int comando, char* comando_p[])
{

	//Declaramos una variable para guardar el ID
	int interfaz1;

	// Primeramente hay que conectarse a la cola creada por la Entidad B
	if ((interfaz1 = msgget(MKEYQ1, 0)) < 0) {
		printf("El Usuario 1 no se ha podido conectar a la cola 1");
		exit(EXIT_FAILURE);
	}	
	
	//Determinamos el PID del usuario de origen
	data_queue.origin = getpid();
	//Escribimos y enviamos el mensaje a la cola de mensajes
	user_send_message(0, interfaz1, data_queue.origin);

	// Leemos mensaje de respuesta 
	printf("\nEsperando mensaje...");
	user_read_queue_msg(interfaz1);


}


