//-------------------------------------------------------------------------------------------------
//
//  socketi.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxSocket Class
//
//-------------------------------------------------------------------------------------------------

#include <iostream>
#include <stdio.h>

//-------------------------------------------------------------------------
// SunOS 4.x system headers don't have C++ extern "C" guards
//-------------------------------------------------------------------------
#if defined(_SUNOS_)
extern "C" {
#endif

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

#if defined(_SUNOS_)
}
#endif

//-------------------------------------------------------------------------
// specific things related to linux
//-------------------------------------------------------------------------
#if defined(_LINUX_) || defined(_OSX_) || defined(_SOLARIS10_) || defined(_NETBSD_)
#define SOCKLEN_T socklen_t
#endif

//-------------------------------------------------------------------------/
// specific things related to solaris
//-------------------------------------------------------------------------
#if defined(_IRIX6_) || defined(_SOLARIS6_)
#define SOCKLEN_T int
#endif

//-------------------------------------------------------------------------
// specific things related to old sunos
//-------------------------------------------------------------------------
#if defined(_SUNOS_)

typedef int socklen_t;

#define SOCKLEN_T int

// must include socket library function prototypes (with C linkage)
extern "C" {
int socket(int domain, int type, int protocol);
int bind(int s, struct sockaddr *name, int namelen);
int listen(int s, int backlog);
void bzero(char *b, int length);
int select(int width, fd_set *redfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
int accept(int s, struct sockaddr *addr, int *addrlen);
int getpeername(int s, struct sockaddr *name, int *namelen);
int getsockname(int s, struct sockaddr *name, int *namelen);
int connect(int s, struct sockaddr *name, int namelen);
int send(int s, char *msg, int len, int flags);
int recv(int s, char *buf, int len, int flags);
int sendto(int s, char *msg, int len, int flags, struct sockaddr *to, int tolen);
int recvfrom(int s, char *buf, int len, int flags, struct sockaddr *from, int *fromlen);
int shutdown(int s, int how);
int gethostname(char *name, int namelen);
}

#endif



#define CLOSESOCKET_MACRO(X) close(X)
#define CxSOCKET_NOT_CONNECTED ENOTCONN



//-------------------------------------------------------------------------
// local headers
//
//-------------------------------------------------------------------------
#include <cx/base/string.h>
#include <cx/base/time.h>
#include <cx/base/exception.h>
#include <cx/base/err.h>
#include <cx/net/inaddr.h>



#ifndef _CxSOCKETIMPL_H_
#define _CxSOCKETIMPL_H_



//-------------------------------------------------------------------------
// class CxSocketImpl
//
//-------------------------------------------------------------------------
class CxSocketImpl 
{
     
  public:
     	
    CxSocketImpl( int dom=AF_INET, int type=SOCK_STREAM, int protocol=0 );
    // constructor
        
    ~CxSocketImpl();
    // Destructor

    int fd( void );
    // return the socket file descriptor
        
    int good( void );
    // check sock object status
	
    int bind( CxInetAddress addr_ );
    // bind an address to a socket

    int listen( int backlog_ = 5 );
    // listen for a connection

    CxSocketImpl *accept ( void );
    // accept a connection on the waiting socket

    int connect( CxInetAddress addr_, int timeout_=0 );
    // connect to a UNIX socket

    int send( char* buf, int len, unsigned int flag=0 ); 	
    // write data

    void sendAtLeast( char* buf, int len, unsigned int flag=0 );
    // send a least len bytes

    int recv( char* buf, int len, unsigned int flag=0 ); 	
    // read data
	
    void recvAtLeast( char* buf, int len, unsigned int flag=0 ); 	
    // read at least len bytes

    CxString recvUntil( char c, unsigned int flag=0 );
    // read until character

    int sendto( char* buf, int len, unsigned int flag, CxInetAddress addr_ );
    // write to a UNIX socket of the given socket name       	

    int recvfrom( char* buf, int len, unsigned int flag=0, CxInetAddress* addr_=NULL );
    // recieve a message from a UNIX domain socket
       	
    int shutdown( int mode = 2 );	
    // shutdown a socket

    int shutdownread( void );
    // close the read side of the socket

    int shutdownwrite( void );
    // close the write side of the socket

    void setNonBlocking( void );
    // ask the socket not to block

    void setBlocking( void );
    // ask the socket to block

    void close( void );
    // close the socket

    int select( unsigned int sec_, unsigned int usec_, unsigned int *flags_ );
    // select call 
   
    int recvDataPending( unsigned int sec_, unsigned int usec_=0 );
    // returns true if data pending

    CxInetAddress getpeername( void );
    // return the address of the peer

    CxInetAddress getsockname( void );
    // return the address of this socket

    static int initWinSock( void );
    // initialize winsock if on windows

    
  private:

    int _sfd;     	
    // socket descriptor

};


#endif
