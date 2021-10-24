/*
In this file, IPC´s keys and permissions are defined

We had used the keys and permissions given in CV examples.

Authors: David Redondo and Miguel Veganzones
*/

#include <sys/ipc.h>
#include <sys/msg.h>


#define PERMS 0666  				// Permissions

#define SHMKEY ftok("./msgq.h", 'a') 		// Key for shared memory
#define SEMKEY ftok("./msgq.h", 'b') 		// key for the semaphore
#define MKEYQ1 ftok("./msgq.h", 'c')  		// Key for queue 1 
#define MKEYQ2 ftok("./msgq.h", 'd')  		// Key for queue 2 