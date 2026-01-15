#ifndef _CxRunnableThread_h_
#define _CxRunnableThread_h_

#include "thread.h"
#include "runnable.h"
#include "quitrequest.h"
#include "pc.h"
#include "rmutex.h"


//-------------------------------------------------------------------------
// class CxRunnableThreadEnqueueException
//
//-------------------------------------------------------------------------
class CxRunnableThreadEnqueueException: public CxException
{
  public:
      CxRunnableThreadEnqueueException( CxString s ) : CxException( s ) { }
};

class CxRunnableThread : public CxThread
{
public:
     	
    CxRunnableThread( size_t queueSize = 1 );
    // Constructor
        
    ~CxRunnableThread( void );
    // Destructor

    virtual void run( void );
    // the run method
	
	void enQueue( CxRunnable* pItem, time_t sec_=0 );
    // Add the item to the queue.
	
    virtual void suggestQuit( void );
    // Tell the thread to quit

    virtual void performQuit( void ) { };
    // Do any additional quit processing that might be required. 

    virtual void performWork( CxRunnable* pItem );
    // Do any additional quit processing that might be required. 

    virtual void endOfRun( ) { };
    // Do any final processing before returning from the run() method.

    int isExecuting( );

protected:
	CxPCQueue<CxRunnable*> _workQueue;
    int _executing;
    int _quitRequested;
    CxRecursiveMutex _enQueueMutex;
};


#endif
