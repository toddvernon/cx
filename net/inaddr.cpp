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
#include <cx/net/inaddr.h>



//-------------------------------------------------------------------------
// CxInetAddress::CxInetAddress
//
//-------------------------------------------------------------------------
CxInetAddress::CxInetAddress( int port_, CxString target_ )
: _impl( new CxInetAddressImpl( port_, target_ ) )
{
}


//-------------------------------------------------------------------------
// CxInetAddress::CxInetAddress
//
//-------------------------------------------------------------------------
CxInetAddress::CxInetAddress( sockaddr_in *saddr_)
: _impl( new CxInetAddressImpl( saddr_ ) )
{
}

        
//-------------------------------------------------------------------------
// CxInetAddress::~CxInetAddress
//
//-------------------------------------------------------------------------
CxInetAddress::~CxInetAddress( void )  
{ 
}


//-------------------------------------------------------------------------
// CxInetAddress::~CxInetAddress
//
//-------------------------------------------------------------------------
CxInetAddress::CxInetAddress( const CxInetAddress& ina_ )  
{
    if (this != &ina_) { 
        _impl = ina_._impl;
    }
}


//-------------------------------------------------------------------------
// CxInetAddress::~CxInetAddress
//
//-------------------------------------------------------------------------
CxInetAddress& CxInetAddress::operator=( const CxInetAddress& ina_ )  
{ 
    if (this != &ina_) { 
        _impl = ina_._impl;
    }
    return( *this );
}


//-------------------------------------------------------------------------
// CxInetAddress::process
//
//-------------------------------------------------------------------------
int 
CxInetAddress::process( void )	
{
    return( _impl->process( ) );
}


//-------------------------------------------------------------------------
// CxInetAddress::ip
//
//-------------------------------------------------------------------------
unsigned long
CxInetAddress::ip( void )
{
    return( _impl->ip( ) );
}


//-------------------------------------------------------------------------
// CxInetAddress::port
//
//-------------------------------------------------------------------------
int 
CxInetAddress::port( void )
{
    return( _impl->port( ) );
}


//-------------------------------------------------------------------------
// CxInetAddress::host
//
//-------------------------------------------------------------------------
CxString
CxInetAddress::host( void )
{
    return( _impl->host( ) );
}


//-------------------------------------------------------------------------
// CxInetAddress::target
//
//-------------------------------------------------------------------------
CxString
CxInetAddress::target( void )
{
    return( _impl->target( ) );
}


//-------------------------------------------------------------------------
// CxInetAddress::sockAddr:
// 
//-------------------------------------------------------------------------
sockaddr_in *
CxInetAddress::sockAddr() const
{
    return( _impl->sockAddr( ) );
}


//-------------------------------------------------------------------------
// CxInetAddress::sockAddrSize:
// 
//-------------------------------------------------------------------------
size_t
CxInetAddress::sockAddrSize() const
{
    return( _impl->sockAddrSize( ) );
}


//-------------------------------------------------------------------------
// CxInetAddress::isValid:
// 
//-------------------------------------------------------------------------
int CxInetAddress::isValid( void )
{
    return( _impl->isValid());
}

//-------------------------------------------------------------------------
// CxInetAddress::getHostByAddress: <static>
//
//-------------------------------------------------------------------------        
CxString
CxInetAddress::getHostByAddress( unsigned long ip_ )
{
    return( CxInetAddressImpl::getHostByAddress( ip_ ) );
}


//-------------------------------------------------------------------------
// CxInetAddress::getHostByName: <static>
//
//-------------------------------------------------------------------------        
unsigned long
CxInetAddress::getHostByName( CxString name_ )
{
    return( CxInetAddressImpl::getHostByName( name_ ) );
}


//-------------------------------------------------------------------------
// CxInetAddress::addressAsString: <static>
//
//-------------------------------------------------------------------------
CxString
CxInetAddress::addressAsString( unsigned long ip_ )
{
    return( CxInetAddressImpl::addressAsString( ip_ ) );
}


//-------------------------------------------------------------------------
// CxInetAddress::me <static>
//
//-------------------------------------------------------------------------
CxString
CxInetAddress::me( void )
{
    return( CxInetAddressImpl::me( ) );
}
