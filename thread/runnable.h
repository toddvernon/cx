//-------------------------------------------------------------------------------------------------
//
//  runnable.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  runnable.h
//
//-------------------------------------------------------------------------------------------------

#ifndef _CxRunnable_h_
#define _CxRunnable_h_



class CxRunnable
{    
  public:
     	
      CxRunnable( void ) { };
    // Constructor
        
    virtual ~CxRunnable( void ) { };
    // Destructor

    virtual void run( void ) = 0;
    // the run method

    virtual int isQuitRequest() { return 0; }
};


#endif
