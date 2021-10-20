#
CFLAGS = -O

MYLIB	= ../libnet.a

#
# Comunicacion cliente-servidor utilizando 1 cola de mensajes
#

all : servidor cliente

servidor :	servidor.o msgq_func.o $(MYLIB)
	cc $(CFLAGS) -o servidor servidor.o msgq_func.o $(MYLIB)

cliente:	cliente.o msgq_func.o $(MYLIB)
	cc $(CFLAGS) -o cliente cliente.o msgq_func.o $(MYLIB)

clean:
	rm -f *.o cliente servidor

