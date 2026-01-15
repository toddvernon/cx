#include "threadpool.h"

//-------------------------------------------------------------------------
// CxThreadPool::CxThreadPool
//
//-------------------------------------------------------------------------
CxThreadPool::CxThreadPool( size_t numThreads, size_t workQueueSize )
    : CxRunnableThread( workQueueSize ), 
      _threads( new CxRunnableThread[numThreads] ), 
      _numThreads( (int)numThreads )
{
    for ( int i = 0; i < _numThreads; i++ )
    {
        _threads[i].start();
    }
}


//-------------------------------------------------------------------------
// CxPCQueue::~CxThreadPool
//
//-------------------------------------------------------------------------
CxThreadPool::~CxThreadPool()
{
    delete[] _threads;
}

//-------------------------------------------------------------------------
// CxThreadPool::suggestQuit
//
//-------------------------------------------------------------------------
void
CxThreadPool::performQuit( )
{
    // Tell the worker threads to quit.

    for (int i = 0; i < _numThreads; i++ ) 
    {
        _threads[i].suggestQuit();
    }
}

//-------------------------------------------------------------------------
// CxThreadPool::performWork
//
//-------------------------------------------------------------------------
void
CxThreadPool::performWork( CxRunnable* pWorkItem )
{
    // Hand off the work request to the next available thread.

    int i = 0;
    for (; i < _numThreads; i++ ) 
    {
        if ( !_threads[i].isExecuting() ) 
        {
            _threads[i].enQueue( pWorkItem );
            break;
        }
    }

    // If we did not find a thread that was not executing, then sleep
    // for a short time to avoid spinning in the for loop until a thread
    // becomes available.

    if ( i == _numThreads ) 
    {
//        Sleep( 1000 );
    }
}


//-------------------------------------------------------------------------
// CxThreadPool::endOfRun()
//
//-------------------------------------------------------------------------
void
CxThreadPool::endOfRun( )
{
    // Wait for all threads to complete.
    for (int i = 0; i < _numThreads; i++ ) 
    {
        _threads[i].join();
    }
}
