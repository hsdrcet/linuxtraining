#ifndef SERVERFIFO
#define SERVERFIFO "serverFifo"
#endif

#ifndef CLIENTFIFO
#define CLIENTFIFO "cli.%d.Fifo"
#endif

// Structure of Client request
typedef struct
{
	int oprnd1;
	int oprnd2;
	char oprtr;
	int pid;
}Request;

// Result back to Client
typedef struct
{
	int pid;
	float result;
}Result;

// Server preparations
typedef struct
{
	int *pipes;
	char *fifoName;
}Infra;

Infra* createInfra(void);
int processRequest(Infra*);
void PRINT(void);
