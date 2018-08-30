/*
 
 ArdOSC 2.1 - OSC Library for Arduino.
 
 -------- Lisence -----------------------------------------------------------
 
 ArdOSC
 
 The MIT License
 
 Copyright (c) 2009 - 2011 recotana( http://recotana.com )　All right reserved
 
 */



#include <stdlib.h>

#include "OSCCommon/OSCClient.h"


OSCClient::OSCClient(void){
}

OSCClient::~OSCClient(void){
}


int16_t OSCClient::sockOpen(void){
	_sock.open(stack);
    return 1;
}

void OSCClient::sockClose(void){
    _sock.close();
}



int16_t OSCClient::send(OSCMessage *_message){
	
    uint16_t result=0;
	uint8_t _sendData[4096];
	memset(_sendData, 0, sizeof(_sendData));
    
	if( encoder.encode( _message , _sendData ) < 0 ){
		return -1;
	}
    
    
    if( sockOpen()<0 ) return -1; //socket open check


    result = _sock.sendto( (const char *)_message->getIpAddress(), _message->getPortNumber(), _sendData , _message->getMessageSize() );
 
	sockClose();
    
	return result;
    
}


