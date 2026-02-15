//-------------------------------------------------------------------------------------------------
//
//  rmutex.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  rmutex.h
//
//-------------------------------------------------------------------------------------------------

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <sys/types.h>


#include <cx/base/string.h>
#include <cx/base/exception.h>

#include <cx/thread/mutex.h>    



#if defined(WIN32)
#endif


#if defined(_LINUX_) || defined(_SOLARIS_) || defined(_OSX_) || defined(_NETBSD_)
#include <pthread.h>
#endif


#ifndef _CxRMUTEX_H_
#define _CxRMUTEX_H_



#if defined(_LINUX_) || defined(_SOLARIS_) || defined(_OSX_) || defined(_NETBSD_)
#define CX_NO_THREAD 0
#define CX_THREAD_TYPE pthread_t
#define CX_THREAD_SELF pthread_self()
#define CX_THREAD_EQUAL(x,y) (pthread_equal(x,y))
#define CX_THREAD_NOT_EQUAL(x,y) (!pthread_equal(x,y))
#endif


#if defined( WIN32 )
#define CX_NO_THREAD 0x0fffffff
#define CX_THREAD_TYPE DWORD
#define CX_THREAD_SELF GetCurrentThreadId()
#define CX_THREAD_EQUAL(x,y) (x==y)
#define CX_THREAD_NOT_EQUAL(x,y) (x!=y)
#endif



class CxRecursiveMutex
{     
  public:
     	
    CxRecursiveMutex( void );
    // Constructor
        
    ~CxRecursiveMutex( void );
    // Destructor

    void acquire( void );
    // lock the mutex
   
    void release( void );
    // unlock the mutex

  private:

#if defined(_LINUX_) || defined(_OSX_)
    int                 _thCount;
    CxMutex             _mutex;
    CxMutex             _guardMutex;
    CX_THREAD_TYPE      _thOwner;
#endif

#if defined(WIN32)
    CxMutex             _mutex;
#endif
};


#endif
