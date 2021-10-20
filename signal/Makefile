#
CFLAGS = -O

MYLIB	= ../libnet.a

#
# Comunicacion cliente-servidor utilizando 1 buffer de memoria 
# compartida y semáforos.
# Empleo de señales entre procesos.
#

all :		servidor cliente

servidor:	servidor.o $(MYLIB)
	cc $(CFLAGS) -o servidor servidor.o $(MYLIB)

cliente:	cliente.o $(MYLIB)
	cc $(CFLAGS) -o cliente cliente.o $(MYLIB)

cliente.o : cliente.c shm.h mesg.h 

servidor.o : servidor.c shm.h mesg.h 

clean:
	rm -f *.o cliente servidor

