/***************************************************************************
 *
 *  socketi.cpp
 *
 *  CxSocketImpl Class
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

#include <stdlib.h>
#include <cx/base/string.h>
#include <cx/net/socketi.h>

#include <net/if.h>

//-------------------------------------------------------------------------
// CxSocketImpl::CxSocketImpl
//
//-------------------------------------------------------------------------
CxSocketImpl::CxSocketImpl( int domain_, int type_, int protocol_ ) 
{
    if ( domain_ == -1 ) {
        _sfd = type_;
    } else {
        _sfd = ::socket(domain_, type_, protocol_);
    }
}

        
//-------------------------------------------------------------------------
// CxSocketImpl::~CxSocketImpl
//
//-------------------------------------------------------------------------
CxSocketImpl::~CxSocketImpl( void )  
{
    try {
        CxSocketImpl::close( );
    } 
    catch( ... ) {
    }
}


//-------------------------------------------------------------------------
// CxSocketImpl::fd
//
//-------------------------------------------------------------------------
int 
CxSocketImpl::fd( void )	
{ 
    return _sfd; 
}

        
//-------------------------------------------------------------------------
// CxSocketImpl::good
//
//-------------------------------------------------------------------------
int 
CxSocketImpl::good( void )    
{ 
    return (_sfd >= 0);  
}


//-------------------------------------------------------------------------
// CxSocketImpl::bind
//
//-------------------------------------------------------------------------
int 
CxSocketImpl::bind(  CxInetAddress addr_ )
{	
    int rc = ::bind( _sfd, (struct sockaddr *) addr_.sockAddr(), addr_.sockAddrSize() );

    if ( rc < 0) {
        throw( CxSocketException( errno, CxError::buildOSErrorString( "CxSocketImpl::bind()" )) );
    }

    return( rc );
}


//-------------------------------------------------------------------------
// CxSocketImpl::listen
//
//-------------------------------------------------------------------------
int
CxSocketImpl::listen( int backlog_ )
{
    int rc = ::listen( _sfd, backlog_ );

    if (rc < 0) {
        throw( CxSocketException( errno, CxError::buildOSErrorString( "CxSocketImpl::listen()" )) );
    }

    return( rc );
}


//-------------------------------------------------------------------------
// CxSocketImpl::listen
//
//-------------------------------------------------------------------------
int
CxSocketImpl::select( unsigned int sec_, unsigned int usec_, unsigned int *flags)
{
    struct timeval tval;
    fd_set read_map;
    fd_set write_map;
    fd_set except_map;

    FD_ZERO( &read_map );
    FD_SET( _sfd, &read_map );

    FD_ZERO( &write_map );
    FD_SET( _sfd, &write_map );

    FD_ZERO( &except_map );
    FD_SET( _sfd, &except_map );

    tval.tv_sec = sec_;
    tval.tv_usec = usec_;

    int i = ::select( _sfd+1, &read_map, &write_map, &except_map, &tval );
    if (i) {

        if (FD_ISSET( _sfd, &read_map)) {
            *flags |= 1;
        }

        if (FD_ISSET( _sfd, &write_map)) {
            *flags |= 2;
        }

        if (FD_ISSET( _sfd, &except_map)) {
            *flags |= 4;
        } 

        return(1);
    }
    return(0);
}


//-------------------------------------------------------------------------
// CxSocketImpl::recvDataPending
//
//-------------------------------------------------------------------------
int
CxSocketImpl::recvDataPending( unsigned int sec, unsigned int usec )
{
    struct timeval tval;
    fd_set read_map;

    FD_ZERO( &read_map );
    FD_SET( _sfd, &read_map );

    tval.tv_sec = sec;
    tval.tv_usec = usec;

    int i = ::select( _sfd+1, &read_map, NULL, NULL, &tval );
    if (i) {
        return(1);
    }
    return(0);
}
         

//-------------------------------------------------------------------------
// CxSocketImpl::accept
//
//-------------------------------------------------------------------------
CxSocketImpl *
CxSocketImpl::accept ( void )
{
    struct sockaddr_in addr;
	    
    SOCKLEN_T size = sizeof (addr);
              
    int rc = ::accept( _sfd, (struct sockaddr*) &addr, &size );

    if ( rc < 0) {
        throw( CxSocketException( errno, CxError::buildOSErrorString( "CxSocketImpl::accept()" )) );
    }

    CxSocketImpl *newSocketImpl = new CxSocketImpl( -1, rc );

    return( newSocketImpl );
}


//-------------------------------------------------------------------------
// CxSocketImpl::getpeername
//
//-------------------------------------------------------------------------
CxInetAddress
CxSocketImpl::getpeername( void )
{
    struct sockaddr_in addr;
	    
    SOCKLEN_T size = sizeof (addr);
             
    int rc = ::getpeername( _sfd, (struct sockaddr*) &addr, &size );

    if ( rc < 0) {
        throw( CxSocketException( errno, CxError::buildOSErrorString( "CxSocketImpl::getpeername()" )) );
    }

    CxInetAddress address( &addr );

    return( address );
};


//-------------------------------------------------------------------------
// CxSocketImpl::getsockname
//
//-------------------------------------------------------------------------
CxInetAddress
CxSocketImpl::getsockname( void )
{
    struct sockaddr_in addr;
	    
    SOCKLEN_T size = sizeof (addr);
             
    int rc = ::getsockname( _sfd, (struct sockaddr*) &addr, &size );

    CxInetAddress address( &addr );

    return( address );
};


//-------------------------------------------------------------------------
// CxSocketImpl::setNonBlocking
//
//-------------------------------------------------------------------------       	
void
CxSocketImpl::setNonBlocking( void )
{

#if defined(_LINUX_) || defined(_SOLARIS6_) || defined(_SOLARIS10_) || defined(_NETBSD_)	
    int flags = fcntl( _sfd, F_GETFL, 0);
    fcntl( _sfd, F_SETFL, flags | O_NONBLOCK);    
#endif

}

//-------------------------------------------------------------------------
// CxSocketImpl::setNonBlocking
//
//-------------------------------------------------------------------------       	
void
CxSocketImpl::setBlocking( void )
{

#if defined(_LINUX_) || defined(_SOLARIS6_) || defined(_SOLARIS10_) || defined(_NETBSD_)	
    int flags = fcntl( _sfd, F_GETFL, 0);
    fcntl( _sfd, F_SETFL, flags |~ O_NONBLOCK);    
#endif

}



//-------------------------------------------------------------------------
// CxSocketImpl::connect
//
//-------------------------------------------------------------------------       	
int 
CxSocketImpl::connect( CxInetAddress addr_, int timeout_  )
{
    int        readable = FALSE;
    int        writeable = FALSE;
    int        rc;
    
#if defined(_LINUX_) || defined(_SOLARIS6_) || defined(_SOLARIS10_) || defined(_NETBSD_)	

	int        n,error;
    SOCKLEN_T  len;
    int        flags;   
	fd_set     rset,wset;
	struct     timeval tval;
    CxTime     now;

    if (timeout_) {
        flags = fcntl( _sfd, F_GETFL, 0);
        fcntl( _sfd, F_SETFL, flags | O_NONBLOCK);
    }

#endif

	rc = ::connect( _sfd, (struct sockaddr*) addr_.sockAddr(), (int) addr_.sockAddrSize() );

	if ( rc < 0 ) {

#if defined(_LINUX_) || defined(_SOLARIS6_) || defined(_SOLARIS10_) || defined(_NETBSD_)
        // if the non blocking flag is set then inprogress error is not an error
        if (timeout_) {

            if ( errno != EINPROGRESS ) {
                throw( CxSocketException( errno, CxError::buildOSErrorString( "CxSocketImpl::connect()" )) );
            }

        } else {               
            throw( CxSocketException( errno, CxError::buildOSErrorString( "CxSocketImpl::connect()" )) );
        }
#else
        throw( CxSocketException( errno, CxError::buildOSErrorString( "CxSocketImpl::connect()" )) );
#endif

	}

#if defined(_LINUX_) || defined(_SOLARIS6_) || defined(_SOLARIS10_) || defined(_NETBSD_)	

    if ( timeout_ ) {

        if (rc != 0) {
 
            FD_ZERO(&rset);
            FD_ZERO(&wset);

            FD_SET( _sfd, &rset);
  
            wset = rset;
   
            tval.tv_sec  = timeout_;
            tval.tv_usec = 0;
    
            if (( n=::select( _sfd + 1, &rset, &wset, NULL, timeout_ ? &tval : NULL )) == 0) {
    
                ::close( _sfd );
   
                errno = ETIMEDOUT;
                throw( CxSocketException( ETIMEDOUT, CxError::buildOSErrorString( "CxSocketImpl::connect()" )) );              
            }


            if (FD_ISSET( _sfd, &rset)) readable=TRUE;
            if (FD_ISSET( _sfd, &wset)) writeable=TRUE;

    
            if ( readable || writeable ) {

                len = sizeof(error);

                if (getsockopt( _sfd, SOL_SOCKET, SO_ERROR, (char*) &error, &len) < 0) {
    
                    ::close( _sfd );
    
                    throw( CxSocketException( errno, CxError::buildOSErrorString( "CxSocketImpl::connect()" )) );
                }

           } else {
    
                throw( CxSocketException( errno, CxError::buildOSErrorString( "CxSocketImpl::connect()" )) );

           }
        }
    
        fcntl( _sfd, F_SETFL, flags);
        rc = 0;
    }
#endif

    return ( rc );

}





//-------------------------------------------------------------------------
// CxSocketImpl::send
//
//-------------------------------------------------------------------------        
int 
CxSocketImpl::send( char* buf, int len, unsigned int flag )
{
    int rc = ::send( _sfd, buf, len, flag );

    if (rc==-1) {
        throw( CxSocketException( errno, CxError::buildOSErrorString( "CxSocketImpl::sendto()" )) );
    }

    return( rc );
}


//-------------------------------------------------------------------------
// CxSocketImpl::sendAtLeast
//
//-------------------------------------------------------------------------
void
CxSocketImpl::sendAtLeast( char* buf, int len, unsigned int flag )
{
	int currentOffset   = 0;
	int bytesLeftToSend = len;

	while ( bytesLeftToSend ) {

		int bytes = send( &(buf[ currentOffset ]), bytesLeftToSend, flag );

		bytesLeftToSend -= bytes;
		currentOffset   += bytes;
  	}
}


//-------------------------------------------------------------------------
// CxSocketImpl::recv
//
//-------------------------------------------------------------------------        
int 
CxSocketImpl::recv( char* buf, int len, unsigned int flag ) 	
{
    int count = ::recv( _sfd, buf, len, flag );

    if (count==-1) {
        throw( CxSocketException( errno, CxError::buildOSErrorString( "CxSocketImpl::recv()" )) );
    }
    return count;
}


//-------------------------------------------------------------------------
// CxSocketImpl::recvAtLeast
//
//-------------------------------------------------------------------------
void
CxSocketImpl::recvAtLeast( char* buf, int len, unsigned int flag )
{
	int recvBytes      = 0;
	int bytesLeftToGet = len;

	while ( recvBytes < len ) {

		int bytes = recv( &(buf[recvBytes]), bytesLeftToGet, flag );

		bytesLeftToGet -= bytes;
		recvBytes      += bytes;

		if ((bytes == 0) && ( bytesLeftToGet > 0)) {

            buf[ recvBytes ] = (char) NULL;
			throw( CxSocketException( 0, "CxSocketImpl::recvAtLeast():returned end of stream" ));			
		}
  	}
}


//-------------------------------------------------------------------------
// CxSocketImpl::recvUntil
//
//-------------------------------------------------------------------------
CxString
CxSocketImpl::recvUntil( char c, unsigned int flag )
{
    char buf[2];
    buf[1] = (char) 0;

    CxString line;

	while ( 1 ) {

		int bytes = recv( &(buf[0]), 1, flag );

        if (bytes) {
            line += buf;
        } else {
            throw( CxSocketException( 0, "CxSocketImpl::recvUntil():returned end of stream" ));			
        }
    
        if ( buf[0] == c ) {
            return( line );
        }
    }
}


//-------------------------------------------------------------------------
// CxSocketImpl::sendto
//
//-------------------------------------------------------------------------
int 
CxSocketImpl::sendto( char* buf, int len, unsigned int flag, CxInetAddress addr_ )
{
    int rc = ::sendto( _sfd, buf, len, flag, 
		(struct sockaddr*) addr_.sockAddr(), addr_.sockAddrSize() );

    if (rc==-1) {
        throw( CxSocketException( errno, CxError::buildOSErrorString( "CxSocketImpl::sendto()" )) );
    }

	return( rc );
}


//-------------------------------------------------------------------------
// CxSocketImpl::recvfrom
//
//
//-------------------------------------------------------------------------
int 
CxSocketImpl::recvfrom( char* buf, int len, unsigned int flag, CxInetAddress* addr_ )
{
	int rc = 0;

  	struct sockaddr_in addr;
	SOCKLEN_T size = sizeof (addr);            
  
	if ( addr_ ) {

		rc = ::recvfrom(_sfd, buf, len, flag, (struct sockaddr*) &addr, &size);

		if ( rc > -1) {		
			*addr_ = CxInetAddress( &addr ); 
    	} else {
            throw( CxSocketException( errno, CxError::buildOSErrorString( "CxSocketImpl::recvfrom()" )) );
		}

	} else {
	
		rc = ::recvfrom(_sfd, buf, len, flag, (struct sockaddr*) 0, 0);
		if (rc < 0) {
            throw( CxSocketException( errno, CxError::buildOSErrorString( "CxSocketImpl::recvfrom()" )) );
		}
	}

    return rc;
}


//-------------------------------------------------------------------------
// CxSocketImpl::shutdown
//
//
//-------------------------------------------------------------------------
int 
CxSocketImpl::shutdown( int mode )
{
    int rc = ::shutdown ( _sfd, mode );

    if (rc==-1) {

        if (errno != CxSOCKET_NOT_CONNECTED ) {
            throw( CxSocketException( errno,  CxError::buildOSErrorString( "CxSocketImpl::shutdown()" )) );
        }
    }

    return rc;
}


//-------------------------------------------------------------------------
// CxSocketImpl::shutdownread
//
//-------------------------------------------------------------------------
int 
CxSocketImpl::shutdownread( void )
{
    int rc = ::shutdown ( _sfd, 0 );

    if (rc==-1) {
        if (errno != CxSOCKET_NOT_CONNECTED ) {
            throw( CxSocketException( errno, CxError::buildOSErrorString( "CxSocketImpl::shutdownread()" )) );
        }
    }

    return rc;
}


//-------------------------------------------------------------------------
// CxSocketImpl::shutdownwrite
//
//-------------------------------------------------------------------------
int 
CxSocketImpl::shutdownwrite( void )
{
    int rc = ::shutdown ( _sfd, 1);

    if (rc==-1) {
        if (errno != CxSOCKET_NOT_CONNECTED ) {
            throw( CxSocketException( errno, CxError::buildOSErrorString( "CxSocketImpl::shutdownwrite()" )) );
        }
    }

    return rc;
}


//-------------------------------------------------------------------------
// CxSocketImpl::close
//
//-------------------------------------------------------------------------        
void
CxSocketImpl::close( void )
{
    ::CLOSESOCKET_MACRO( _sfd );
}
