//-------------------------------------------------------------------------------------------------
//
//  thread2.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  thread2.h
//
//-------------------------------------------------------------------------------------------------

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
#include <signal.h>
#endif



#ifndef _CxTHREAD2_H_
#define _CxTHREAD2_H_



class CxThreadDuple
{
     
  public:
     	
    CxThreadDuple( void );
    // Constructor
        
    ~CxThreadDuple( void );
    // Destructor

    int start( void );
    // do any lookups

    virtual void run1( void );
    virtual void run2( void );
    // the run method

    static void* srun1( void *i );
    static void* srun2( void *i );
    // startic thread run function

    void join( void );
    // join with thread

    void suggestQuit( void );
    // set the thread quit flag

    void resetSuggestQuit( void );
    // set the thread quit flag

    void sendSignal1( int s );
    void sendSignal2( int s );
    // send the thread a signal

  protected:

    int       _suggestQuit;

#if defined(_LINUX_) || defined(_OSX_)
    pthread_t _threadId1;
    pthread_t _threadId2;
#endif

};


#endif
