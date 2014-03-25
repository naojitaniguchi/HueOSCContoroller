//
//  HueOSCReceiver.cpp
//  HueOSCController
//
//  Created by 谷口 直嗣 on 2014/02/26.
//  Copyright (c) 2014年 Philips. All rights reserved.
//

#include "HueOSCReceiver.h"
#include <unistd.h>

void HueOSCReceiver::ProcessMessage( const osc::ReceivedMessage& m, const IpEndpointName& remoteEndpoint )
{
    (void) remoteEndpoint; // suppress unused parameter warning
    
    try{
        
        // std::cout << m.AddressPattern() ;
        
        // この中でObj-Cの関数が呼べると良い
        if( std::strcmp( m.AddressPattern(), "/hue" ) == 0 ){
            osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();
            int a0 = (arg++)->AsInt32();
            int a1 = (arg++)->AsInt32();
            int a2 = (arg++)->AsInt32();
            int a3 = (arg++)->AsInt32();

            if( arg != m.ArgumentsEnd() )
                throw osc::ExcessArgumentException();
            
            if ( cbFuncPtr ){
                (*cbFuncPtr)( a0, a1, a2, a3 );
            }
            //std::cout << "received '/test2' message with arguments: "
            //<< a1 << " " << a2 << " " << a3 << " " << a4 << "\n";
        }
        
        /*
        // example of parsing single messages. osc::OsckPacketListener
        // handles the bundle traversal.
        
        std::cout << m.AddressPattern() ;
        
        if( std::strcmp( m.AddressPattern(), "/test1" ) == 0 ){
            // example #1 -- argument stream interface
            osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
            bool a1;
            osc::int32 a2;
            float a3;
            const char *a4;
            args >> a1 >> a2 >> a3 >> a4 >> osc::EndMessage;
            
            std::cout << "received '/test1' message with arguments: "
            << a1 << " " << a2 << " " << a3 << " " << a4 << "\n";
            
        }else if( std::strcmp( m.AddressPattern(), "/test2" ) == 0 ){
            // example #2 -- argument iterator interface, supports
            // reflection for overloaded messages (eg you can call
            // (*arg)->IsBool() to check if a bool was passed etc).
            osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();
            bool a1 = (arg++)->AsBool();
            int a2 = (arg++)->AsInt32();
            float a3 = (arg++)->AsFloat();
            const char *a4 = (arg++)->AsString();
            if( arg != m.ArgumentsEnd() )
                throw osc::ExcessArgumentException();
            
            std::cout << "received '/test2' message with arguments: "
            << a1 << " " << a2 << " " << a3 << " " << a4 << "\n";
        }
         */
    }catch( osc::Exception& e ){
        // any parsing errors such as unexpected argument types, or
        // missing arguments get thrown as exceptions.
        std::cout << "error while parsing message: "
        << m.AddressPattern() << ": " << e.what() << "\n";
    }
}

void HueOSCReceiver::setup( int listen_port )
{
	// if we're already running, shutdown before running again
	if ( listen_socket )
		shutdown();
	
	// create the mutex
#ifdef TARGET_WIN32
	mutex = CreateMutexA( NULL, FALSE, NULL );
#else
	pthread_mutex_init( &mutex, NULL );
#endif
	
	// create socket
	socketHasShutdown = false;
	listen_socket = new UdpListeningReceiveSocket( IpEndpointName( IpEndpointName::ANY_ADDRESS, listen_port ), this );
    
	// start thread
#ifdef TARGET_WIN32
	thread	= CreateThread(
                           NULL,              // default security attributes
                           0,                 // use default stack size
                           &HueOSCReceiver::startThread,        // thread function
                           (void*)this,             // argument to thread function
                           0,                 // use default creation flags
                           NULL);             // we don't the the thread id
    
#else
	pthread_create( &thread, NULL, &HueOSCReceiver::startThread, (void*)this );
#endif
}

void HueOSCReceiver::shutdown()
{
	if ( listen_socket )
	{
		// tell the socket to shutdown
		listen_socket->AsynchronousBreak();
		// wait for shutdown to complete
		while (!socketHasShutdown)
		{
#ifdef TARGET_WIN32
			Sleep(1);
#else
			// sleep 0.1ms
			usleep(100);
#endif
		}
		
		// thread will clean up itself
		
		// clean up the mutex
#ifdef TARGET_WIN32
		ReleaseMutex( mutex );
#else
		pthread_mutex_destroy( &mutex );
#endif
		
		// delete the socket
		delete listen_socket;
		listen_socket = NULL;
	}
}

HueOSCReceiver::~HueOSCReceiver()
{
	shutdown();
}

#ifdef TARGET_WIN32
DWORD WINAPI
#else
void*
#endif

HueOSCReceiver::startThread( void* hueOscReceiverInstance )
{
	// cast the instance
	HueOSCReceiver* instance = (HueOSCReceiver*)hueOscReceiverInstance;
	// start the socket listener
	instance->listen_socket->Run();
	// socket listener has finished - remember this fact
	instance->socketHasShutdown = true;
	// return
#ifdef TARGET_WIN32
	return 0;
#else
	return NULL;
#endif
}

void HueOSCReceiver::setCallback(CALLBACKFUNCPTR _funcPtr ){
    cbFuncPtr = _funcPtr ;
}


