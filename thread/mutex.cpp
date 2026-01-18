/***************************************************************************
 *
 *  mutex.cpp
 *
 *  CxMutex Class
 *
 ***************************************************************************
 *
 * (c) Copyright 1999 T.Vernon
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
 * adapted from:
 *   http://www.ucs.br.so/ch19.htm
 *
 **************************************************************************/


#include <cx/base/string.h>
#include <cx/thread/mutex.h>



//-------------------------------------------------------------------------
// CxMutex::CxMutex
//
//-------------------------------------------------------------------------
CxMutex::CxMutex( void )
{

#if defined(_LINUX_) || defined(_OSX_)
    pthread_mutex_init( &_mutex, 0 );
#endif

#ifdef WIN32
    _mutex = CreateMutex( NULL, FALSE, NULL );
#endif

}


//-------------------------------------------------------------------------
// CxMutex::~CxMutex
//
//-------------------------------------------------------------------------
CxMutex::~CxMutex( void )  
{
#if defined(_LINUX_) || defined(_OSX_)
    pthread_mutex_destroy( &_mutex );
#endif

#ifdef WIN32
#endif

}


//-------------------------------------------------------------------------
// CxMutex::acquire
//
//-------------------------------------------------------------------------
void
CxMutex::acquire( void )
{

#if defined(_LINUX_) || defined(_OSX_)
    pthread_mutex_lock( &_mutex );
#endif

#ifdef WIN32
    WaitForSingleObject( _mutex, INFINITE );
#endif
}


//-------------------------------------------------------------------------
// CxMutex::tryAcquire
//
//-------------------------------------------------------------------------
CxMutex::tryResult
CxMutex::tryAcquire( void )
{

#if defined(_LINUX_) || defined(_OSX_)
    if (pthread_mutex_trylock( &_mutex )==EBUSY) {
		return( CxMutex::MUTEX_BUSY );
    }
#endif

#ifdef WIN32              
    WaitForSingleObject( _mutex, INFINITE );

#endif

    return( CxMutex::MUTEX_ACQUIRED );
}


//-------------------------------------------------------------------------
// CxMutex::release
//
//-------------------------------------------------------------------------
void
CxMutex::release( void )
{

#if defined(_LINUX_) || defined(_OSX_)
    pthread_mutex_unlock( &_mutex );
#endif

#ifdef WIN32
    ReleaseMutex( _mutex );
#endif

}


//-------------------------------------------------------------------------
// CxMutex::pthread_mutex
//
//-------------------------------------------------------------------------
#if defined(_LINUX_) || defined(_OSX_)
pthread_mutex_t *
CxMutex::pthread_mutex(void)
{
	return( &_mutex );
}
#endif
