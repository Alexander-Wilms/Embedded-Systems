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
#ifndef _CSOCKACCEPTOR_H_
#define _CSOCKACCEPTOR_H_

#include "CInetAddr.h"
#include "CSockStream.h"

class CSockAcceptor
{
public:
	CSockAcceptor();
	~CSockAcceptor();
	
	/**
	 * Initialize CInetAddr listen passively
	 */
	bool open(CInetAddr& address);
	
	/**
	 * Init CSockStream with a new client connection
	 */
	bool accept(CSockStream& stream, Int8 timeout=0);
private:
	Int32 mListenfd;
	// used to remember initializing of windows sockets
	#ifdef WIN32
		static int mW32InitCounter;
	#endif
};
#endif //_CSOCKACCEPTOR_H_

