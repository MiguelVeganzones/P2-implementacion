#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/types.h>
#include	<sys/ipc.h>
#include	<sys/shm.h>

#include	<sys/sem.h>
#include	"shm.h"

int	shmid, clisem, servsem;	/* shared memory and semaphore IDs */
struct Memory_shared	*Memory_shared_ptr;		/* ptr to message structure, which is
				   in the shared memory segment */

main()
{
	/*
	 * Create the shared memory segment, if required,
	 * then attach it.
	 */

	if ( (shmid = shmget(SHMKEY, sizeof(struct Memory_shared), PERMS | IPC_CREAT)) < 0)
		err_sys("server: can't get shared memory");
	if ( (Memory_shared_ptr = (struct Memory_shared *) shmat(shmid, (char *) 0, 0)) == (struct Memory_shared *) -1)
		err_sys("server: can't attach shared memory");

	/*
	 * Create two semaphores.  The client semaphore starts out at 1
	 * since the client process starts things going.
	 */

	if ( (clisem = sem_create(SEMKEY1, 1)) < 0)
		err_sys("server: can't create client semaphore");
	if ( (servsem = sem_create(SEMKEY2, 0)) < 0)
		err_sys("server: can't create server semaphore");

	server();

	/*
	 * Detach the shared memory segment and close the semaphores.
	 * The client is the last one to use the shared memory, so
	 * it'll remove it when it's done.
	 */

	if (shmdt((char *) Memory_shared_ptr) < 0)
		err_sys("server: can't detach shared memory");

	sem_close(clisem);
	sem_close(servsem);

	exit(0);
}
