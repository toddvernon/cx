//-------------------------------------------------------------------------------------------------
//
//  editbufferpos.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxEditBufferPos Class
//
//-------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>

#ifndef _CxEditBufferPos_
#define _CxEditBufferPos_


//-------------------------------------------------------------------------------------------------
// CxEditBufferPosition::
//
//-------------------------------------------------------------------------------------------------

class CxEditBufferPosition
{
  public:
    
    CxEditBufferPosition(void);
    // constructor
    
    CxEditBufferPosition(unsigned long r, unsigned long c);
    // constructor
    
    CxEditBufferPosition( const CxEditBufferPosition& c_);
    // copy constructor
    
    CxEditBufferPosition operator=( const CxEditBufferPosition& c_);
    // assignment operator

    int operator==( const CxEditBufferPosition& c);
    // are item equal
    
    int operator<=( const CxEditBufferPosition& c);
    // is item less than or equal
    
    int operator>=( const CxEditBufferPosition& c);
    // is item great than or equal
    
    unsigned long row;
    unsigned long col;
};


#endif
