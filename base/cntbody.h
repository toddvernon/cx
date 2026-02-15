//-------------------------------------------------------------------------------------------------
//
//  cntbody.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxCountedBody Class
//
//-------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <iostream>


#if defined(_LINUX_) || defined(_OSX_)
#include <unistd.h>
#endif

#if defined(_NT_)
#endif


#ifndef _CxCountedBody_h_
#define _CxCountedBody_h_


//-------------------------------------------------------------------------
// class CxCountedBody
//
//-------------------------------------------------------------------------
class CxCountedBody
{
  public:

    CxCountedBody();
	// constructor

    virtual ~CxCountedBody( void );
	// destructor

    void incCount( void );
	// increment the count

    void decCount( void );
	// decrement the count

    int count( void );
	// return the count

  private:

    int _count;
};


#endif
