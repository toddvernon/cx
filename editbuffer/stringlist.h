/***************************************************************************
 *
 *  dlist.h
 *
 *  CxDList Class
 *
 ***************************************************************************
 *
 * (c) Copyright 1999,2000,2001 T.Vernon
 * ALL RIGHTS RESERVED
 *
 * The software and information contained herein are proprietary to, and
 * comprise valuable trade secrets of, T.Vernon , which
 * intends to preserve as trade secrets such software and information.
 * This software is furnished pursuant to a written license agreement and
 * may be used, copied, transmitted, and stored only in accordance with
 * the terms of such license and with the inclusion of the above copyright
 * notice.  This software and information or any other copies thereof may
 * not be provided or otherwise made available to any other person.
 *
 *
 **************************************************************************/

#include <stdio.h>
#include <cx/base/exception.h>

#ifndef _CxBufferList_h_
#define _CxBufferList_h_


//-------------------------------------------------------------------------
// class CxStringList
//
//-------------------------------------------------------------------------

class CxStringList
{
public:

    CxStringList( void );
    // constructor
    
    CxStringList(unsigned long preallocated);
    // constructor
    
    ~CxStringList( void );
    // destructor
    
    void allocate(unsigned long entries = 10);
    // allocate the pointer array
    
    void reallocate(unsigned long increaseCount = 10);
    // reallocate the new pointer array
    
    void insertAfter( int index, CxString *newItem );
    // insert a new item below the index
    
    void insertBefore( int index, CxString *newItem );
    // insert a new item below the index
    
    void append( CxString *item );
    // add item to the list

    CxString *replaceAt( unsigned long i, CxString *item);
    // replace the item at index i

    CxString *removeAt( unsigned long i );
    // remove the item at item i
    
    unsigned long entries( void );
    // return the number of items in the list
    
    CxString * at( unsigned long i );
    // return a copy of the item at index i

    void initLazy(char* buffer, unsigned long size, int tabSpaces);
    // initialize lazy loading mode with raw buffer

//protected:

    unsigned long  _entries;
    unsigned long  _allocated;

    CxString **_list;

    // Lazy loading support
    char*           _rawBuffer;      // Raw file content (owned in lazy mode)
    unsigned long*  _lineOffsets;    // Start offset of each line
    unsigned long*  _lineLengths;    // Length of each line (excluding newline)
    int             _tabSpaces;      // Tab expansion setting
    int             _lazyMode;       // 1 = lazy mode active

private:
    void materializeLine(unsigned long i);
    // create CxString on demand from raw buffer

};

#endif
