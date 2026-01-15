/***************************************************************************
 *
 *  rmutex.cpp
 *
 *  CxRecursiveMutex Class
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
#include <cx/thread/rmutex.h>



//-------------------------------------------------------------------------
// CxRecursiveMutex::CxRecursiveMutex
//
//-------------------------------------------------------------------------
CxRecursiveMutex::CxRecursiveMutex( void )
{
#ifdef _LINUX_
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

#ifdef _LINUX_
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

#ifdef _LINUX_

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

#ifdef _LINUX_

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
