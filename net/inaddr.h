/***************************************************************************
 *
 *  inetaddri.h
 *
 *  CxInetAddressImpl Class
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
// standard includes
//
//-------------------------------------------------------------------------
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
