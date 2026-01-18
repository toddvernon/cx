/***************************************************************************
 *
 *  inetaddri.cpp
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


#include <cx/base/string.h>
#include <cx/net/inaddri.h>



//-------------------------------------------------------------------------
// CxInetAddressImpl::CxInetAddressImpl
//
//-------------------------------------------------------------------------
CxInetAddressImpl::CxInetAddressImpl( int port_, CxString target_ )
{
    _target    = target_;
    _port      = port_;
    _processed = FALSE;
    _valid     = FALSE;
}


//-------------------------------------------------------------------------
// CxInetAddressImpl::CxInetAddressImpl
//
//-------------------------------------------------------------------------
CxInetAddressImpl::CxInetAddressImpl( sockaddr_in *saddr_ )
{
	memcpy((void*) &_saddr, (void*) saddr_, sizeof( *saddr_ ) ); 
	unprocess();
}


//-------------------------------------------------------------------------
// CxInetAddressImpl::~CxInetAddressImpl
//
//-------------------------------------------------------------------------
CxInetAddressImpl::~CxInetAddressImpl( void )  
{ 
}


//-------------------------------------------------------------------------
// CxInetAddressImpl::unprocess
//-------------------------------------------------------------------------
void
CxInetAddressImpl::unprocess( void )
{
    memcpy( (char*) &_ip, (char*) &_saddr.sin_addr, 4);

    // Always extract the port - it's valid even when IP is INADDR_ANY (0)
    _port = ntohs( _saddr.sin_port );

    if (_ip != 0 ) {

	    _host   = CxInetAddressImpl::getHostByAddress( _ip );
	    _target = _host;
        _valid  = TRUE;

    } else {

        // IP is INADDR_ANY - valid for binding, but no specific host
        _valid  = TRUE;
    }

    _processed = TRUE;
}


//-------------------------------------------------------------------------
// CxInetAddress::process
//
//-------------------------------------------------------------------------
int 
CxInetAddressImpl::process( void )	
{

	memset(  &_saddr, 1, sizeof( _saddr ));
//	bzero( &_saddr, sizeof( _saddr ));
	
    _saddr.sin_family = AF_INET;
    _saddr.sin_port   = htons( _port );
            
    // if we have no host name them address any
    if (!_target.length()) {

        _ip = INADDR_ANY;
        _saddr.sin_addr.s_addr = _ip;
        
        _valid = TRUE;

    } else {
        
        _ip   = CxInetAddressImpl::getHostByName( _target.data() );

        if (_ip != 0) {
           _host = CxInetAddressImpl::getHostByAddress( _ip ); 
            memcpy( (char*) &_saddr.sin_addr,(char*) &_ip, 4);
            _valid = TRUE;
        } else {
            _valid = FALSE;
        }
    }

    _processed = TRUE;

    return( 1 );
}


//-------------------------------------------------------------------------
// CxInetAddressImpl::address
//
//-------------------------------------------------------------------------
unsigned long
CxInetAddressImpl::ip( void )
{
    if (!_processed) process();
    return( _ip );
}


//-------------------------------------------------------------------------
// CxInetAddressImpl::port
//
//-------------------------------------------------------------------------
int 
CxInetAddressImpl::port( void )
{
    if (!_processed) process();
    return( _port );
}


//-------------------------------------------------------------------------
// CxInetAddressImpl::host
//
//-------------------------------------------------------------------------
CxString
CxInetAddressImpl::host( void )
{
    if (!_processed) process();
    return( _host );
}


//-------------------------------------------------------------------------
// CxInetAddressImpl::target
//
//-------------------------------------------------------------------------
CxString
CxInetAddressImpl::target( void )
{
    if (!_processed) process();
    return( _target );
}

//-------------------------------------------------------------------------
// CxInetAddressImpl::sockAddr: 
//
//-------------------------------------------------------------------------        
int
CxInetAddressImpl::isValid( void )
{
    if (!_processed) process();
    return( _valid );
}


//-------------------------------------------------------------------------
// CxInetAddressImpl::sockAddr: 
//
//-------------------------------------------------------------------------        
sockaddr_in * 
CxInetAddressImpl::sockAddr()
{
    if (!_processed) process();
    return ((sockaddr_in *) &_saddr);
}


//-------------------------------------------------------------------------
// CxInetAddressImpl::sockAddrSize: 
//
//-------------------------------------------------------------------------        
SOCKLEN_T 
CxInetAddressImpl::sockAddrSize() 
{
    if (!_processed) process();
    return sizeof(_saddr);
}  


//-------------------------------------------------------------------------
// CxInetAddressImpl::getHostByAddress: <static>
//
//-------------------------------------------------------------------------        
CxString
CxInetAddressImpl::getHostByAddress( unsigned long ip_ )
{
    struct hostent *hp = gethostbyaddr((char *)& ip_, sizeof ( ip_ ), AF_INET);           
    if (hp == NULL) return("");
    return( CxString( hp->h_name ) );
}


//-------------------------------------------------------------------------
// CxInetAddressImpl::getHostByName: <static>
//
//-------------------------------------------------------------------------        
unsigned long
CxInetAddressImpl::getHostByName( CxString name_ )
{
    struct hostent *hp = gethostbyname( name_.data() );            	
    if (hp==0) return 0;
    unsigned long ip = ((struct in_addr*)((hp->h_addr_list)[0]))->s_addr;
    return( ip );
}


//-------------------------------------------------------------------------
// CxInetAddressImpl::addressAsString: <static>
//
//-------------------------------------------------------------------------
CxString
CxInetAddressImpl::addressAsString( unsigned long ip_ )
{
  in_addr addr;
  addr.s_addr = ip_;
  CxString as = inet_ntoa( addr );
  return as;
}


//-------------------------------------------------------------------------
// CxInetAddressImpl::me <static>
//
//-------------------------------------------------------------------------
CxString
CxInetAddressImpl::me( void )
{
  int bs = 257;
  char buf[257];
  gethostname(buf, bs);
  return( CxString( buf ));
}  
