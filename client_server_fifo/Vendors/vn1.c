#include "../headers.h"
#include "../shrdd.h"

int main(int argc, char **argv)
{
	printf("File: %s; Func: %s BEGIN \n", __FILE__, __func__);
	
	Request req;
	Result res;
	int pid, rfd, wfd, rret, wret, result, total; 
	
	// convert sfd to fd
	if (argc >= 3)
	{
		rfd = atoi(argv[1]); // Read Request
		wfd = atoi(argv[2]); // Write result
	}

	// get pid of client
	pid = getpid();
	if (pid == -1)
	{
		perror("getpid");
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("FILE: %s; pid of vendor is => %d\n", __FILE__, getpid());
	}
	
	// read from server 
	rret = read(rfd, &req, sizeof(Request));
	if (rret == -1)
	{
		perror("read");
		exit(EXIT_FAILURE);
	}
        
	// Debug here
	printf("FILE: %s; Data recieved from Server\n", __FILE__);
	printf("FILE: %s; Requesting client PID => %d; oprtr1 %d; oprtr2 %d; oprnd %c\n", __FILE__, req.pid, req.oprnd1, req.oprnd2, req.oprtr);

	// calulate
	res.result = (float)(req.oprnd1 - req.oprnd2);
	res.pid = req.pid;
	printf("FILE: %s; Result calulated in vendor => %f\n", __FILE__, res.result);

	// Calculate and write
	wret = write(wfd, &res, sizeof(Result));	
	if (wret == -1)
	{
		perror("write");
		exit(EXIT_FAILURE);
	}	
	printf("FILE: %s; Result written in Server....\n", __FILE__);
	close(rfd);
	close(wfd);
	printf("File: %s; Func: %s END\n", __FILE__, __func__);
}
