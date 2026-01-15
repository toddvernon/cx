/***************************************************************************
 *
 *  inetaddri.h
 *
 *  CxInetAddressImpl Class
 *
 ***************************************************************************
 *
 * (c) Copyright 1999-2022 T.Vernon
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

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <strings.h>

#include <cx/base/string.h>

//-------------------------------------------------------------------------
// specific things related to linux
//-------------------------------------------------------------------------
#if defined(_LINUX_) || defined(_OSX_) || defined(_SOLARIS10_) || defined(_NETBSD_)
#define SOCKLEN_T socklen_t
#endif

//-------------------------------------------------------------------------/
// specific things related to solaris
//-------------------------------------------------------------------------
#if  defined(_IRIX6_) || defined(_SOLARIS6_)
#define SOCKLEN_T int
#endif

//-------------------------------------------------------------------------
// specific things related to old sunos
//-------------------------------------------------------------------------
#if defined(_SUNOS_)
#define SOCKLEN_T int
#include <netinet/in.h>
int gethostname(char *name, int namelen);
#endif
    
#ifndef _CxINADDRIMPL_H_
#define _CxINADDRIMPL_H_



//-------------------------------------------------------------------------
// CxInetAddressImpl::CxInetAddressImpl
//
//-------------------------------------------------------------------------
class CxInetAddressImpl
{
     
  public:
     	
    CxInetAddressImpl( int port = 0, CxString hostname = "" );
    // Constructor
        
    CxInetAddressImpl( struct sockaddr_in *saddr_ );
    // Constructor

    ~CxInetAddressImpl( void );
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
   
    sockaddr_in *sockAddr( void );
    // return a pointer to socket address structure

    SOCKLEN_T sockAddrSize( void );
    // return the size of address structure

    int isValid( void );
    // is address valid

    static CxString addressAsString( unsigned long ip_ );
	// return an ip address as string

    static CxString getHostByAddress( unsigned long ip_ );
	// return a host name from an ip address

    static unsigned long getHostByName( CxString name_ );
	// return an ip address based on a name

    static CxString me( void );
	// return name of self


  private:
	
	void unprocess(void);

    struct sockaddr_in   _saddr;
    CxString             _target;
    CxString             _host;
    int                  _port;
    unsigned long        _ip;
    int                  _processed;
    int                  _valid;
};


#endif
