//-------------------------------------------------------------------------------------------------
//
//  CxEditBufferPos.h
//
//  CxEditBufferPos Class
//
//-------------------------------------------------------------------------------------------------
//
//  (c) Copyright 2022 T.Vernon
//  ALL RIGHTS RESERVED
//
//  The software and information contained herein are proprietary to, and
//  comprise valuable trade secrets of, T.Vernon , which
//  intends to preserve as trade secrets such software and information.
//  This software is furnished pursuant to a written license agreement and
//  may be used, copied, transmitted, and stored only in accordance with
//  the terms of such license and with the inclusion of the above copyright
//  notice.  This software and information or any other copies thereof may
//  not be provided or otherwise made available to any other person.
//
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

