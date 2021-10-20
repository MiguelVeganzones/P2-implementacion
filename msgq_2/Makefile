#
CFLAGS = -O

MYLIB	= ../libnet.a

#
# Comunicacion cliente-servidor utilizando 2 colas de mensajes
#

all :		servidor cliente

servidor:	servidor.o func_ser.o msgq_func.o $(MYLIB)
	cc $(CFLAGS) -o servidor servidor.o func_ser.o msgq_func.o $(MYLIB)

cliente:	cliente.o func_cli.o msgq_func.o $(MYLIB)
	cc $(CFLAGS) -o cliente cliente.o func_cli.o msgq_func.o $(MYLIB)

clean:
	rm -f *.o cliente servidor

