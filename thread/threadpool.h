#ifndef _CxThreadPool_h_
#define _CxThreadPool_h_

// This class provides a pool of threads that pull runnable objects from a
// producer/consumer queue and execute them.

#include "pc.h"
#include "thread.h"
#include "runnablethread.h"

class CxThreadPool : public CxRunnableThread
{
public:
	CxThreadPool( size_t numThreads = 20, size_t workQueueSize = 100 );
	~CxThreadPool();

    virtual void performWork( CxRunnable* pItem );
    // Farm out the input CxRunnable to the first available thread.

    virtual void performQuit( void );
    // Extra quit processing (tell the threads to quit)

    virtual void endOfRun( void );
    // Extra processing at the end of the run() method (join the threads)


protected:
    CxRunnableThread*   _threads;
    int                 _numThreads;
};

#endif
