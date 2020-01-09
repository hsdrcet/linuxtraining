#include "../shrdd.h"
#include "../headers.h"

Infra* createInfra(void)
{
	int i, pret, mret, aret;
	Infra *infra;
	
	printf("File:%s; Func:%s BEGIN\n", __FILE__, __func__);

	//************************* Allocate memory ***********************
	
	// allocate mm for infra
	infra = (Infra*)malloc((sizeof(Infra)));
	if (!infra)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	
	// Memory for TWO pipes
	infra->pipes = (int*)malloc(sizeof(int)*4);
	if (!infra->pipes)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	// MM for fifo file name
	infra->fifoName = (char*)malloc(sizeof(char)*20);
	if (!infra->fifoName)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	// Fill with zero to avoid garbbage; always fill string 
	memset(infra->fifoName, '\0', 20);
	
	//************************* Create resources ***********************
	
	// Create TWO pipes
	for (i=0; i<4;i+=2)
	{
		pret = pipe(infra->pipes+i);
		if (pret == -1)
		{
			perror("pipe");
			free(infra->pipes);
			exit(EXIT_FAILURE);
		}
	}
	
	// Copy fifo name in infra
	strncpy(infra->fifoName, SERVERFIFO, 20);
	
	// Create server fifo if NOT exists
	aret = access(infra->fifoName, F_OK);
	if (aret == -1)
	{
		mret = mkfifo(infra->fifoName, 0666);
		if (mret == -1)
		{
			perror("mkfifo");
			exit(EXIT_FAILURE);
		}
	}
	printf("File:%s; Create server fifo OK\n", __FILE__);
	

	printf("File:%s; Func:%s END\n", __FILE__, __func__);	
	return infra;
}
 
