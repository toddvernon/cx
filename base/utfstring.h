//-------------------------------------------------------------------------------------------------
//
//  utfstring.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxUTFString Class
//
//-------------------------------------------------------------------------------------------------

#ifndef _CxUTFString_h_
#define _CxUTFString_h_

#include <cx/base/utfcharacter.h>
#include <cx/base/string.h>

//-------------------------------------------------------------------------------------------------
// CxUTFString
//
//-------------------------------------------------------------------------------------------------

class CxUTFString
{
  public:

    CxUTFString(void);
    // default constructor - creates empty string

    CxUTFString(const CxUTFString &s);
    // copy constructor

    CxUTFString& operator=(const CxUTFString &s);
    // assignment operator

    ~CxUTFString(void);
    // destructor

    void fromBytes(const char *utf8bytes, int len, int tabWidth);
    // Parse UTF-8 bytes into an array of CxUTFCharacter.
    // Tabs are expanded based on their position and tabWidth.

    void fromCxString(const CxString &s, int tabWidth);
    // Parse a CxString (convenience wrapper for fromBytes).

    void fromUTF8Bytes(const char *utf8bytes, int len);
    // Parse raw UTF-8 bytes. Tabs stored as width-1 (no tab expansion).

    CxString toBytes(void) const;
    // Convert back to raw UTF-8 bytes.
    // Tabs are stored as single '\t' characters (no expansion).

    CxString toBytesExpanded(void) const;
    // Convert to UTF-8 bytes with tabs expanded to spaces.
    // For display purposes only - tabs are replaced with appropriate spaces.

    int charCount(void) const;
    // Return the number of characters (grapheme clusters).

    int displayWidth(void) const;
    // Return the total display width in screen columns.

    CxUTFCharacter* at(int charIndex);
    // Return pointer to character at index, or NULL if out of bounds.

    const CxUTFCharacter* at(int charIndex) const;
    // Return const pointer to character at index, or NULL if out of bounds.

    int displayWidthAt(int charIndex) const;
    // Return the display width of the character at index.

    int displayColumnOfChar(int charIndex) const;
    // Return the screen column where character at index starts.

    int charIndexAtDisplayColumn(int displayCol) const;
    // Return the character index at or before the given display column.
    // If displayCol is in the middle of a wide character, returns that character's index.

    void insert(int charIndex, const CxUTFCharacter &ch);
    // Insert a single character at the given index.
    // If charIndex == charCount(), appends to end.

    void insert(int charIndex, const CxUTFString &s);
    // Insert a string at the given index.

    void remove(int charIndex, int count);
    // Remove count characters starting at charIndex.

    void append(const CxUTFCharacter &ch);
    // Append a single character to the end.

    void append(const CxUTFString &s);
    // Append a string to the end.

    CxUTFString subString(int charStart, int charCount) const;
    // Return a substring from charStart with charCount characters.

    void clear(void);
    // Remove all characters.

    int isEmpty(void) const;
    // Return true if string has no characters.

    int isASCII(void) const;
    // Return true if all characters are ASCII (optimization hint).

    void recalculateTabWidths(int tabWidth);
    // Recalculate tab display widths based on their positions.
    // Call this after insert/remove operations that may affect tab positions.

    int operator==(const CxUTFString &s) const;
    // Equality comparison.

    int operator!=(const CxUTFString &s) const;
    // Inequality comparison.

  private:

    CxUTFCharacter *_chars;
    int _length;
    int _capacity;

    void grow(int minCapacity);
    // Grow the internal array to hold at least minCapacity characters.

    void updateTabWidth(int charIndex, int tabWidth);
    // Update the display width of a tab at the given index.
};


#endif
