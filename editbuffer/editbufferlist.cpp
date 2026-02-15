//-------------------------------------------------------------------------------------------------
//
//  editbufferlist.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  Created by Todd Vernon on 3/22/22.
//
//-------------------------------------------------------------------------------------------------

#include "editbufferlist.h"


//-------------------------------------------------------------------------------------------------
// CxEditBufferList
//
//
//-------------------------------------------------------------------------------------------------
CxEditBufferList::CxEditBufferList(void)
{
    currentIndex = -1;
}


//-------------------------------------------------------------------------------------------------
// CxEditBufferList
//
//
//-------------------------------------------------------------------------------------------------
CxEditBufferList::CxEditBufferList( const CxEditBufferList& ebl_ )
{
	if ( &ebl_ != this ) {
		list = ebl_.list;
	}
}


//-------------------------------------------------------------------------------------------------
// CxEditBufferList::~CxEditBufferList
//
//
//-------------------------------------------------------------------------------------------------
CxEditBufferList::~CxEditBufferList( void )
{
}


//-------------------------------------------------------------------------------------------------
// CxEditBufferList::operator=
// 
// Add a buffer pointer to the list
// 
//-------------------------------------------------------------------------------------------------
CxEditBufferList&
CxEditBufferList::operator=( const CxEditBufferList& ebl_ )
{
	if ( &ebl_ != this ) {
		list = ebl_.list;
	}
	return( *this );
}


//-------------------------------------------------------------------------------------------------
// CxEditBufferList::add
// 
// Add a buffer pointer to the list
// 
//-------------------------------------------------------------------------------------------------
void 
CxEditBufferList::add( CxEditBuffer *buffer )
{
	list.append( buffer );
    currentIndex = list.entries() -1;
}


//-------------------------------------------------------------------------------------------------
// CxEditBufferList::findPath
//
// return the buffer matching the passed in path if it exists
//
//-------------------------------------------------------------------------------------------------
CxEditBuffer *
CxEditBufferList::findPath( CxString path )
{
    for (int c=0; c<list.entries(); c++) {
        
        CxEditBuffer *buffer = list.at(c);
        CxString bufferPath = buffer->getFilePath();
        
        if (bufferPath == path) {
            currentIndex = c;
            return( buffer );
        }
    }
    
    return( NULL );
}


//-------------------------------------------------------------------------------------------------
// CxEditBufferList::at
//
// Returns a pointer to the buffer at the specified index
//
//-------------------------------------------------------------------------------------------------
CxEditBuffer *
CxEditBufferList::at( int n )
{
	if (n < list.entries()) {
		return( list.at(n) );
	}

    currentIndex = -1;
	return( NULL );
}


//-------------------------------------------------------------------------------------------------
// CxEditBufferList::current
//
// get the current edit buffer
//
//-------------------------------------------------------------------------------------------------
CxEditBuffer *
CxEditBufferList::current( void)
{
    if (list.entries() == 0) {
        currentIndex = -1;
        return(NULL);
    }
    
    return( list.at( currentIndex ));
}


//-------------------------------------------------------------------------------------------------
// CxEditBufferList::next
//
// get the next buffer in the list
//
//-------------------------------------------------------------------------------------------------
CxEditBuffer *
CxEditBufferList::next( void )
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
// CxEditBufferList::previous
//
// get the previous buffer in the list
//
//-------------------------------------------------------------------------------------------------
CxEditBuffer *
CxEditBufferList::previous( void )
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
// CxEditBufferList::first
//
// get the first buffer in the list
//
//-------------------------------------------------------------------------------------------------
CxEditBuffer *
CxEditBufferList::first( void )
{
    if (list.entries() > 0) {
        currentIndex = 0;
        return( list.at( currentIndex ) );
    }
    
    currentIndex = -1;
    return( NULL );
}


//-------------------------------------------------------------------------------------------------
// CxEditBufferList::last
//
// get the last buffer in the list
//
//-------------------------------------------------------------------------------------------------
CxEditBuffer *
CxEditBufferList::last( void )
{
    if (list.entries() > 0) {
        currentIndex = list.entries()-1;
        return( list.at( currentIndex ) );
    }
    
    currentIndex = -1;
    return( NULL );
}


int
CxEditBufferList::items(void)
{
    return( list.entries() );
}


int
CxEditBufferList::currentItemIndex(void)
{
    return( currentIndex );
}
