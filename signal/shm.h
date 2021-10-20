#include	"mesg.h"

#define	NBUFF	4	/* number of buffers in shared memory */
			/* (for multiple buffer version) */

#define	SHMKEY	((key_t) ftok("./shm.h", 'a')) /* base value for shmem key */

#define	SEMKEY1	((key_t) ftok("./shm.h", 'b')) /* client semaphore key */
#define	SEMKEY2	((key_t) ftok("./shm.h", 'c')) /* server semaphore key */

#define	PERMS	0600
