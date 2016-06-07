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
#ifndef _CSOCKCONNECTOR_H_
#define _CSOCKCONNECTOR_H_

#include "CSockStream.h"
#include "CInetAddr.h"

class CSockConnector
{
public:
	CSockConnector();
	~CSockConnector();
	
	/**
	 * connecting
	 * \param stream the stream data sent or received
	 * \param address of the server to connect to
	 * \block whether the call should block
	 */
	bool connect(CSockStream & stream, CInetAddr& address, bool block=true);
	
	#ifdef WIN32
	private:
		static int mW32InitCounter;
	#endif
};
#endif //_CSOCKCONNECTOR_H_

