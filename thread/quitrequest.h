//-------------------------------------------------------------------------------------------------
//
//  quitrequest.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  quitrequest.h
//
//-------------------------------------------------------------------------------------------------

#ifndef _CxQuitRequest_h_
#define _CxQuitRequest_h_



class CxQuitRequest : public CxRunnable
{    
  public:
     	
    CxQuitRequest( void ) { }
    // Constructor
        
    ~CxQuitRequest( void ) { }
    // Destructor

    virtual int isQuitRequest() { return 1; }
    // This is a quit request

    virtual void run() { }
};


#endif
