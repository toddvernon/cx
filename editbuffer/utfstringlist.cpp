//-------------------------------------------------------------------------------------------------
//
// CxUTFStringList
//
// CxUTFStringList Class
//
//-------------------------------------------------------------------------------------------------
//
// (c) Copyright 2025 T.Vernon
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
#include <string.h>

#include <cx/editbuffer/utfstringlist.h>


#define CXUTFSTRINGLIST_INITIAL_CAPACITY 16


//-------------------------------------------------------------------------------------------------
// CxUTFStringList:: (default constructor)
//
//-------------------------------------------------------------------------------------------------
CxUTFStringList::CxUTFStringList(void)
{
    _list = 0;
    _entries = 0;
    _capacity = 0;
}


//-------------------------------------------------------------------------------------------------
// CxUTFStringList:: (constructor with preallocated capacity)
//
//-------------------------------------------------------------------------------------------------
CxUTFStringList::CxUTFStringList(unsigned long preallocated)
{
    _list = 0;
    _entries = 0;
    _capacity = 0;

    if (preallocated > 0) {
        grow(preallocated);
    }
}


//-------------------------------------------------------------------------------------------------
// CxUTFStringList:: (copy constructor)
//
//-------------------------------------------------------------------------------------------------
CxUTFStringList::CxUTFStringList(const CxUTFStringList &list)
{
    _list = 0;
    _entries = 0;
    _capacity = 0;

    if (list._entries > 0) {
        grow(list._entries);
        for (unsigned long i = 0; i < list._entries; i++) {
            _list[i] = list._list[i];
        }
        _entries = list._entries;
    }
}


//-------------------------------------------------------------------------------------------------
// CxUTFStringList:: (assignment operator)
//
//-------------------------------------------------------------------------------------------------
CxUTFStringList&
CxUTFStringList::operator=(const CxUTFStringList &list)
{
    if (&list != this) {
        clear();

        if (list._entries > 0) {
            grow(list._entries);
            for (unsigned long i = 0; i < list._entries; i++) {
                _list[i] = list._list[i];
            }
            _entries = list._entries;
        }
    }
    return *this;
}


//-------------------------------------------------------------------------------------------------
// CxUTFStringList:: (destructor)
//
//-------------------------------------------------------------------------------------------------
CxUTFStringList::~CxUTFStringList(void)
{
    if (_list) {
        delete[] _list;
        _list = 0;
    }
    _entries = 0;
    _capacity = 0;
}


//-------------------------------------------------------------------------------------------------
// CxUTFStringList::grow
//
//-------------------------------------------------------------------------------------------------
void
CxUTFStringList::grow(unsigned long minCapacity)
{
    if (minCapacity <= _capacity) {
        return;
    }

    // Calculate new capacity
    unsigned long newCapacity = _capacity == 0 ? CXUTFSTRINGLIST_INITIAL_CAPACITY : _capacity * 2;
    if (newCapacity < minCapacity) {
        newCapacity = minCapacity;
    }

    // Allocate new array
    CxUTFString *newList = new CxUTFString[newCapacity];

    // Copy existing entries
    for (unsigned long i = 0; i < _entries; i++) {
        newList[i] = _list[i];
    }

    // Free old array
    if (_list) {
        delete[] _list;
    }

    _list = newList;
    _capacity = newCapacity;
}


//-------------------------------------------------------------------------------------------------
// CxUTFStringList::entries
//
//-------------------------------------------------------------------------------------------------
unsigned long
CxUTFStringList::entries(void) const
{
    return _entries;
}


//-------------------------------------------------------------------------------------------------
// CxUTFStringList::at
//
//-------------------------------------------------------------------------------------------------
CxUTFString*
CxUTFStringList::at(unsigned long index)
{
    if (index >= _entries) {
        return 0;
    }
    return &_list[index];
}


//-------------------------------------------------------------------------------------------------
// CxUTFStringList::at (const)
//
//-------------------------------------------------------------------------------------------------
const CxUTFString*
CxUTFStringList::at(unsigned long index) const
{
    if (index >= _entries) {
        return 0;
    }
    return &_list[index];
}


//-------------------------------------------------------------------------------------------------
// CxUTFStringList::append
//
//-------------------------------------------------------------------------------------------------
void
CxUTFStringList::append(const CxUTFString &s)
{
    grow(_entries + 1);
    _list[_entries] = s;
    _entries++;
}


//-------------------------------------------------------------------------------------------------
// CxUTFStringList::insertBefore
//
//-------------------------------------------------------------------------------------------------
void
CxUTFStringList::insertBefore(unsigned long index, const CxUTFString &s)
{
    if (index >= _entries) {
        append(s);
        return;
    }

    grow(_entries + 1);

    // Shift entries down to make room
    for (unsigned long i = _entries; i > index; i--) {
        _list[i] = _list[i - 1];
    }

    _list[index] = s;
    _entries++;
}


//-------------------------------------------------------------------------------------------------
// CxUTFStringList::insertAfter
//
//-------------------------------------------------------------------------------------------------
void
CxUTFStringList::insertAfter(unsigned long index, const CxUTFString &s)
{
    if (index >= _entries) {
        append(s);
        return;
    }

    insertBefore(index + 1, s);
}


//-------------------------------------------------------------------------------------------------
// CxUTFStringList::removeAt
//
//-------------------------------------------------------------------------------------------------
void
CxUTFStringList::removeAt(unsigned long index)
{
    if (index >= _entries) {
        return;
    }

    // Shift entries up
    for (unsigned long i = index; i < _entries - 1; i++) {
        _list[i] = _list[i + 1];
    }

    // Clear the last entry (now a duplicate)
    _list[_entries - 1].clear();

    _entries--;
}


//-------------------------------------------------------------------------------------------------
// CxUTFStringList::replaceAt
//
//-------------------------------------------------------------------------------------------------
void
CxUTFStringList::replaceAt(unsigned long index, const CxUTFString &s)
{
    if (index >= _entries) {
        return;
    }
    _list[index] = s;
}


//-------------------------------------------------------------------------------------------------
// CxUTFStringList::clear
//
//-------------------------------------------------------------------------------------------------
void
CxUTFStringList::clear(void)
{
    _entries = 0;
    // Keep capacity for reuse
}


//-------------------------------------------------------------------------------------------------
// CxUTFStringList::isEmpty
//
//-------------------------------------------------------------------------------------------------
int
CxUTFStringList::isEmpty(void) const
{
    return _entries == 0;
}
