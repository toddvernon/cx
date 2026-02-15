//-------------------------------------------------------------------------------------------------
//
//  socket.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxSocket Class
//
//-------------------------------------------------------------------------------------------------

#include <cx/base/string.h>
#include <cx/net/socket.h>



//-------------------------------------------------------------------------
// CxSocket::CxSocket
//
//-------------------------------------------------------------------------
CxSocket::CxSocket( int domain_, int type_, int protocol_ ) 
: _impl( new CxSocketImpl( domain_, type_, protocol_ ) )
{
}


//-------------------------------------------------------------------------
// CxSocket::CxSocket
//
//-------------------------------------------------------------------------
CxSocket::CxSocket( CxSocketImpl *impl_ )
: _impl( impl_ )
{
}

        
//-------------------------------------------------------------------------
// CxSocket::~CxSocket
//
//-------------------------------------------------------------------------
CxSocket::~CxSocket( void )  
{ 
}


//-------------------------------------------------------------------------
// CxSocket::CxSocket
//
//-------------------------------------------------------------------------
CxSocket::CxSocket( const CxSocket& sock_ )
{
    if (this != &sock_) {
        _impl = sock_._impl;
    }
}


//-------------------------------------------------------------------------
// CxSocket::operator=
//
//-------------------------------------------------------------------------
CxSocket&
CxSocket::operator=(const CxSocket& sock_)
{
    if (this != &sock_) {
        _impl = sock_._impl;
    }
    return(*this);
}


//-------------------------------------------------------------------------
// CxSocket::fd
//
//-------------------------------------------------------------------------
int 
CxSocket::fd( void )	
{ 
    return( _impl->fd( ) );
}

        
//-------------------------------------------------------------------------
// CxSocket::good
//
//-------------------------------------------------------------------------
int 
CxSocket::good( void )    
{ 
    return( _impl->good( ) );
}


//-------------------------------------------------------------------------
// CxSocket::bind
//
//-------------------------------------------------------------------------
int 
CxSocket::bind(  CxInetAddress addr_ )
{
    return( _impl->bind( addr_ ) );
}


//-------------------------------------------------------------------------
// CxSocket::listen
//
//-------------------------------------------------------------------------
int
CxSocket::listen( int backlog_ )
{
    return( _impl->listen( backlog_ ) );
}
        

//-------------------------------------------------------------------------
// CxSocket::accept
//
//-------------------------------------------------------------------------
CxSocket
CxSocket::accept ( void )
{
    CxSocketImpl *newSocketImpl = _impl->accept( );
    CxSocket newSocket( newSocketImpl );
    return( newSocket );
}


//-------------------------------------------------------------------------
// CxSocket::getpeername
//
//-------------------------------------------------------------------------
CxInetAddress
CxSocket::getpeername( void )
{
    return( _impl->getpeername( ) );
}


//-------------------------------------------------------------------------
// CxSocket::getpeername
//
//-------------------------------------------------------------------------
CxInetAddress
CxSocket::getsockname( void )
{
    return( _impl->getsockname( ) );
}


//-------------------------------------------------------------------------
// CxSocket::connect
//
//-------------------------------------------------------------------------       	
int 
CxSocket::connect( CxInetAddress addr_, int timeout_  )
{
    return( _impl->connect( addr_, timeout_ ) );
}


//-------------------------------------------------------------------------
// CxSocket::send
//
//-------------------------------------------------------------------------        
int 
CxSocket::send( char* buf, int len, unsigned int flag )
{
    return( _impl->send( buf, len, flag ) );
}


//-------------------------------------------------------------------------
// CxSocket::sendAtLeast
//
//-------------------------------------------------------------------------        
void
CxSocket::sendAtLeast( char* buf, int len, unsigned int flag ) 	
{
    _impl->sendAtLeast( buf, len, flag );
}


//-------------------------------------------------------------------------
// CxSocket::sendAtLeast
//
//-------------------------------------------------------------------------        
void
CxSocket::sendAtLeast( const CxString buf, unsigned int flag ) 	
{
    _impl->sendAtLeast( buf.data() , buf.length(), flag );
}


//-------------------------------------------------------------------------
// CxSocket::recv
//
//-------------------------------------------------------------------------        
int 
CxSocket::recv( char* buf, int len, unsigned int flag ) 	
{
    return( _impl->recv( buf, len, flag ) );
}


