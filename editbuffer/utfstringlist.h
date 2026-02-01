//-------------------------------------------------------------------------------------------------
//
//  utfstringlist.h
//
//  CxUTFStringList Class
//
//-------------------------------------------------------------------------------------------------
//
//  (c) Copyright 2025 T.Vernon
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
//
//  CxUTFStringList is a dynamic array of CxUTFString objects.
//  Used by CxUTFEditBuffer to store lines of text.
//
//-------------------------------------------------------------------------------------------------

#ifndef _CxUTFStringList_h_
#define _CxUTFStringList_h_

#include <cx/editbuffer/utfstring.h>

//-------------------------------------------------------------------------------------------------
// CxUTFStringList
//
//-------------------------------------------------------------------------------------------------

class CxUTFStringList
{
  public:

    CxUTFStringList(void);
    // default constructor

    CxUTFStringList(unsigned long preallocated);
    // constructor with pre-allocated capacity

    CxUTFStringList(const CxUTFStringList &list);
    // copy constructor

    CxUTFStringList& operator=(const CxUTFStringList &list);
    // assignment operator

    ~CxUTFStringList(void);
    // destructor

    unsigned long entries(void) const;
    // return the number of strings in the list

    CxUTFString* at(unsigned long index);
    // return pointer to string at index, or NULL if out of bounds

    const CxUTFString* at(unsigned long index) const;
    // return const pointer to string at index

    void append(const CxUTFString &s);
    // add a string to the end of the list

    void insertBefore(unsigned long index, const CxUTFString &s);
    // insert a string before the given index
    // if index >= entries(), appends to end

    void insertAfter(unsigned long index, const CxUTFString &s);
    // insert a string after the given index
    // if index >= entries(), appends to end

    void removeAt(unsigned long index);
    // remove the string at the given index

    void replaceAt(unsigned long index, const CxUTFString &s);
    // replace the string at the given index

    void clear(void);
    // remove all strings

    int isEmpty(void) const;
    // return true if list has no entries

  private:

    CxUTFString *_list;
    unsigned long _entries;
    unsigned long _capacity;

    void grow(unsigned long minCapacity);
    // grow the internal array
};


#endif
