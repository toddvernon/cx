#ifndef _CxThreadPool_h_
#define _CxThreadPool_h_

// This class provides a pool of threads that pull runnable objects from a
// shared producer/consumer queue and execute them.
//
// Usage:
//   CxThreadPool pool(4, 100);  // 4 workers, queue size 100
//   pool.start();               // Start worker threads
//   pool.enQueue(new MyWork()); // Add work items
//   pool.suggestQuit();         // Signal shutdown
//   pool.join();                // Wait for completion

#include "pc.h"
#include "thread.h"
#include "runnable.h"
#include "quitrequest.h"
#include <cx/base/exception.h>


//-------------------------------------------------------------------------
// class CxThreadPoolEnqueueException
//
//-------------------------------------------------------------------------
class CxThreadPoolEnqueueException: public CxException
{
  public:
      CxThreadPoolEnqueueException( CxString s ) : CxException( s ) { }
};


class CxThreadPool
{
public:
    CxThreadPool( size_t numThreads = 4, size_t queueSize = 100 );
    ~CxThreadPool();

    void start();
    // Start all worker threads. Workers begin pulling from the shared queue.

    void enQueue( CxRunnable* pItem, time_t sec = 0 );
    // Add work item to the shared queue. Blocks if queue is full (unless timeout).
    // Throws CxThreadPoolEnqueueException if pool is shutting down.

    void suggestQuit();
    // Signal shutdown by enqueuing one CxQuitRequest per worker.
    // Each worker will exit after processing its quit request.

    void join();
    // Wait for all worker threads to complete.


protected:
    class Worker;                      // Forward declare inner class

    CxPCQueue<CxRunnable*> _workQueue; // Shared queue - all workers pull from this
    Worker*                _workers;   // Array of worker threads
    int                    _numWorkers;
    int                    _started;
    int                    _quitRequested;
};

#endif
