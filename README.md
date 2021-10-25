
HAY QUE TRADUCIR TODO ESTO AL INGLES!!!!

Trabajo conjunto entre David Redondo y Miguel Veganzones.

En esta práctica se plantea reaizar la comunicación entre procesos de usuario, en una máquina Linux.
Para poder llevar a cabo la comunicación, los procesos de usuario han de emplear los servicios de comunicación
necesarios que les proporciones entidades de la arquitectura.

Entre las entidades A y B se establece un protocolo que permitirá que interactuén intercambiando
mensajes (PDUs). Asimismo, estas entidades harán uso de una serie de funciones de interfaz con el nivel inferior,
servicios de comunicación entre procesos (IPC) que proporciona el sistema operativo. Estos servicios se invocan
mediante llamadas al sistema que permiten la comunicación entre procesos.

-----------------------------------------------------------------------------------------------------------------------------

Exceution rules for correct client-server communcation.

First one, run Makefile. Then we are going to execute the rest of files in this order: 

First EntidadB -> command to run the file: ./entityB
Second EntityA -> command to run the file: ./entityA
Third User2  -> command to run the file: ./user2
Fourth User1 -> command to run the file: ./user1 PID_user2 m [TF]

The user 1 is who will let write the character chain, directories,...
The rest work internally. They will only show some dates on the screen to know how they are working



