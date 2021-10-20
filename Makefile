
#
CFLAGS = -O
#

all : libnet.a msgq1 msgq2 shm sign 

#-----------------------------------------------------------------------

libnet.a : semaph.o error.o 
	ar r libnet.a semaph.o error.o

semaph.o :	semaph.c
	cc $(CFLAGS) -c semaph.c

error.o :	error.c 
	cc $(CFLAGS) -c error.c

#-----------------------------------------------------------------------

msgq1 : libnet.a msgq_1
	( cd msgq_1 ; make)

#-----------------------------------------------------------------------

msgq2 : libnet.a msgq_2
	( cd msgq_2 ; make)

#-----------------------------------------------------------------------

shm : libnet.a shmem
	( cd shmem ; make)

#-----------------------------------------------------------------------

sign : libnet.a signal
	( cd signal; make)

#-----------------------------------------------------------------------

clean:
	rm *.a *.o
	( cd msgq_1 ; make clean)
	( cd msgq_2 ; make clean)
	( cd shmem ;  make clean)
	( cd signal;  make clean)

