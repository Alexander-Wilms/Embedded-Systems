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

const char sem_name1[] = "/semaphore";
CNamedSemaphore semaphore(sem_name1, 1);

int main()
{
    // SERVER
    //###########################

    CSockAcceptor server = CSockAcceptor();

    CInetAddr server_own_addr;
    server_own_addr.set(2000);

    // listen on port 2000
    if (server.open(server_own_addr))
        printf("server listening on port 2000\n");

    getchar();
    
    UInt8 buf[MAX_CHARS];
    CSockStream server_stream;
    //##########################

    // CLIENT
    //##########################
    // zu sendende Nachricht
    char msg[MAX_CHARS];

    // Adresse des Servers
    CInetAddr server_addr;
    // lokal
   server_addr.set(2000, "127.0.0.1");
    // remote
   //server_addr.set(2000, "192.168.220.144");

    // Klasse zum Verbinden von Client und Server
    CSockConnector client = CSockConnector();

    // TCP Client Datenstrom
    CSockStream client_stream;

    // Verbinde Client mit Server
    if (client.connect(client_stream, server_addr, 1))
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
    // parent
    else if (pid != 0) {
        printf("parent \n");

        if (server.accept(server_stream, 0)) {
            printf("server accepted connection\n");

            // number of received packets
            int actually_received = 0;

	   // save received bytes in buf and the number of received bytes in actually_received
            while ((actually_received = server_stream.read(buf, MAX_CHARS)) > 0) {
               // create array with the same length as the number of bytes received
                char* output_string = new char[actually_received];
                // copy actually_received bytes from buf to c
                memcpy(output_string, buf, actually_received);
                printf("server received %d bytes\n", actually_received);
                cout << "server received: " << output_string << endl;

                server_stream.write((const UInt8*) "ack", 3);
            }
        }
    }
    // child
    else {
        printf("child \n");

	// do not limit the number of messages
       while(1)
        {
            // clear message buffer
            for (int k = 0; k < MAX_CHARS; k++)
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
                if ((pos + MAX_CHARS) > strlen(msg))
                {
                    num_to_send = MAX_CHARS - ((pos + MAX_CHARS) - strlen(msg));
                    // we're not done yet
                    send = false;
                } else // if we haven't...
                {
                    num_to_send = MAX_CHARS;
                }

                // wenn noch packets zu senden sind...
                if (num_to_send > 0) {

                    char* buf = new char[MAX_CHARS];
                    memcpy(buf, msg + pos, num_to_send);

                    printf("client sent: %s\n", buf);
                    client_stream.write((const UInt8*) buf, MAX_CHARS);

                    // receive acknowledge bit
                    UInt8 buf_ack[3];
                    // read next byte in the client stream
                    client_stream.read(buf_ack, 3);
                    char ack[3];
                    memcpy(&ack, buf_ack, 3);
                    printf("client received ack: %s\n", ack);

                    // check if the server has received the packet
                    // If it has, go to the next packet
                    // if it hasn't, resend the current packet
                    if ("ack" == ack) {
                        pos += num_to_send;
                    }
                }
            }
        }
        client_stream.close();
    }
}
