/***************************************************************************
 *
 *  stringlist.c
 *
 *  CxStringList Class
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
#include <cx/editbuffer/stringlist.h>

//-------------------------------------------------------------------------
// CxStringList::
//
//-------------------------------------------------------------------------
CxStringList::CxStringList( void  )
:_entries(0), _allocated(0)
{
    allocate( 1000 );
}


CxStringList::CxStringList(unsigned long preallocated)
:_entries(0), _allocated(0)
{
    allocate( preallocated );
}


//-------------------------------------------------------------------------
// CxStringList::~CxStringList
//
//-------------------------------------------------------------------------
CxStringList::~CxStringList( void )
{
}


//-------------------------------------------------------------------------
// CxStringList::allocate
//
//-------------------------------------------------------------------------
void
CxStringList::allocate(unsigned long entries)
{
    size_t size = sizeof( CxString * );

    _allocated = entries;
    _entries   = 0;

    // new
    _list = new CxString*[ _allocated ];
}


//-------------------------------------------------------------------------
// CxStringList::reallocate
//
//-------------------------------------------------------------------------
void
CxStringList::reallocate(unsigned long increaseCount)
{
    size_t size = sizeof( CxString * );

    unsigned long newNumberOfEntries = _allocated + increaseCount;

    //new
    CxString **newList = new  CxString*[ newNumberOfEntries ];

    for (int c=0; c<newNumberOfEntries; c++) {
        if (c<_entries) {
            newList[c] = _list[c];
        } else {
            newList[c] = (CxString *) NULL;
        }
    }

    // new
    delete[] _list;


    _list = newList;
    _allocated = newNumberOfEntries;
}


//-------------------------------------------------------------------------
// CxStringList::entries
//
//-------------------------------------------------------------------------
unsigned long
CxStringList::entries(void)
{
    return(_entries);
}


//-------------------------------------------------------------------------
// CxStringList::replaceAt
//
//-------------------------------------------------------------------------
CxString *
CxStringList::replaceAt( unsigned long i, CxString *item )
{
    if (i >= _entries) {
        throw CxException("CxStringList::at(invalid index)");
    }

    CxString *oldItem = _list[i];
    _list[i] = item;

    return( oldItem );
}


//-------------------------------------------------------------------------
// CxStringList::removeAt
//
//-------------------------------------------------------------------------
CxString *
CxStringList::removeAt( unsigned long i )
{
    if (_entries == 0) {
        throw CxException("CxStringList::at(invalid index)");
    }

    if (i >= _entries) {
        throw CxException("CxStringList::at(invalid index)");
    }

    CxString *oldItem = _list[i];

    for (unsigned j=i+1; j<_entries; j++) {
        _list[j-1] = _list[j];
    }

    _list[ _entries ] = NULL;

    _entries = _entries - 1;

    return( oldItem );
}


//-------------------------------------------------------------------------
// CxStringList::insertAfter
//
//-------------------------------------------------------------------------
void
CxStringList::insertAfter( int i,  CxString *newItem )
{
    if (_entries == 0) {
        throw CxException("CxStringList::at(invalid index)");
    }

    if (i >= _entries) {
        throw CxException("CxStringList::at(invalid index)");
    }

    if (_entries == _allocated) {
        reallocate(10);
    }

    _entries = _entries + 1;

    for (unsigned j=_entries; j>i; j--) {
        _list[j] = _list[j-1];
    }

    _list[i+1] = newItem;
}


//-------------------------------------------------------------------------
// CxStringList::insertBefore
//
//-------------------------------------------------------------------------
void
CxStringList::insertBefore( int i,  CxString *newItem )
{
    if (_entries == 0) {
        throw CxException("CxStringList::insertBefore(invalid index)");
    }

    if (i >= _entries) {
        throw CxException("CxStringList::insertBefore(invalid index)");
    }

    if (_entries == _allocated) {
        reallocate(10);
    }

    _entries = _entries + 1;

    for (unsigned j=_entries; j>i; j--) {
        _list[j] = _list[j-1];
    }

    _list[i] = newItem;
}


//-------------------------------------------------------------------------
// CxStringList::append
//
//-------------------------------------------------------------------------
void
CxStringList::append( CxString *item )
{
    if ( _entries == _allocated) {
        reallocate(10);
    }

    _list[ _entries ] = item;
    _entries++;
}


//-------------------------------------------------------------------------
// CxStringList::at
//
//-------------------------------------------------------------------------
CxString *
CxStringList::at( unsigned long i )
{
    return( _list[i] );

    throw CxException("CxStringList::at(invalid index)");
}

