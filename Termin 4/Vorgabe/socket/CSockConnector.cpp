/***************************************************************************
 *  openICM-framework
 ***************************************************************************
 *  Copyright 2010 Joachim Wietzke and Manh Tien Tran
 *
 *  Contact and informations:
 *  University of Applied Sciences Darmstadt, Germany
 *  	http://www.fbi.h-da.de/~j.wietzke
 *  or
 *  	http://www.fbi.h-da.de/~openicm
 *
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  	http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
*****************************************************************************/
#include "CSockConnector.h"

// includes header for socket handling 
#if defined (__GNUC__)
	#include <sys/socket.h>
#elif defined(WIN32)
	#ifndef _WINSOCK2API_
		#include <winsock2.h>
	#endif
#endif

#include <sys/types.h>

#if defined (__GNUC__)
	#include <unistd.h>
    #include <fcntl.h>
#endif

#ifdef WIN32
   int CSockConnector::mW32InitCounter = 0;
#endif

CSockConnector::CSockConnector() 
{ 
}

/**
 * \brief used to cleanup the windows socket(s)
 */
CSockConnector::~CSockConnector() 
{ 
	#ifdef WIN32
		for (; mW32InitCounter > 0; mW32InitCounter--)
		{
			WSACleanup();
		}
	#endif
}

bool CSockConnector::connect(CSockStream& stream, CInetAddr& address, bool block)
{
	int sockfd;
   
	#ifdef WIN32
	if ( INVALID_SOCKET == (sockfd = socket( AF_INET, SOCK_STREAM, 0)) )
	{
		if (WSANOTINITIALISED == WSAGetLastError()) 
		{
			WORD wVersionRequested;
			WSADATA wsaData;
			int err;

			wVersionRequested = MAKEWORD( 2, 2 );

			err = WSAStartup( wVersionRequested, &wsaData );
			if ( err != 0 ) 
			{
				return false;
			}

			if ( LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 2 ) 
			{
				WSACleanup();
				return false; 
			}

			// Winsock initialized, try again:
			if (INVALID_SOCKET == (sockfd = socket( AF_INET, SOCK_STREAM, 0)))
			{
				return false;
			}
			
			// We got the Socket. Remember to clean it up:
			mW32InitCounter++;
		} 
		else
		{
			return false;
		}
	}
	#else // check for __GNUC__ directive
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0 )) < 0)
	{
		return false;
	}
	#endif //WIN32
   
	if (::connect(sockfd, (sockaddr*) &address.mInetAddr, sizeof(address.mInetAddr)) < 0)
	{
		return false;
	}
	// save the socket filedescriptor
	stream.mSocket = sockfd;
	return true;
}

