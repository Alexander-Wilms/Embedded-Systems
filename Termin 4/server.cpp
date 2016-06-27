#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <unistd.h> // for fork()
#include <stdio.h> // for printf()
#include <stdint.h> // for uint8_t
#include "socket/CInetAddr.h"
#include "socket/CSockAcceptor.h"
#include "socket/CSockConnector.h"
#include "socket/CSockStream.h"
#include "socket/Global.h"
#include "socket/IIOStream.h"
#include "socket/SocketIncludes.h"
#include "CNamedSemaphore.h"

// client and server need to know beforehand, how many bytes will be sent
#define MAX_CHARS 10

using namespace std;

int main()
{
	// SERVER

	// create new server
	CSockAcceptor server ;

	CInetAddr server_own_addr;
	server_own_addr.set(9000);

	// create socket, bind and listen on port 2000
	if (server.open(server_own_addr))
	printf("server listening on port 9000\n");

	UInt8 buf[MAX_CHARS];
	CSockStream server_stream;

	// wait for client to connect (blocking)
	if(server.accept(server_stream, 0)) {
	printf("server accepted connection\n");

	uint8_t receive_char[3];
	receive_char [2] = 0;
	int num_received = 0;

		num_received = server_stream.read(receive_char, 2) ;
		if(num_received >0)
			cout << "server received  2 bytes: " << receive_char << endl;
			//printf("server received: %s\n", receive_char);
} else
{
	printf("server failed to accept connection\n");
}
}
