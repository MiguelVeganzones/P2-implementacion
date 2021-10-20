
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <setjmp.h>
#include <signal.h>

	/* estructura de la memoria compartida */

typedef struct {
  int 	dato1;
  int 	dato2;
  int 	pid_server;
} Mesg;

