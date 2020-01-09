#! /bin/bash

client: rc1.o
	gcc -o rc1 rc1.o

rc1.o: rc1.c ../headers.h ../shrdd.h
	gcc -c rc1.c

clean:
	rm *.o
