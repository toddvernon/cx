//-------------------------------------------------------------------------------------------------
//
//  mutex.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxMutex Class
//
//-------------------------------------------------------------------------------------------------

#include <cx/base/string.h>
#include <cx/thread/mutex.h>



//-------------------------------------------------------------------------
// CxMutex::CxMutex
//
//-------------------------------------------------------------------------
CxMutex::CxMutex( void )
{

#if defined(_LINUX_) || defined(_OSX_) || defined(_IRIX6_)
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
#if defined(_LINUX_) || defined(_OSX_) || defined(_IRIX6_)
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

#if defined(_LINUX_) || defined(_OSX_) || defined(_IRIX6_)
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

#if defined(_LINUX_) || defined(_OSX_) || defined(_IRIX6_)
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

#if defined(_LINUX_) || defined(_OSX_) || defined(_IRIX6_)
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
#if defined(_LINUX_) || defined(_OSX_) || defined(_IRIX6_)
pthread_mutex_t *
CxMutex::pthread_mutex(void)
{
	return( &_mutex );
}
#endif
