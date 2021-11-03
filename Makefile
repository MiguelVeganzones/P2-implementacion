all: enttityA entityB user1 user2


entityA: entityA.c message.h msgq.h
	cc -o entityA entityA.cpp
entityB: entityB.c message.h msgq.h
	cc -o entityB entityB.cpp
user1: user1.c message.h msgq.h
	cc -o user1 user1.cpp
user2: user2.c message.h msgq.h
	cc -o user2 user2.cpp

clean:
	rm entity1 entityB user1 user2 *.o
