#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "message.h"
#include "msgq.h"


//We define the structure instance created in message.h

struct data_queue data_queue;

/*getpid devuelve el identificador de proceso del proceso actual. (Esto es usado normalmente
	   por rutinas que generan nombres únicos  de  ficheros  temporales.)   getppid  devuelve  el
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
	// Obtenemos el PID del Usuario 1, Usuario 2
	data_queue.origin = getpid();
	data_queue.destination = atoi(comando_p[1]);

	while (1) {
		// Ponemos el flag a 0 en cada iteracion 
		//mesg_cola.cola_final = 0;
		// Colocamos el sentido para enviar hacia la entidad
		data_queue.cola_sentido = 1L;

		//Para que el humano pueda introducir un texto
		puts("\nIntroduzca un texto para enviar: ");

		// Como no se consiguio programar la desfragmentación, si datos es mayor de 4096 caracteres se sale con error.
		if (fgets(data_queue.data, total_msg_size, stdin) == NULL)
		{
			printf("\nERROR");
			printf("\nEl maximo numero de caracteres es 4096");
			exit(1);
		}

		// Mostramos por pantalla lo que introduce el usuario para que sepa que el mensaje enviado era el que deseaba
		printf("\nCadena de caracteres enviada:\n");
		printf("\tDatos: %s\n", data_queue.data);

		//Mensajes de seguimiento y a su vez de envia el mensaje a la entidad A
		printf("\nEnviando mensaje por la cola...");
		msgsnd(interfaz1, &data_queue, sizeof(data_queue) - sizeof(long), 0);
		printf("\nEl mensaje se ha enviado por la cola!");

		//Para recibir el mensaje de nuevo

			printf("\nEsperando mensaje...");
			msgrcv(interfaz1, (data_queue*)&data_queue, sizeof(data_queue) - sizeof(long), (long)getpid(), 0);

			// Mostramos el mensaje que recibimos de Usuario 2
			printf("\n\nCadena de caracteres recibida:\n");
			printf("\tDatos: %s\n", data_queue.data);

			//Salimos
			exit(EXIT_SUCCESS);

		// Finalmente, se cambia el sentido de la cola, para la proxima iteración
		//data_queue.queue_direction = mesg_cola.cola_destino;
		//mesg_cola.cola_destino = mesg_cola.cola_origen;
		//mesg_cola.cola_origen = mesg_cola.cola_sentido;
	}
}


