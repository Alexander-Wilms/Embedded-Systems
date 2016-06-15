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

#ifndef _ICMSOCKET_H_
#define _ICMSOCKET_H_

	#if defined(__linux__) || defined(__SH__)
		#include "sys/socket.h"
		#include <unistd.h> // close()...
		#include <netinet/in.h> // in_addr_t, sockaddr_in
		#include <arpa/inet.h>
		#include <sys/select.h>
	#elif defined(WIN32)
		#include <winsock2.h>
		#include <io.h>
	#endif
	
	#include <sys/types.h>
#endif //_ICMSOCKET_H_
