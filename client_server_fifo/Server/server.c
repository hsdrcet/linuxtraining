// Server 
// It will create 2 pipes for each vendor. total 8 pipes for add, sub, mul, div
// Pipe1 => Vendor Request: Server will write request to Vendor using pipe1
// Pipe2 => Vendor Result : Vendor write result tp pipe2; Server read Result
// Server will create server fifo >> wait for Client requests >> pass data to appropriate vendor >> read result >> Write in requesting client's fifo. 

#include "../shrdd.h"
#include "../headers.h"
int main()
{	
	int count;
	Infra *infra;

	printf("File:%s; Func:%s. BEGIN\n", __FILE__, __func__);
	infra = createInfra();

	while(1)
	{
		processRequest(infra);
	}

	printf("File:%s; Func:%s. END\n", __FILE__, __func__);
	return 0;
}
