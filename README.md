Join proyect between David Redondo and Miguel Veganzones

In this practice it's presented a inter process communication between users in a Linux machine.
To carry out this communication, the user process must use the necessary communication services needed that are provided 
by the architecture's entities.

Between entities A and B is stablished a protocol that will let they can interact exchanging messages (PDU's).
Likewise, these entities will use interfaz functions with the lower level, communication servicies between processes
(IPC) provided by the OS. These services are invoked through call to system which let inter processes communication.

-----------------------------------------------------------------------------------------------------------------------------

Exceution rules for correct client-server communcation.

First one, run Makefile. Then we are going to execute the rest of files in this order: 

First EntidadB -> command to run the file: ./entityB
Second EntityA -> command to run the file: ./entityA
Third User2  -> command to run the file: ./user2
Fourth User1 -> command to run the file: ./user1 

The user 1 is who will let write the character chain, directories,...
The rest work internally. They will only show some dates on the screen to know how they are working



