#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>
#include <errno.h>

#include <cx/base/string.h>


#if defined(_LINUX_) || defined(_OSX_)
#include <pthread.h>
#endif


#if defined(WIN32)
#include <windows.h>
#endif


#include <cx/thread/mutex.h>
    

#ifndef _CxCOND_H_
#define _CxCOND_H_



class CxCondition
{
     
  public:
     
	enum waitResult { CONDITION_SIGNAL, CONDITION_TIMEOUT };
	
    CxCondition( void );
    // Constructor
        
    ~CxCondition( void );
    // Destructor

	waitResult timedWait( CxMutex *mutex, time_t sec_ );
	// wait until condition for until timeout

    void wait( CxMutex *mutex_ );
    // wait on condition
   
    void signal( void );
    // 

  private:

#if defined(_LINUX_) || defined(_OSX_)
    pthread_cond_t _condition;
#endif

#if defined(WIN32)

#endif

};


#endif
