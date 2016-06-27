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

//const char sem_name1[] = "/semaphore";
//CNamedSemaphore semaphore(sem_name1, 1);

int main()
{
	// CLIENT

	// zu sendende Nachricht
	char message [MAX_CHARS];

	// Adresse des Servers
	CInetAddr server_addr;
	// lokal
	 //server_addr.set(9000, "127.0.0.1");
	server_addr.set(9000, "172.17.15.191");

	// Klasse zum Verbinden von Client und Server
	CSockConnector client;

	// TCP Client Datenstrom
	CSockStream client_stream;

	// Verbinde Client mit Server
	if (client.connect(client_stream, server_addr, true))
	{
		printf("client successfully connected to server\n");
	} else
	{
		printf("client not successfully connected to server\n");
		// exit
		return 0;
	}
	
	sleep(10);

	// UInt8 sendvar;
	UInt8 sendvar[] = {'a','b'};
	//	printf("enter text: ");
	//	cin >> sendvar;
	printf("client sending 2 bytes to server\n");
	client_stream.write((const UInt8 * ) sendvar,  2);
}
