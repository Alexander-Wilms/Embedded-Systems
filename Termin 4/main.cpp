#include <iostream>
#include <stdlib.h>

#include <unistd.h> // for fork()
#include <stdio.h> // for printf()
#include <stdint.h> // for uint8_t

#include "socket/CInetAddr.h"
#include "socket/CSockAcceptor.h"
#include "./socket/CSockConnector.h"
#include "socket/CSockStream.h"
#include "socket/Global.h"
#include "socket/IIOStream.h"
#include "socket/SocketIncludes.h"

#define NUM_TESTS 3

int main() {
	// passive socket, server
	CSockAcceptor myCSockAcceptor = CSockAcceptor();

	// active socket, client
	CSockConnector myCSockConnector = CSockConnector();

	CSockStream myCSockStream = CSockStream();

	// fork process
	int pid = fork();
	printf("pid %d \n", pid);

	if(pid == -1) {
		printf("error due to fork()\n");
	}
	else if(pid != 0) { // parent
		printf("parent \n");
		
		myCSockStream.close();

		CInetAddr myCInetAddr = CInetAddr();

		// port 5
		myCInetAddr.set(5);

		// wait for incoming
		if(myCSockAcceptor.accept(myCSockStream),99999)
			printf("Server listens on port 5 \n");
			
		uint8_t recbuffer;
		
		// wait for incoming connection
		for(int i = 0nm  ; i < NUM_TESTS; i++) {
			myCSockStream.read(&recbuffer,1);
			std::cout << recbuffer << std::endl;
		}
	} else { // child
		printf("child \n");

		CInetAddr myCInetAddr = CInetAddr();

		// port 5, address 7
		myCInetAddr.set(5,"7");

		myCSockConnector.connect(myCSockStream, myCInetAddr);

		const uint8_t * writebuffer = (uint8_t *) 'a';
		
		for(int i = 0; i < NUM_TESTS; i++) {
			myCSockStream.write(writebuffer,1);
		}
	}
}
