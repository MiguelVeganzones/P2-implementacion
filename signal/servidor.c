
#include	"shm.h"

static void sig_usr();	/* handler para signals */

int	shmid, clisem, servsem;	/* IDs de shm y semaforos */
Mesg	*mesgptr;		/* ptr a la estructura del mensaje, que esta
				   en el segmento de memoria compartida */

static jmp_buf env_usr1;

main()
{

	/*
	 * Crea el segmento de memoria compartica, si no existe,
	 * y se conecta a el.
	 */

	if ( (shmid = shmget(SHMKEY, sizeof(Mesg), PERMS | IPC_CREAT)) < 0)
		err_sys("servidor: no puedo obtener(crear) shm");
	if ( (mesgptr = (Mesg *) shmat(shmid, (char *) 0, 0)) == (Mesg *) -1)
		err_sys("servidor: no puedo conectarme a shm");

	mesgptr->pid_server = getpid();

	/*
	 * Crea dos semaforos. El semaforo del cliente se inicializa a 1
	 * dado que sera el proceso cliente el que inicia la sesion
	 */

	if ( (clisem = sem_create(SEMKEY1, 1)) < 0)
		err_sys("servidor: no puedo crear el semaforo del cliente");
	if ( (servsem = sem_create(SEMKEY2, 0)) < 0)
		err_sys("servidor: no puedo crear el semaforo del servidor");

	if((void *) sysv_signal(SIGUSR1, sig_usr) == SIG_ERR){
		fprintf(stderr, "NO puedo coger SIGUSR1\n");
		exit(1);
	}

	while(server());

	/*
	 * Desconexion y eliminacion del segmento de memoria compartida
	 * y cierre de los semaforos
	 */

	if (shmdt((char *) mesgptr) < 0)
		err_sys("servidor: no puedo desconectarme de shm");


	if (shmctl(shmid, IPC_RMID, (struct shmid_ds *) 0) < 0)
		err_sys("servidor: no puedo eliminar shm");


	sem_close(clisem);
	sem_close(servsem);

	exit(0);
}

int server()
{
	int	n;
	unsigned int suma = 1;

	/*
	 * Espera peticion del cliente.
	 */
	sem_wait(servsem);

	if( (n = setjmp(env_usr1)) == 0){

		mesgptr->dato1 += mesgptr->dato2;
		suma = (unsigned int) mesgptr->dato1;

		if(suma)
			getchar();

		sem_signal(clisem);
	}else{
		printf("\t\tSERVIDOR : setjmp = %d\n", n);
	}

	return (suma);
}

static void sig_usr(signo)
int signo;
{
	if(signo == SIGUSR1){
		fprintf(stderr, "he recibido la senal SIGUSR1\n");
		if((void *) sysv_signal(SIGUSR1, sig_usr) == SIG_ERR){
			fprintf(stderr, "NO puedo coger SIGUSR1\n");
			exit(1);
		}

		longjmp(env_usr1, 2);
	}
	else{
		fprintf(stderr, "he recibido la senal %d\n", signo);
	}

	return;
}