//-------------------------------------------------------------------------
// CxSocket::recv
//
//-------------------------------------------------------------------------        
CxString
CxSocket::recv( int len, unsigned int flag ) 	
{
    int returnLength = 0;
    char *buf = new char[ len+1 ];
    memset(buf, 0, len+1);

    try {
        returnLength = _impl->recv( buf, len, flag );
    }

    catch ( const CxSocketException&  ) {
    }

    buf[ returnLength ] = (char) NULL;

    CxString s = buf;

    delete[] buf;

    return( s );
}


//-------------------------------------------------------------------------
// CxSocket::recvAtLeast
//
//-------------------------------------------------------------------------        
void
CxSocket::recvAtLeast( char* buf, int len, unsigned int flag ) 	
{
    _impl->recvAtLeast( buf, len, flag );
}


//-------------------------------------------------------------------------
// CxSocket::recvUntil
//
//-------------------------------------------------------------------------        
CxString
CxSocket::recvUntil( const char c, unsigned int flag ) 	
{
   return(  _impl->recvUntil( c, flag ) );
}


//-------------------------------------------------------------------------
// CxSocket::recvAtLeast
//
//-------------------------------------------------------------------------        
CxString
CxSocket::recvAtLeast( int len, unsigned int flag ) 	
{
    char *buf = new char[ len+1 ];
    memset(buf, 0, len+1);

    try {
        _impl->recvAtLeast( buf, len, flag );
    }

    catch ( const CxSocketException&  ) {
    }

    return( CxString( buf ) );
}   



//-------------------------------------------------------------------------
// CxSocket::sendto
//
//-------------------------------------------------------------------------
int 
CxSocket::sendto( char* buf, int len, unsigned int flag, CxInetAddress addr_ )
{
    return( _impl->sendto( buf, len, flag, addr_) );
}


//-------------------------------------------------------------------------
// CxSocket::recvfrom
//
//-------------------------------------------------------------------------
int 
CxSocket::recvfrom( char* buf, int len, unsigned int flag, CxInetAddress* addr_ )
{
    return( _impl->recvfrom( buf, len, flag, addr_) );
}


//-------------------------------------------------------------------------
// CxSocket::shutdown
//
//-------------------------------------------------------------------------
int 
CxSocket::shutdown( int mode )
{
    return( _impl->shutdown( mode ) );
}


//-------------------------------------------------------------------------
// CxSocket::shutdownread
//
//-------------------------------------------------------------------------
int 
CxSocket::shutdownread( void )
{
    return( _impl->shutdownread( ) );
}


//-------------------------------------------------------------------------
// CxSocket::shutdownwrite
//
//-------------------------------------------------------------------------
int 
CxSocket::shutdownwrite( void )
{
    return( _impl->shutdownwrite( ) );
}


//-------------------------------------------------------------------------
// CxSocket::close
//
//-------------------------------------------------------------------------        
void
CxSocket::close( void )
{
    _impl->close( );
}


//-------------------------------------------------------------------------
// CxSocket::setNonBlocking
//
//-------------------------------------------------------------------------        
void
CxSocket::setNonBlocking( void )
{
	_impl->setNonBlocking( );
}

//-------------------------------------------------------------------------
// CxSocket::setBlocking
//
//-------------------------------------------------------------------------        
void
CxSocket::setBlocking( void )
{
	_impl->setBlocking( );
}



//-------------------------------------------------------------------------
// CxSocket::recvDataPending
//
//-------------------------------------------------------------------------        
int
CxSocket::recvDataPending( unsigned int sec_, unsigned int usec_ )
{
	return (_impl->recvDataPending( sec_, usec_ ));
}


//-------------------------------------------------------------------------
// CxSocket::select
//
//-------------------------------------------------------------------------        
int
CxSocket::select( unsigned int sec_, unsigned int usec_, unsigned int *flags_ )
{
	return (_impl->select( sec_, usec_, flags_ ));
}


//-------------------------------------------------------------------------
// CxSocket::initWinSock <static>
//
//-------------------------------------------------------------------------        
int
CxSocket::initWinSock( void )
{

#ifdef _LINUX_
    return(1);
#endif

#ifdef WIN32
    WSADATA data;
    const WORD versionRequested = 1 + 1*256;
    int err = WSAStartup(versionRequested, &data);
    if ( err != 0 ) { 
        return(0);
    }
    return(1);
#endif

    return( 1 );
}
