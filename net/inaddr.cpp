//-------------------------------------------------------------------------------------------------
//
//  inaddr.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxInetAddressImpl Class
//
//-------------------------------------------------------------------------------------------------

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
