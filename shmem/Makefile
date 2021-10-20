#
CFLAGS = -O

MYLIB	= ../libnet.a

#
# Comunicacion cliente-servidor utilizando 1 buffer de memoria 
# compartida y semáforos.
#

all :		servidor cliente

servidor:	servidor.o $(MYLIB)
	cc $(CFLAGS) -o servidor servidor.o $(MYLIB)

cliente:	cliente.o $(MYLIB)
	cc $(CFLAGS) -o cliente cliente.o $(MYLIB)

clean:
	rm -f *.o cliente servidor

