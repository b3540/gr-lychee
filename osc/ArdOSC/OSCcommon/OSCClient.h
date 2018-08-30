/*
 
 ArdOSC 2.1 - OSC Library for Arduino.
 
 -------- Lisence -----------------------------------------------------------
 
 ArdOSC
 
 The MIT License
 
 Copyright (c) 2009 - 2011 recotana( http://recotana.com )　All right reserved
 
 */


#ifndef OSCClient_h
#define OSCClient_h

#include "OSCcommon.h"
#include "OSCMessage.h"
#include "OSCEncoder.h"
#include "UDPSocket.h"
#include "netsocket/NetworkStack.h"
#include "netsocket/SocketAddress.h"

#define kDummyPortNumber 10000



class OSCClient{
	
private:
        
	UDPSocket _sock;
	NetworkStack *stack;    
    OSCEncoder encoder;
	
    int16_t sockOpen(void);
	void sockClose(void);
    
    
public:
    
	OSCClient(void);
	~OSCClient(void);

	void setStack(NetworkStack *stack) {
		this->stack = stack;
	}

	int16_t send( OSCMessage *_message);
    
			
};


#endif