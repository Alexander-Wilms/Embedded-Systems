client:
	g++ -lpthread client.cpp\
		socket/CSockConnector.h\
		socket/CSockConnector.cpp\
		socket/CSockAcceptor.h\
		socket/CSockAcceptor.cpp\
		socket/CSockStream.h\
		socket/CSockStream.cpp\
		socket/IIOStream.h\
		socket/IIOStream.cpp\
		socket/CInetAddr.h\
		socket/CInetAddr.cpp\
		CNamedSemaphore.h\
		CNamedSemaphore.cpp -o client.out && ./client.out

server:
	g++ -lpthread server.cpp\
		socket/CSockConnector.h\
		socket/CSockConnector.cpp\
		socket/CSockAcceptor.h\
		socket/CSockAcceptor.cpp\
		socket/CSockStream.h\
		socket/CSockStream.cpp\
		socket/IIOStream.h\
		socket/IIOStream.cpp\
		socket/CInetAddr.h\
		socket/CInetAddr.cpp\
		CNamedSemaphore.h\
		CNamedSemaphore.cpp -o server.out && ./server.out
