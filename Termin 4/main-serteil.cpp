   // fork process
    int pid = fork();
    printf("pid %d: ", pid);

    // parent
if (pid != 0) {
	printf("parent \n");

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
	if(server.accept(server_stream, 1)) {
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
// child
else {
	printf("child \n");
