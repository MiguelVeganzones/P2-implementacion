#include	<sys/types.h>
#include	<sys/ipc.h>
#include	<sys/msg.h>

#include	<sys/errno.h>
extern int	errno;

#define	MKEY1	ftok("./msgq.h", 'a')
#define	MKEY2	ftok("./msgq.h", 'b')

#define	PERMS	0666
