//-------------------------------------------------------------------------------------------------
//
//  rmutex.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxRecursiveMutex Class
//
//-------------------------------------------------------------------------------------------------

#include <cx/base/string.h>
#include <cx/thread/rmutex.h>



//-------------------------------------------------------------------------
// CxRecursiveMutex::CxRecursiveMutex
//
//-------------------------------------------------------------------------
CxRecursiveMutex::CxRecursiveMutex( void )
{
#if defined(_LINUX_) || defined(_OSX_) || defined(_IRIX6_)
    _thCount = 0;
#endif

#ifdef WIN32
	
#endif

}


//-------------------------------------------------------------------------
// CxRecursiveMutex::~CxRecursiveMutex
//
//-------------------------------------------------------------------------
CxRecursiveMutex::~CxRecursiveMutex( void )
{

#if defined(_LINUX_) || defined(_OSX_) || defined(_IRIX6_)
#endif

#ifdef WIN32
#endif

}


//-------------------------------------------------------------------------
// CxRecursiveMutex::acquire
//
//-------------------------------------------------------------------------
void
CxRecursiveMutex::acquire( void )
{

#if defined(_LINUX_) || defined(_OSX_) || defined(_IRIX6_)

    CX_THREAD_TYPE mySelf = CX_THREAD_SELF;

    _guardMutex.acquire();

    if (_thCount && CX_THREAD_EQUAL( _thOwner, CX_THREAD_SELF ) ) {
            _thCount++;
            _guardMutex.release();
            return;
    }

    _guardMutex.release();
    _mutex.acquire();
    _guardMutex.acquire();

    _thOwner = mySelf;
    _thCount = 1;

    _guardMutex.release();

#endif


#ifdef WIN32
	_mutex.acquire();
#endif

}


//-------------------------------------------------------------------------
// CxRecursiveMutex::release
//
//-------------------------------------------------------------------------
void
CxRecursiveMutex::release( void )
{

#if defined(_LINUX_) || defined(_OSX_) || defined(_IRIX6_)

    CX_THREAD_TYPE mySelf = CX_THREAD_SELF;

    _guardMutex.acquire();

    if (_thCount == 0) {
        _guardMutex.release();
        throw CxMutexException( "CxRecursiveMutex::release():internal error 1" );
    }

    if (_thCount && CX_THREAD_NOT_EQUAL( _thOwner, mySelf ) ) {
        _guardMutex.release();
        throw CxMutexException( "CxRecursiveMutex::release():internal error 2" );
    }

    _guardMutex.release();
    _thCount--;

    if ( _thCount == 0 ) {
        _thOwner = CX_NO_THREAD;
        _mutex.release();
    }

#endif

#ifdef WIN32
	_mutex.release();
#endif

}
