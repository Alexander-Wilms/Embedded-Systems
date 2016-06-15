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

#define BUFFER_SIZE 32

using namespace std;

const char sem_name1[] = "/semaphore";
CNamedSemaphore semaphore(sem_name1, 1);

int main()
{
    // SERVER
    //###########################
    printf("server running\n");

    CSockAcceptor* server = new CSockAcceptor();

    CInetAddr server_own_addr;
    server_own_addr.set(2000);

    // listen on port 2000
    if (server->open(server_own_addr))
        printf("server listening on port 2000\n");

	getchar();
    UInt8 buf[BUFFER_SIZE];
    CSockStream server_stream;
    //##########################

    // CLIENT
    //##########################
    // zu sendende Nachricht
    char msg[32];

    // Adresse des Servers
    CInetAddr server_addr;
    // lokal
    server_addr.set(2000, "127.0.0.1");
    // remote
   // server_addr.set(2000, "192.168.220.144");

    // Klasse zum Verbinden von Client und Server
    CSockConnector* client = new CSockConnector();

    // TCP Client Datenstrom
    CSockStream client_stream;

    // Verbinde Client mit Server
    if (client->connect(client_stream, server_addr, 1))
        printf("successfully connected\n");
    else
    {
        printf("not successfully connected\n");
        return 0;
    }

    printf("client running\n");

    char mychar;
    int j = 0;

    // fork process
    int pid = fork();
    printf("pid %d \n", pid);

    if (pid == -1) {
        printf("error \n");
    }
    else if (pid != 0) { // parent
        printf("parent \n");

        if (server->accept(server_stream, 0)) {
            cout << "server accepted connection\n" << endl;

            // number of received packets
            int num_received = 0;

            while ((num_received = server_stream.read(buf, BUFFER_SIZE)) > 0) {
                char* c = new char[num_received];
                memcpy(c, buf, num_received);
                cout << "server received: " << c << endl;

                server_stream.write((const UInt8*)"1", 1);
            }
        }
    }
    else { // child
        printf("child \n");

        for (int i = 0; i < 100; i++)
        {
            // clear message buffer
            for (int k = 0; k < 32; k++)
            {
                msg[k] = 0;
            }
            
            printf("\nenter text: ");
            // fill message buffer with user input
            while ((mychar = getchar()) != '\n' && j < 15)
            {
                msg[j] = mychar;
                j++;
            }
            // make sure that we don't just append the text next time
            j = 0;
            
            int pos = 0;
            bool send = true;
            
            while (send)
            {
                int num_to_send = 0;
                // if we've already sent some chars...
                if ((pos + BUFFER_SIZE) > strlen(msg))
                {
                    num_to_send = BUFFER_SIZE - ((pos + BUFFER_SIZE) - strlen(msg));
                    // we're not done yet
                    send = false;
                } else // if we haven't...
                {
                    num_to_send = BUFFER_SIZE;
                }

                // wenn noch packets zu senden sind...
                if (num_to_send > 0) {

                    char* buf = new char[BUFFER_SIZE];
                    memcpy(buf, msg + pos, num_to_send);

                   printf("client sent: %s\n", buf);
                    client_stream.write((const UInt8*)buf, BUFFER_SIZE);

                    // Receive ACK
                    UInt8 buf_ack[1];
                    client_stream.read(buf_ack, 1);
                    char* ack = new char[1];
                    memcpy(ack, buf_ack, 1);
                    printf("client received ack: %c\n", *ack);

                    // Check if the server has received the packet
                    // If it has, go to the next packet
                    // if it hasn't, resend the current packet
                    if ('1' == ack[0]) {
                        pos += num_to_send;
                    }
                }
            }
        }
        client_stream.close();
    }
}
