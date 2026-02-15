//-------------------------------------------------------------------------------------------------
//
//  thread2.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxThread Class
//
//-------------------------------------------------------------------------------------------------

#include <cx/base/string.h>
#include <cx/thread/thread2.h>


//-------------------------------------------------------------------------
// CxThreadDuple::CxThreadDuple
//
//-------------------------------------------------------------------------
CxThreadDuple::CxThreadDuple( void )
:_suggestQuit( FALSE )
{
}


//-------------------------------------------------------------------------
// CxThreadDuple::~CxThreadDuple
//
//-------------------------------------------------------------------------
CxThreadDuple::~CxThreadDuple( void )  
{ 
}


//-------------------------------------------------------------------------
// CxThreadDuple::start
//
//-------------------------------------------------------------------------
int
CxThreadDuple::start( void )
{
#if defined(_LINUX_) || defined(_OSX_)
    pthread_create( &_threadId1, NULL, CxThreadDuple::srun1, (void*) this);
    pthread_create( &_threadId2, NULL, CxThreadDuple::srun2, (void*) this);
#endif
    return 0;
}


//-------------------------------------------------------------------------
// CxThreadDuple::join
//
//-------------------------------------------------------------------------
void
CxThreadDuple::join( void )
{
#if defined(_LINUX_) || defined(_OSX_)
    pthread_join( _threadId1, NULL );
    pthread_join( _threadId2, NULL );
#endif
}


//-------------------------------------------------------------------------
// CxThreadDuple::srun1
//
//-------------------------------------------------------------------------
void *
CxThreadDuple::srun1( void *impl )
{
    CxThreadDuple *threadImpl = (CxThreadDuple*) impl;
    threadImpl->resetSuggestQuit();
    threadImpl->run1();
    return( 0 );
}

//-------------------------------------------------------------------------
// CxThreadDuple::srun2
//
//-------------------------------------------------------------------------
void *
CxThreadDuple::srun2( void *impl )
{
    CxThreadDuple *threadImpl = (CxThreadDuple*) impl;
    threadImpl->resetSuggestQuit();
    threadImpl->run2();
    return( 0 );
}


//-------------------------------------------------------------------------
// CxThreadDuple::run1
//
//-------------------------------------------------------------------------
void
CxThreadDuple::run1( void )
{
}

//-------------------------------------------------------------------------
// CxThreadDuple::run2
//
//-------------------------------------------------------------------------
void
CxThreadDuple::run2( void )
{
}


//-------------------------------------------------------------------------
// CxThreadDuple::resetSuggestQuit
//
//-------------------------------------------------------------------------
void
CxThreadDuple::resetSuggestQuit( void )
{
    _suggestQuit = FALSE;
}


//-------------------------------------------------------------------------
// CxThreadDuple::suggestQuit
//
//-------------------------------------------------------------------------
void
CxThreadDuple::suggestQuit( void )
{
    _suggestQuit = TRUE;
}


//-------------------------------------------------------------------------
// CxThreadDuple::sendSignal1
//
//-------------------------------------------------------------------------
void
CxThreadDuple::sendSignal1( int sig_ )
{
#if defined(_LINUX_) || defined(_OSX_)
	pthread_kill( _threadId1, sig_ );
#endif
}

//-------------------------------------------------------------------------
// CxThreadDuple::sendSignal2
//
//-------------------------------------------------------------------------
void
CxThreadDuple::sendSignal2( int sig_ )
{
#if defined(_LINUX_) || defined(_OSX_)
	pthread_kill( _threadId2, sig_ );
#endif
}
