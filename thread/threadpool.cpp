//-------------------------------------------------------------------------------------------------
//
//  threadpool.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  threadpool.cpp
//
//-------------------------------------------------------------------------------------------------

#include "threadpool.h"


//-------------------------------------------------------------------------
// CxThreadPool::Worker - Inner class
//
// Worker threads that pull work items from the shared queue and execute them.
// Each worker loops until it receives a CxQuitRequest.
//-------------------------------------------------------------------------
class CxThreadPool::Worker : public CxThread
{
public:
    CxPCQueue<CxRunnable*>* _queue;

    Worker() : _queue(0) {}

    void setQueue( CxPCQueue<CxRunnable*>* q ) { _queue = q; }

    virtual void run()
    {
        while (1)
        {
            CxRunnable* item = _queue->deQueue();

            if ( item->isQuitRequest() )
            {
                delete item;
                break;
            }

            item->run();
            delete item;
        }
    }
};


//-------------------------------------------------------------------------
// CxThreadPool::CxThreadPool
//
// Create worker threads but don't start them. Workers are started lazily
// when start() is called.
//-------------------------------------------------------------------------
CxThreadPool::CxThreadPool( size_t numThreads, size_t queueSize )
    : _workQueue( queueSize ),
      _workers( new Worker[numThreads] ),
      _numWorkers( (int)numThreads ),
      _started( 0 ),
      _quitRequested( 0 )
{
    // Point all workers at the shared queue
    for ( int i = 0; i < _numWorkers; i++ )
    {
        _workers[i].setQueue( &_workQueue );
    }
}


//-------------------------------------------------------------------------
// CxThreadPool::~CxThreadPool
//
//-------------------------------------------------------------------------
CxThreadPool::~CxThreadPool()
{
    delete[] _workers;
}


//-------------------------------------------------------------------------
// CxThreadPool::start
//
// Start all worker threads. They will begin pulling work from the shared
// queue immediately.
//-------------------------------------------------------------------------
void
CxThreadPool::start()
{
    if ( !_started )
    {
        _started = 1;
        for ( int i = 0; i < _numWorkers; i++ )
        {
            _workers[i].start();
        }
    }
}


//-------------------------------------------------------------------------
// CxThreadPool::enQueue
//
// Add a work item to the shared queue. All workers compete to dequeue items.
// Throws CxThreadPoolEnqueueException if the pool is shutting down.
//-------------------------------------------------------------------------
void
CxThreadPool::enQueue( CxRunnable* pItem, time_t sec )
{
    if ( _quitRequested )
    {
        throw CxThreadPoolEnqueueException( "Cannot enqueue: pool is shutting down" );
    }
    _workQueue.enQueue( pItem, sec );
}


//-------------------------------------------------------------------------
// CxThreadPool::suggestQuit
//
// Signal shutdown by enqueuing one CxQuitRequest per worker. Each worker
// will process work items until it dequeues its quit request, then exit.
// This ensures all pending work is completed before shutdown.
//-------------------------------------------------------------------------
void
CxThreadPool::suggestQuit()
{
    _quitRequested = 1;

    // Enqueue one quit request per worker
    for ( int i = 0; i < _numWorkers; i++ )
    {
        _workQueue.enQueue( new CxQuitRequest() );
    }
}


//-------------------------------------------------------------------------
// CxThreadPool::join
//
// Wait for all worker threads to complete.
//-------------------------------------------------------------------------
void
CxThreadPool::join()
{
    for ( int i = 0; i < _numWorkers; i++ )
    {
        _workers[i].join();
    }
}
