//-------------------------------------------------------------------------------------------------
//
// CxEditBufferPos
//
// CxEditBufferPos Class
//
//-------------------------------------------------------------------------------------------------
//
// (c) Copyright 2023 T.Vernon
// ALL RIGHTS RESERVED
//
// The software and information contained herein are proprietary to, and
// comprise valuable trade secrets of, T.Vernon , which
// intends to preserve as trade secrets such software and information.
// This software is furnished pursuant to a written license agreement and
// may be used, copied, transmitted, and stored only in accordance with
// the terms of such license and with the inclusion of the above copyright
// notice.  This software and information or any other copies thereof may
// not be provided or otherwise made available to any other person.
//
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

