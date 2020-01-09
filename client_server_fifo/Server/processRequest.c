#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "../shrdd.h"
#include "../headers.h"
int processRequest(Infra *infra)
{
	int rfd, rret, fret, wret, count, wfd, rmkfifo;
	char p0rfd[4], p1wfd[4];

	Request req;
	Result res;

	// open server fifo
	if ((access(infra->fifoName, F_OK)) == -1)
	{
		perror("access");
		exit(1);
	}
	else
	{
		printf("File: %s; Opening server Fifo file for reading client request.....\n", __FILE__);
		// Server will block on open here, until a client open serverFifo's other END and write request
		rfd = open(infra->fifoName, O_RDONLY);
		if (rfd == -1)
		{
			perror("open");
			exit(1);
		}
		printf("File: %s; Server fifo file opened for reading client request....\n", __FILE__);
	}

	// Server will read client request 
	rret = read(rfd, &req, sizeof(Request));
	if (rret == -1)
	{
		perror("read");
		exit(1);
	}
	printf("File: %s; Request recieved from client PID => %d\n",__FILE__, req.pid);

	// write recived data to pipe1 of vendor
	wret = write(*(infra->pipes+1), &req, sizeof(Request));
	if (wret == -1)
	{
		free(infra->pipes);
		perror("write");
		exit(EXIT_FAILURE);
	}
	printf("File: %s; Request sent to Vendor...\n", __FILE__);
	close(rfd);

	// invoke vendor
	fret = fork();
	switch (fret)
	{
		case -1:
			perror("fork");
			exit(EXIT_FAILURE);
		case 0: 
			sprintf(p0rfd, "%d", *(infra->pipes+0)); // Vendor will read from pipe0
			sprintf(p1wfd, "%d", *(infra->pipes+3)); // Vendor will write result to pipe1
			execl("./vn1", "vn1", p0rfd, p1wfd, NULL);
			printf("File: %s; Error in execl \n", __FILE__);
		default:
			// Read calculated result in server; Server will Block on read here 
			count = read(*(infra->pipes+2) , &res, sizeof(Result));
			if (count == -1)
			{
				free(infra->pipes);
				perror("read");
				exit(EXIT_FAILURE);
			}
			printf("File: %s; Result from Vendor in server ........%f\n", __FILE__, res.result);							

			//*********************** Write this result in client fifo ********************
			
			// Prepare fifo name
		      char *clientFifoName = (char*)malloc(sizeof(char)*50);
		      if (!clientFifoName)
		      {
	               perror("malloc");
        	       exit(EXIT_FAILURE);
		      }	

                        // Fill with zero to avoid garbbage; always fill string
                        memset(clientFifoName, '\0', 20);

			sprintf(clientFifoName, CLIENTFIFO, req.pid);
			 
			printf("File: %s; Client fifoName in server => %s\n",__FILE__,  clientFifoName);

			// Check client fifo avilabilty 
			if ((access(clientFifoName, F_OK)) == -1)
			{
				rmkfifo = mkfifo(clientFifoName, 0666);
				if(rmkfifo == -1)
				{
					perror("mkfifo");
					if (errno != EEXIST)
						exit(1);
				}
				printf("FILE: %s; Client fifo created by SERVER...\n", __FILE__);
			}
			else
			{
				printf("FILE: %s; Client fifo %s exists...\n", __FILE__, clientFifoName);
			}
	
			// Send result to requesting client
			printf("FILE: %s; Before opening Client fifo...\n", __FILE__);
			wfd = open(clientFifoName, O_WRONLY);
			if (wfd == -1)
			{
				perror("open");
				exit(1);
			}
			 printf("File: %s; Client %s's fifo opened for writting...\n", __FILE__, clientFifoName);

			printf("File: %s; Writting result in Client %d...\n", __FILE__, res.pid);
			count = write(wfd, &res, sizeof(Result));
			if (count == -1)
			{
				free(infra->pipes);
				perror("write");
				exit(EXIT_FAILURE);
			}
			close(wfd);
			printf("File: %s; Data from server written in Client....\n", __FILE__);
	}
}
