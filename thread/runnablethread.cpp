//-------------------------------------------------------------------------------------------------
//
//  runnablethread.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  runnablethread.cpp
//
//-------------------------------------------------------------------------------------------------

#include "runnablethread.h"


//-------------------------------------------------------------------------
// CxRunnableThread::CxRunnableThread
//
//-------------------------------------------------------------------------
CxRunnableThread::CxRunnableThread( size_t queueSize ) 
    : CxThread( ), 
      _executing( 0 ), 
      _workQueue( queueSize ), 
      _quitRequested( 0 )
{
}


//-------------------------------------------------------------------------
// CxRunnableThread::~CxRunnableThread
//
//-------------------------------------------------------------------------
CxRunnableThread::~CxRunnableThread( void )  
{ 
}


//-------------------------------------------------------------------------
// CxRunnableThread::run
//
//-------------------------------------------------------------------------
void
CxRunnableThread::run( void )
{
    // Perform work until we have been told to quit.

    while ( !_suggestQuit ) {

        // Get the next peice of work from the queue, and do the work.
        // Note that this call will block indefinitely until something
        // new shows up on the work queue. A QuitRequest added to the
        // queue is used to wake up the thread and tell it to quit.

        CxRunnable* pWorkItem = _workQueue.deQueue();

        // Set the flag that indicates that the thread is executing, 
        // so that it will not be used for other requests.

        _executing = 1;

        // If this is a not a CxQuitRequest, then let the thread process the
        // CxRunnable.

        if ( !pWorkItem->isQuitRequest() ) {
            performWork( pWorkItem );
        }
        else {
            // Do the final quit processing.

            _suggestQuit = 1;
            performQuit();
            delete pWorkItem;
        }

        // Indicate that we are done processing this request.

        _executing = 0;
    }

    // Do any final processing before returning from the run method.

    endOfRun();
}

void
CxRunnableThread::performWork( CxRunnable* pWorkItem )
{
    // The default behavior is to simply call the Runnable's run method.
    // Note derived classes can override this and do whatever they need to do.

    pWorkItem->run();

    // Note that ownership of the CxRunnable is passed into the performWork()
    // method, so all derived classes should handle deleting it when they are
    // done with it.

    delete pWorkItem;
}

//-------------------------------------------------------------------------
// CxRunnableThread::enQueue
//
//-------------------------------------------------------------------------
void
CxRunnableThread::enQueue( CxRunnable* pWorkItem, time_t sec )
{
    // If the quitRequest has been made, don't allow any more
    // requests to be added.

    if ( !_quitRequested )
    {
        _workQueue.enQueue( pWorkItem, sec );
    }
    else
    {			
        throw CxRunnableThreadEnqueueException( "Runnable Thread cannot accept requests after suggestQuit() has been called");
    }
}


//-------------------------------------------------------------------------
// CxRunnableThread::isExecuting
//
//-------------------------------------------------------------------------
int
CxRunnableThread::isExecuting( )
{
    return _executing;
}


//-------------------------------------------------------------------------
// CxRunnableThread::suggestQuit
//
//-------------------------------------------------------------------------
void
CxRunnableThread::suggestQuit( )
{
    // Set a flag which stops the public enQueue method from taking any more
    // requests.

    _quitRequested = 1;

    // Put the special QuitRequest method on the work queue. This wakes up
    // the thread and tells it to quit. Note that we are not using the public
    // enQueue method here because we do not want this request to be denied.

    _workQueue.enQueue( new CxQuitRequest() );
}
