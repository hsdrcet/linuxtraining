#include "../headers.h"
#include "../shrdd.h"
int main(int argc, char **argv)
{
	printf("File: %s; Func: %s BEGIN \n", __FILE__, __func__);

	Request req;
	Result res;
	int pid, wfd, rfd, rret, wret, mret;

	// 1. get pid of client
	pid = getpid();
	if (pid == -1)
	{
		perror("getpid");
		exit(EXIT_FAILURE);
	}	
	printf("FILE: %s; pid of Client is => %d\n", __FILE__, pid);

	// prepare client request
	req.oprnd1 = 5;
	req.oprnd2 = 5;
	req.oprtr = '+';
	req.pid = pid;

	// Open server fifo and Send write request to server
	printf("FILE: %s; Opening SERVER FIFO for writting request.....\n", __FILE__);
	wfd = open(SERVERFIFO, O_WRONLY);
	if (wfd == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	printf("FILE: %s; SERVER FIFO File opened for writting request..\n", __FILE__);

	wret = write(wfd, &req, sizeof(Request));
	if (wret == -1)
	{
		perror("write");
		exit(EXIT_FAILURE);
	}
	printf("FILE: %s; Client has written request to SERVERFIFO..\n", __FILE__);
	close(wfd);
	
	// Prepare fifo name
        char *clientFifoName = (char*)malloc(sizeof(char)*20);
        if (!clientFifoName)
        {
                perror("malloc");
                exit(EXIT_FAILURE);
        }

        // Fill with zero to avoid garbbage; always fill string 
        memset(clientFifoName, '\0', 20);

	sprintf(clientFifoName, CLIENTFIFO, pid);
	printf("FILE: %s; Client fifoName n client=> %s\n", __FILE__, clientFifoName);
	
	// create client fifo
	if ((access(clientFifoName, F_OK)) == -1)
	{
		mret = mkfifo(clientFifoName, 0666);
		if (mret == -1)
		{
			perror("mkfifo");
			// give error only if there is another error than file exist
			if (errno != EEXIST)
				exit(1);
		}
		printf("FILE: %s; Client fifo creted in client...\n", __FILE__);
	}	
	
	// Make client result fifo block on OPEN
	rfd = open(clientFifoName, O_RDONLY);
	if (rfd == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	printf("FILE: %s; CLIENT FIFO File => %s is opened for reading request..\n", __FILE__, clientFifoName);
	rret = read(rfd, &res, sizeof(Result));
	if (rret == -1)
	{
		perror("read");
		exit(EXIT_FAILURE);
	}
	printf("FILE: %s; Final RESULT ==>%f from pid =>%d\n", __FILE__, res.result, res.pid);
	close(rfd);
	printf("File: %s; Func: %s END\n", __FILE__, __func__);
}
