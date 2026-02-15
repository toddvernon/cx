//-------------------------------------------------------------------------------------------------
//
//  inaddr.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxInetAddressImpl Class
//
//-------------------------------------------------------------------------------------------------

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>


//-------------------------------------------------------------------------
// platform includes
//
//-------------------------------------------------------------------------
#if defined(_LINUX_) || defined(_SOLARIS) || defined(_OSX_) || defined(_SUNOS_) || defined(_IRIX6_)
#endif


//-------------------------------------------------------------------------
// local includes
//
//-------------------------------------------------------------------------
#include <cx/base/string.h>
#include <cx/base/handle.h>
#include <cx/net/inaddri.h>


#ifndef _CxINADDR_H_
#define _CxINADDR_H_


//-------------------------------------------------------------------------
// class CxInetAddress
//
//-------------------------------------------------------------------------
class CxInetAddress
{     
  public:
     	
    CxInetAddress( int port = 0, CxString hostname = "" );
    // Constructor

    CxInetAddress( sockaddr_in * );
    // Constructor

    CxInetAddress( const CxInetAddress& ina_ );
    // copy constructor

    CxInetAddress& operator=(const CxInetAddress& ina_ );
    // assignment operator
        
    ~CxInetAddress( void );
    // Destructor

    int process( void );
    // do any lookups

    CxString host( void );
    // return the target host name lookup

    int port( void );
    // return the target host port

    unsigned long ip( void );
    // return the target  host ip address

    CxString target( void );
    // return the target machine name

	sockaddr_in *sockAddr( void ) const;
	// return a pointer to socket address structure	

	size_t sockAddrSize( void ) const;
	// return the size of address structure

    int isValid( void );
    // is the address valid

    static CxString addressAsString( unsigned long ip_ );

    static CxString getHostByAddress( unsigned long ip_ );

    static unsigned long getHostByName( CxString name_ );

    static CxString me( void );


  private:

    CxHandle< CxInetAddressImpl > _impl;

};

#endif
