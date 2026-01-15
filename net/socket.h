/***************************************************************************
 *
 *  socket.h
 *
 *  CxSocket Class
 *
 ***************************************************************************
 *
 * (c) Copyright 1999,2000,2001 T.Vernon
 * ALL RIGHTS RESERVED
 *
 * The software and information contained herein are proprietary to, and
 * comprise valuable trade secrets of, T.Vernon , which
 * intends to preserve as trade secrets such software and information.
 * This software is furnished pursuant to a written license agreement and
 * may be used, copied, transmitted, and stored only in accordance with
 * the terms of such license and with the inclusion of the above copyright
 * notice.  This software and information or any other copies thereof may
 * not be provided or otherwise made available to any other person.
 *
 *
 **************************************************************************/

//-------------------------------------------------------------------------
// system headers
//-------------------------------------------------------------------------
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

//-------------------------------------------------------------------------
// local headers
//-------------------------------------------------------------------------
#include <cx/net/inaddr.h>
#include <cx/net/socketi.h>



#ifndef _CxSOCKET_H_
#define _CxSOCKET_H_



//-------------------------------------------------------------------------
// class CxSocket
//
//-------------------------------------------------------------------------
class CxSocket 
{
     
  public:

    CxSocket( CxSocketImpl *);
    // constructor taking a new implementation
     	
    CxSocket( int dom=AF_INET, int type=SOCK_STREAM, int protocol=0 );
    // constructor

    CxSocket( const CxSocket& sock_ );
    // copy constructor
        
    ~CxSocket();
    // Destructor

    CxSocket& operator=(const CxSocket& sock_);

	int fd( void );
    // return the socket file descriptor
        
    int good( void );
    // check sock object status
	
    int bind( CxInetAddress addr_ );
    // bind an address to a socket

    int listen( int backlog_ = 5);
    // listen for a connection

    CxSocket accept ( void );
    // accept a connection on the waiting socket

    int connect( CxInetAddress addr_, int timeout_=0 );
    // connect to a UNIX socket

    int send( char* buf, int len, unsigned int flag=0 ); 	
    // write data

    void sendAtLeast( char* buf, int len, unsigned int flag=0);
    // send at least len bytes

    void sendAtLeast( CxString buf, unsigned int flag=0 );
    // send at the least characters    

    int recv( char* buf, int len, unsigned int flag=0 ); 	
    // read data

    CxString recv( int len, unsigned int flag=0 ); 	
    // read more then zero but not more then len bytes
	
    void recvAtLeast( char* buf, int len, unsigned int flag=0 ); 	
    // read at lest len bytes from stream

    CxString recvAtLeast( int len, unsigned int flag=0 ); 	
    // read at lest len bytes from stream and return as CxString

    CxString recvUntil( char c, unsigned int flag=0 );
    // read character until the c character is read 	
    
    int sendto( char* buf, int len, unsigned int flag, CxInetAddress addr_ );
    // write to a UNIX socket of the given socket name       	

    int recvfrom( char* buf, int len, unsigned int flag=0, CxInetAddress* addr_=NULL );
    // receieve a message from a UNIX domain socket
       	
    int shutdown( int mode = 2 );	
    // shutdown a socket

    int shutdownread( void );
    // shutdown the read side of the socket

    int shutdownwrite( void );
    // shutdown the write side of the socket

    void close( void );
    // close the socket

    int select( unsigned int sec_, unsigned int usec_, unsigned int *_flags );
    // select call &1=read, &2=write, &4=except

    int recvDataPending( unsigned int sec, unsigned int usec=0 );
    // returns true if data can be read with not blocking

    CxInetAddress getpeername( void );
    // get the address of the peer computer

    CxInetAddress getsockname( void );
    // get the address of this socket

    void setNonBlocking( void );
    // set the socket Non blocking

    void setBlocking( void );
    // set the socket blocking

    static int initWinSock( void );
    // initialize winsock if on windows
    
  private:

    CxHandle< CxSocketImpl > _impl;
    // handle to the implementation (guts)
};


#endif
