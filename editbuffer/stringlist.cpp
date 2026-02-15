//-------------------------------------------------------------------------------------------------
//
//  stringlist.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxStringList Class
//
//-------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <cx/base/exception.h>
#include <cx/editbuffer/stringlist.h>
#include <cx/editbuffer/stringutils.h>

//-------------------------------------------------------------------------
// CxStringList::
//
//-------------------------------------------------------------------------
CxStringList::CxStringList( void  )
:_entries(0), _allocated(0),
 _rawBuffer(NULL), _lineOffsets(NULL), _lineLengths(NULL),
 _tabSpaces(4), _lazyMode(0)
{
    allocate( 1000 );
}


CxStringList::CxStringList(unsigned long preallocated)
:_entries(0), _allocated(0),
 _rawBuffer(NULL), _lineOffsets(NULL), _lineLengths(NULL),
 _tabSpaces(4), _lazyMode(0)
{
    allocate( preallocated );
}


//-------------------------------------------------------------------------
// CxStringList::~CxStringList
//
//-------------------------------------------------------------------------
CxStringList::~CxStringList( void )
{
    if (_lazyMode) {
        // In lazy mode, we own the CxString objects that were materialized
        for (unsigned long i = 0; i < _entries; i++) {
            if (_list[i] != NULL) {
                delete _list[i];
            }
        }
        delete[] _list;
        delete[] _rawBuffer;
        delete[] _lineOffsets;
        delete[] _lineLengths;
    }
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

    for (unsigned long c=0; c<newNumberOfEntries; c++) {
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

    // Also reallocate offset/length arrays if in lazy mode
    if (_lazyMode) {
        unsigned long *newOffsets = new unsigned long[ newNumberOfEntries ];
        unsigned long *newLengths = new unsigned long[ newNumberOfEntries ];

        for (unsigned long c=0; c<newNumberOfEntries; c++) {
            if (c < _entries) {
                newOffsets[c] = _lineOffsets[c];
                newLengths[c] = _lineLengths[c];
            } else {
                newOffsets[c] = 0;
                newLengths[c] = 0;
            }
        }

        delete[] _lineOffsets;
        delete[] _lineLengths;

        _lineOffsets = newOffsets;
        _lineLengths = newLengths;
    }
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

    // In lazy mode, replaced line is no longer from raw buffer
    if (_lazyMode) {
        _lineOffsets[i] = 0;
        _lineLengths[i] = 0;
    }

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

    for (unsigned long j=i+1; j<_entries; j++) {
        _list[j-1] = _list[j];
    }

    // Also shift offset/length arrays in lazy mode
    if (_lazyMode) {
        for (unsigned long j=i+1; j<_entries; j++) {
            _lineOffsets[j-1] = _lineOffsets[j];
            _lineLengths[j-1] = _lineLengths[j];
        }
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

    if (i >= (int)_entries) {
        throw CxException("CxStringList::at(invalid index)");
    }

    if (_entries == _allocated) {
        reallocate(10);
    }

    _entries = _entries + 1;

    for (unsigned long j=_entries; j>(unsigned long)i; j--) {
        _list[j] = _list[j-1];
    }

    // Also shift offset/length arrays in lazy mode
    if (_lazyMode) {
        for (unsigned long j=_entries; j>(unsigned long)i; j--) {
            _lineOffsets[j] = _lineOffsets[j-1];
            _lineLengths[j] = _lineLengths[j-1];
        }
        // Inserted line is not from raw buffer
        _lineOffsets[i+1] = 0;
        _lineLengths[i+1] = 0;
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

    if (i >= (int)_entries) {
        throw CxException("CxStringList::insertBefore(invalid index)");
    }

    if (_entries == _allocated) {
        reallocate(10);
    }

    _entries = _entries + 1;

    for (unsigned long j=_entries; j>(unsigned long)i; j--) {
        _list[j] = _list[j-1];
    }

    // Also shift offset/length arrays in lazy mode
    if (_lazyMode) {
        for (unsigned long j=_entries; j>(unsigned long)i; j--) {
            _lineOffsets[j] = _lineOffsets[j-1];
            _lineLengths[j] = _lineLengths[j-1];
        }
        // Inserted line is not from raw buffer
        _lineOffsets[i] = 0;
        _lineLengths[i] = 0;
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

    // In lazy mode, appended line is not from raw buffer
    if (_lazyMode) {
        _lineOffsets[_entries] = 0;
        _lineLengths[_entries] = 0;
    }

    _entries++;
}


//-------------------------------------------------------------------------
// CxStringList::at
//
//-------------------------------------------------------------------------
CxString *
CxStringList::at( unsigned long i )
{
    // In lazy mode, materialize line on first access
    if (_lazyMode && _list[i] == NULL) {
        materializeLine(i);
    }

    return( _list[i] );
}


//-------------------------------------------------------------------------
// CxStringList::initLazy
//
// Initialize lazy loading mode. Takes ownership of buffer.
// Single pass to count lines and record offsets.
//-------------------------------------------------------------------------
void
CxStringList::initLazy(char* buffer, unsigned long size, int tabSpaces)
{
    _rawBuffer = buffer;
    _tabSpaces = tabSpaces;
    _lazyMode = 1;

    // First pass: count lines
    unsigned long lineCount = 0;
    for (unsigned long i = 0; i < size; i++) {
        if (buffer[i] == '\n') {
            lineCount++;
        }
    }
    // Account for final line without newline
    if (size > 0 && buffer[size-1] != '\n') {
        lineCount++;
    }

    // Allocate arrays
    _allocated = lineCount + 100;  // Some extra for insertions
    _entries = lineCount;

    _list = new CxString*[_allocated];
    _lineOffsets = new unsigned long[_allocated];
    _lineLengths = new unsigned long[_allocated];

    // Initialize list to NULL
    for (unsigned long i = 0; i < _allocated; i++) {
        _list[i] = NULL;
        _lineOffsets[i] = 0;
        _lineLengths[i] = 0;
    }

    // Second pass: record offsets and lengths
    unsigned long lineIndex = 0;
    unsigned long lineStart = 0;

    for (unsigned long i = 0; i < size; i++) {
        if (buffer[i] == '\n') {
            _lineOffsets[lineIndex] = lineStart;
            _lineLengths[lineIndex] = i - lineStart;
            lineIndex++;
            lineStart = i + 1;
        }
    }

    // Handle final line without newline
    if (size > 0 && buffer[size-1] != '\n') {
        _lineOffsets[lineIndex] = lineStart;
        _lineLengths[lineIndex] = size - lineStart;
    }
}


//-------------------------------------------------------------------------
// CxStringList::materializeLine
//
// Create CxString on demand from raw buffer.
// Temporarily null-terminates line for tab expansion.
//-------------------------------------------------------------------------
void
CxStringList::materializeLine(unsigned long i)
{
    if (!_lazyMode || i >= _entries) {
        return;
    }

    unsigned long offset = _lineOffsets[i];
    unsigned long length = _lineLengths[i];

    // Save char after line, temporarily null-terminate
    char savedChar = _rawBuffer[offset + length];
    _rawBuffer[offset + length] = '\0';

    // Create CxString with tab expansion
    // toTabExtensionFormat2 takes a CxString which can be constructed from char*
    _list[i] = new CxString(CxStringUtils::toTabExtensionFormat2(&_rawBuffer[offset], _tabSpaces));

    // Restore original char
    _rawBuffer[offset + length] = savedChar;
}
