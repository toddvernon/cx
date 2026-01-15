
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <sys/types.h>

#include <cx/base/string.h>


#if defined(WIN32)
#include <windows.h>
#endif


#if defined(_LINUX_) || defined(_SOLARIS_) || defined(_OSX_) || defined(_NETBSD_)
#include <pthread.h>
#include <signal.h> //UBUNTU
#endif



#ifndef _CxTHREAD_H_
#define _CxTHREAD_H_



class CxThread
{
     
  public:
     	
    CxThread( void );
    // Constructor
        
    ~CxThread( void );
    // Destructor

    int start( void );
    // do any lookups

    virtual void run( void );
    // the run method

    static void* srun( void *i );
    // startic thread run function

    void join( void );
    // join with thread

    void suggestQuit( void );
    // set the thread quit flag

    void resetSuggestQuit( void );
    // set the thread quit flag

    void sendSignal( int s );
    // send the thread a signal

  protected:

    int       _suggestQuit;

#if defined(_LINUX_) || defined(_OSX_)
    pthread_t _threadId;
#endif

};


#endif
