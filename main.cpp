


#define BUFSIZE 1024
#define DEFAULT_PORT 9090

#include "SocketManager.hpp"
#include <cstdlib>

int main(int argc, char **argv)
{
	int portno ;

	if (argc == 2)
		portno = atoi(argv[2]) ;
	else
		portno = DEFAULT_PORT ;
	SocketManager::startServer(portno) ;
}
