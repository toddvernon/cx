//-------------------------------------------------------------------------------------------------
//
//  editbufferpos.cpp
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

#include <cx/editbuffer/editbufferpos.h>



//-------------------------------------------------------------------------------------------------
// CxEditBufferPosition:: (constructor)
//
//-------------------------------------------------------------------------------------------------
CxEditBufferPosition::CxEditBufferPosition(void)
:row(0), col(0)
{
}

//-------------------------------------------------------------------------------------------------
// CxEditBufferPosition:: (copy constructor)
//
//-------------------------------------------------------------------------------------------------
CxEditBufferPosition::CxEditBufferPosition(unsigned long r, unsigned long c)
:row(0), col(0)
{
    row = r;
    col = c;
}

//-------------------------------------------------------------------------------------------------
// CxEditBufferPosition:: (copy constructor)
//
//-------------------------------------------------------------------------------------------------
CxEditBufferPosition::CxEditBufferPosition( const CxEditBufferPosition& c )
{
    if (&c != this) {
        row = c.row;
        col = c.col;
    }
}

//-------------------------------------------------------------------------------------------------
// CxEditBufferPosition:: (assignment operator)
//
//-------------------------------------------------------------------------------------------------
CxEditBufferPosition
CxEditBufferPosition::operator=( const CxEditBufferPosition& c)
{
    if (&c != this) {
        row = c.row;
        col = c.col;
    }

    return( *this );
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

int
CxEditBufferPosition::operator==( const CxEditBufferPosition& c)
{
    if ((row == c.row) && (col == c.col)) return( true );
    return(false);
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
int
CxEditBufferPosition::operator>=( const CxEditBufferPosition& c)
{
    if ((row >= c.row) && (col >= c.col)) return( true );
    return( false );
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
int
CxEditBufferPosition::operator<=( const CxEditBufferPosition& c)
{
    if ((row <= c.row) && (col <= c.col)) return( true );
    return( false );
}
