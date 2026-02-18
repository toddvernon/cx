//-------------------------------------------------------------------------------------------------
//
//  thread.cpp
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
#include <cx/thread/thread.h>


//-------------------------------------------------------------------------
// CxThread::CxThread
//
//-------------------------------------------------------------------------
CxThread::CxThread( void )
:_suggestQuit( FALSE )
{
}


//-------------------------------------------------------------------------
// CxThread::~CxThread
//
//-------------------------------------------------------------------------
CxThread::~CxThread( void )  
{ 
}


//-------------------------------------------------------------------------
// CxThread::start
//
//-------------------------------------------------------------------------
int
CxThread::start( void )
{
#if defined(_LINUX_) || defined(_OSX_) || defined(_IRIX6_)
    pthread_create( &_threadId, NULL, CxThread::srun, (void*) this);
#endif
    return 0;
}


//-------------------------------------------------------------------------
// CxThread::join
//
//-------------------------------------------------------------------------
void
CxThread::join( void )
{
#if defined(_LINUX_) || defined(_OSX_) || defined(_IRIX6_)
    pthread_join( _threadId, NULL );
#endif
}


//-------------------------------------------------------------------------
// CxThread::srun
//
//-------------------------------------------------------------------------
void *
CxThread::srun( void *impl )
{
    CxThread *threadImpl = (CxThread*) impl;
    threadImpl->resetSuggestQuit();
    threadImpl->run();
    return( 0 );
}


//-------------------------------------------------------------------------
// CxThread::run
//
//-------------------------------------------------------------------------
void
CxThread::run( void )
{
}


//-------------------------------------------------------------------------
// CxThread::resetSuggestQuit
//
//-------------------------------------------------------------------------
void
CxThread::resetSuggestQuit( void )
{
    _suggestQuit = FALSE;
}


//-------------------------------------------------------------------------
// CxThread::suggestQuit
//
//-------------------------------------------------------------------------
void
CxThread::suggestQuit( void )
{
    _suggestQuit = TRUE;
}


//-------------------------------------------------------------------------
// CxThread::sendKill
//
//-------------------------------------------------------------------------
void
CxThread::sendSignal( int sig_ )
{
#if defined(_LINUX_) || defined(_OSX_) || defined(_IRIX6_)
	pthread_kill( _threadId, sig_ );
#endif
}
