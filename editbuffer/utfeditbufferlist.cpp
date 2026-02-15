//-------------------------------------------------------------------------------------------------
//
//  utfeditbufferlist.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxUTFEditBufferList implementation
//
//-------------------------------------------------------------------------------------------------

#include "utfeditbufferlist.h"


//-------------------------------------------------------------------------------------------------
// CxUTFEditBufferList
//
//
//-------------------------------------------------------------------------------------------------
CxUTFEditBufferList::CxUTFEditBufferList(void)
{
    currentIndex = -1;
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBufferList
//
//
//-------------------------------------------------------------------------------------------------
CxUTFEditBufferList::CxUTFEditBufferList( const CxUTFEditBufferList& ebl_ )
{
    if ( &ebl_ != this ) {
        list = ebl_.list;
    }
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBufferList::~CxUTFEditBufferList
//
//
//-------------------------------------------------------------------------------------------------
CxUTFEditBufferList::~CxUTFEditBufferList( void )
{
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBufferList::operator=
//
// Add a buffer pointer to the list
//
//-------------------------------------------------------------------------------------------------
CxUTFEditBufferList&
CxUTFEditBufferList::operator=( const CxUTFEditBufferList& ebl_ )
{
    if ( &ebl_ != this ) {
        list = ebl_.list;
    }
    return( *this );
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBufferList::add
//
// Add a buffer pointer to the list
//
//-------------------------------------------------------------------------------------------------
void
CxUTFEditBufferList::add( CxUTFEditBuffer *buffer )
{
    list.append( buffer );
    currentIndex = list.entries() -1;
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBufferList::findPath
//
// return the buffer matching the passed in path if it exists
//
//-------------------------------------------------------------------------------------------------
CxUTFEditBuffer *
CxUTFEditBufferList::findPath( CxString path )
{
    for (int c=0; c<list.entries(); c++) {

        CxUTFEditBuffer *buffer = list.at(c);
        CxString bufferPath = buffer->getFilePath();

        if (bufferPath == path) {
            currentIndex = c;
            return( buffer );
        }
    }

    return( NULL );
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBufferList::at
//
// Returns a pointer to the buffer at the specified index
//
//-------------------------------------------------------------------------------------------------
CxUTFEditBuffer *
CxUTFEditBufferList::at( int n )
{
    if (n < list.entries()) {
        return( list.at(n) );
    }

    currentIndex = -1;
    return( NULL );
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBufferList::current
//
// get the current edit buffer
//
//-------------------------------------------------------------------------------------------------
CxUTFEditBuffer *
CxUTFEditBufferList::current( void)
{
    if (list.entries() == 0) {
        currentIndex = -1;
        return(NULL);
    }

    return( list.at( currentIndex ));
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBufferList::next
//
// get the next buffer in the list
//
//-------------------------------------------------------------------------------------------------
CxUTFEditBuffer *
CxUTFEditBufferList::next( void )
{
    if (list.entries() == 0) {
        currentIndex = -1;
        return(NULL);
    }

    if (currentIndex == -1) {
        currentIndex = 0;
        return( list.at( currentIndex ));
    }

    currentIndex++;

    if (currentIndex >= list.entries()) {
        currentIndex = 0;
    }

    return( list.at( currentIndex ));
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBufferList::previous
//
// get the previous buffer in the list
//
//-------------------------------------------------------------------------------------------------
CxUTFEditBuffer *
CxUTFEditBufferList::previous( void )
{
    if (list.entries() == 0) {
        currentIndex = -1;
        return(NULL);
    }

    if (currentIndex == -1) {
        currentIndex = 0;
        return( list.at( currentIndex ));
    }

    currentIndex--;

    if (currentIndex < 0) {
        currentIndex = list.entries()-1;
    }

    return( list.at( currentIndex ));
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBufferList::first
//
// get the first buffer in the list
//
//-------------------------------------------------------------------------------------------------
CxUTFEditBuffer *
CxUTFEditBufferList::first( void )
{
    if (list.entries() > 0) {
        currentIndex = 0;
        return( list.at( currentIndex ) );
    }

    currentIndex = -1;
    return( NULL );
}


//-------------------------------------------------------------------------------------------------
// CxUTFEditBufferList::last
//
// get the last buffer in the list
//
//-------------------------------------------------------------------------------------------------
CxUTFEditBuffer *
CxUTFEditBufferList::last( void )
{
    if (list.entries() > 0) {
        currentIndex = list.entries()-1;
        return( list.at( currentIndex ) );
    }

    currentIndex = -1;
    return( NULL );
}


int
CxUTFEditBufferList::items(void)
{
    return( list.entries() );
}


int
CxUTFEditBufferList::currentItemIndex(void)
{
    return( currentIndex );
}
