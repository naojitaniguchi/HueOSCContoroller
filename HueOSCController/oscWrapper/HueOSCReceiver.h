//
//  HueOSCReceiver.h
//  HueOSCController
//
//  Created by 谷口 直嗣 on 2014/02/26.
//  Copyright (c) 2014年 Philips. All rights reserved.
//

#ifndef __HueOSCController__HueOSCReceiver__
#define __HueOSCController__HueOSCReceiver__

#include <iostream>


/*
 Example of two different ways to process received OSC messages using oscpack.
 Receives the messages from the SimpleSend.cpp example.
 */

#include <iostream>
#include <cstring>

#if defined(__BORLANDC__) // workaround for BCB4 release build intrinsics bug
namespace std {
    using ::__strcmp__;  // avoid error: E2316 '__strcmp__' is not a member of 'std'.
}
#endif

#include "osc/OscReceivedElements.h"
#include "osc/OscPacketListener.h"
#include "ip/UdpSocket.h"

#include <deque>

#ifdef TARGET_WIN32
// threads
#include <windows.h>
#else
// threads
#include <pthread.h>
#endif

// osc
#include "OscTypes.h"
#include "OscPacketListener.h"
#include "UdpSocket.h"



typedef void (*CALLBACKFUNCPTR)(int,int,int,int);

class HueOSCReceiver : public osc::OscPacketListener {
private:
    // socket to listen on
	UdpListeningReceiveSocket* listen_socket;

#ifdef TARGET_WIN32
	// thread to listen with
	HANDLE thread;
	// mutex for the thread queue
	HANDLE mutex;
#else
	// thread to listen with
	pthread_t thread;
	// mutex for the message queue
	pthread_mutex_t mutex;
#endif

    // ready to be deleted
	bool socketHasShutdown;
    
    // shutdown the listener
	void shutdown();

	// start the listening thread
#ifdef TARGET_WIN32
	static DWORD WINAPI startThread( void* hueOscReceiverInstance );
#else
	static void* startThread( void* hueOscReceiverInstance );
#endif
    CALLBACKFUNCPTR cbFuncPtr ;

protected:
    
    virtual void ProcessMessage( const osc::ReceivedMessage& m, const IpEndpointName& remoteEndpoint );

public:
    ~HueOSCReceiver();
    /// listen_port is the port to listen for messages on
	void setup( int listen_port );
    void setCallback(CALLBACKFUNCPTR funcPtr );
};


#endif /* defined(__HueOSCController__HueOSCReceiver__) */
