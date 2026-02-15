//-------------------------------------------------------------------------------------------------
//
//  mutex.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  mutex.h
//
//-------------------------------------------------------------------------------------------------

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>


#include <cx/base/string.h>


#if defined(_LINUX_) || defined(_OSX_)
#include <pthread.h>
#endif


#if defined(WIN32)
#include <windows.h>
#endif

    
#ifndef _CxMUTEX_H_
#define _CxMUTEX_H_



class CxMutex
{
     
  public:

    enum tryResult { MUTEX_ACQUIRED, MUTEX_BUSY };
	// result from tryAcquire
     	
    CxMutex( void );
    // Constructor
        
    ~CxMutex( void );
    // Destructor

    void acquire( void );
    // lock the mutex
   
    void release( void );
    // unlock the mutex

	tryResult tryAcquire( void );
	// try to acquire the mutex

#if defined(_LINUX_) || defined(_OSX_)
	pthread_mutex_t *pthread_mutex(void);
#endif

  private:

#if defined(_LINUX_) || defined(_OSX_)
    pthread_mutex_t _mutex;
#endif

#if defined(WIN32)
    HANDLE _mutex;
#endif

};


#endif
