
#include	"shm.h"

static void sig_alrm();	/* handler para signals */

int	shmid, clisem, servsem;	/* shared memory and semaphore IDs */
Mesg	*mesgptr;		/* ptr to message structure, which is
				   in the shared memory segment */

static jmp_buf env_alrm;
pid_t  pid_server;
int    timeout = 2;

main()
{
	int i=1;


	/*
	 * Obtiene el segmento de memoria compartida y se conecta a el.
	 * El servidor debe haberlo creado previamente.
	 */

	if ( (shmid = shmget(SHMKEY, sizeof(Mesg), 0)) < 0)
		err_sys("cliente: no puedo obtener shm");
	if ( (mesgptr = (Mesg *) shmat(shmid, (char *) 0, 0)) == (Mesg *) -1)
		err_sys("cliente: no puedo conectarme con shm");

	/*
	 * Abre dos semaforos creados previamente por el servidor.
	 */

	if ( (clisem = sem_open(SEMKEY1)) < 0)
		err_sys("cliente: no puedo abrir el semaforo del cliente");
	if ( (servsem = sem_open(SEMKEY2)) < 0)
		err_sys("cliente: no puedo abrir el semaforo del servidor");

	/*
	 * Identificador del proceso servidor.
	 */
	pid_server = mesgptr->pid_server;

	printf("%d)\t", i++);	client(1, 2);
	printf("%d)\t", i++);	client(10, 20);
	printf("%d)\t", i++);	client(2, 3);
	printf("%d)\t", i++);	client(2, 7);
	printf("%d)\t", i++);	client(4, 5);
	printf("%d)\t", i++);	client(8, 5);
	printf("%d)\t", i++);	client(4, 2);
	printf("%d)\t", i++);	client(9, -2);
	printf("%d)\t", i++);	client(7, 7);
	printf("%d)\t", i++);	client(1, 2);
	printf("%d)\t", i++);	client(1, 2);
	printf("%d)\t", i++);	client(0, 0);

	/*
	 * Desconexion del shm y cierre de los semaforos.
	 */
	if (shmdt((char *) mesgptr) < 0)
		err_sys("cliente: no puedo desconectarme del shm");

	sem_close(clisem);
	sem_close(servsem);

	exit(0);
}

client(a, b)
int a, b;
{
	int	n;

	/*
	 * Prepara peticion.
	 */

	sem_wait(clisem);		/* espera turno para hacer peticion */



	/*
	 * Escribe la peticion
	 */
	mesgptr->dato1 = a;
	mesgptr->dato2 = b;

	printf("\t%d + %d = ", a, b);	fflush(stdout);

	sem_signal(servsem);		/* despierta al servidor */



	/*
	 * Espera la respuesta del servidor
	 */
	if((void *) sysv_signal(SIGALRM, sig_alrm) == SIG_ERR){
		fprintf(stderr, "NO puedo coger SIGALRM\n");
		exit(1);
	}

	if( (n = setjmp(env_alrm)) == 0){
		alarm(timeout);
		sem_wait(clisem);	/* espera respuesta del servidor*/
		alarm(0);

		/*
		 * Muestra la respuesta
		 */
		printf("%d\t", mesgptr->dato1);	fflush(stdout);
	}else{
		printf("Envio la señal SIGUSR1 al servidor\t"); fflush(stdout);
		kill(pid_server, SIGUSR1);
	}

	printf("setjmp = %d\n", n);

	sem_signal(clisem);
}

static void sig_alrm(signo)
int signo;
{
	printf("\nrecibida alarma!!\t");	fflush(stdout);

	longjmp(env_alrm, 1);

	return;
}

