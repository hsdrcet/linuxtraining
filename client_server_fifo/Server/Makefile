#! /bin/bash

server: server.o createInfra.o processRequest.o
	gcc -o server server.o createInfra.o processRequest.o

server.o: server.c ../headers.h ../shrdd.h
	gcc -c server.c

createInfra.o: createInfra.c ../headers.h ../shrdd.h
	gcc -c createInfra.c

processRequest.o: processRequest.c ../headers.h ../shrdd.h
	gcc -c processRequest.c

clean:
	rm *.o
