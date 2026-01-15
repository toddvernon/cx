/***************************************************************************
 *
 *  thread.cpp
 *
 *  CxThread Class
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
 *
 **************************************************************************/


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
#if defined(_LINUX_) || defined(_OSX_)
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
#if defined(_LINUX_) || defined(_OSX_)
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
#if defined(_LINUX_) || defined(_OSX_)
	pthread_kill( _threadId, sig_ );
#endif
}
