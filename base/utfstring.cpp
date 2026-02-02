//-------------------------------------------------------------------------------------------------
//
// CxUTFString
//
// CxUTFString Class
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

#include <cx/base/utfstring.h>


#define CXUTFSTRING_INITIAL_CAPACITY 16


//-------------------------------------------------------------------------------------------------
// CxUTFString:: (default constructor)
//
//-------------------------------------------------------------------------------------------------
CxUTFString::CxUTFString(void)
{
    _chars = 0;
    _length = 0;
    _capacity = 0;
}


//-------------------------------------------------------------------------------------------------
// CxUTFString:: (copy constructor)
//
//-------------------------------------------------------------------------------------------------
CxUTFString::CxUTFString(const CxUTFString &s)
{
    _chars = 0;
    _length = 0;
    _capacity = 0;

    if (s._length > 0) {
        grow(s._length);
        for (int i = 0; i < s._length; i++) {
            _chars[i] = s._chars[i];
        }
        _length = s._length;
    }
}


//-------------------------------------------------------------------------------------------------
// CxUTFString:: (assignment operator)
//
//-------------------------------------------------------------------------------------------------
CxUTFString&
CxUTFString::operator=(const CxUTFString &s)
{
    if (&s != this) {
        clear();
        if (s._length > 0) {
            grow(s._length);
            for (int i = 0; i < s._length; i++) {
                _chars[i] = s._chars[i];
            }
            _length = s._length;
        }
    }
    return *this;
}


//-------------------------------------------------------------------------------------------------
// CxUTFString:: (destructor)
//
//-------------------------------------------------------------------------------------------------
CxUTFString::~CxUTFString(void)
{
    if (_chars) {
        delete[] _chars;
        _chars = 0;
    }
    _length = 0;
    _capacity = 0;
}


//-------------------------------------------------------------------------------------------------
// CxUTFString::grow
//
// Grow the internal array to hold at least minCapacity characters.
//
//-------------------------------------------------------------------------------------------------
void
CxUTFString::grow(int minCapacity)
{
    if (minCapacity <= _capacity) {
        return;
    }

    // Calculate new capacity (double or use minCapacity, whichever is larger)
    int newCapacity = _capacity == 0 ? CXUTFSTRING_INITIAL_CAPACITY : _capacity * 2;
    if (newCapacity < minCapacity) {
        newCapacity = minCapacity;
    }

    // Allocate new array
    CxUTFCharacter *newChars = new CxUTFCharacter[newCapacity];

    // Copy existing characters
    for (int i = 0; i < _length; i++) {
        newChars[i] = _chars[i];
    }

    // Free old array and update pointers
    if (_chars) {
        delete[] _chars;
    }
    _chars = newChars;
    _capacity = newCapacity;
}


//-------------------------------------------------------------------------------------------------
// CxUTFString::updateTabWidth
//
// Update the display width of a tab character based on its position.
//
//-------------------------------------------------------------------------------------------------
void
CxUTFString::updateTabWidth(int charIndex, int tabWidth)
{
    if (charIndex < 0 || charIndex >= _length) return;
    if (!_chars[charIndex].isTab()) return;

    // Calculate display column of this character
    int displayCol = displayColumnOfChar(charIndex);

    // Tab width is distance to next tab stop
    int width = tabWidth - (displayCol % tabWidth);
    if (width == 0) width = tabWidth;

    // Update the tab's display width by recreating it
    _chars[charIndex] = CxUTFCharacter::makeTab(width);
}


//-------------------------------------------------------------------------------------------------
// CxUTFString::fromBytes
//
// Parse UTF-8 bytes into an array of CxUTFCharacter.
//
//-------------------------------------------------------------------------------------------------
void
CxUTFString::fromBytes(const char *utf8bytes, int len, int tabWidth)
{
    clear();

    if (utf8bytes == 0 || len <= 0) {
        return;
    }

    // First pass: count characters to pre-allocate
    const char *p = utf8bytes;
    const char *end = utf8bytes + len;
    int count = 0;

    while (p < end && *p != 0) {
        if (*p == '\t') {
            count++;
            p++;
        } else {
            CxUTFCharacter temp;
            int consumed = temp.fromUTF8(p);
            if (consumed == 0) break;
            count++;
            p += consumed;
        }
    }

    if (count == 0) {
        return;
    }

    // Allocate space
    grow(count);

    // Second pass: parse characters
    p = utf8bytes;
    int displayCol = 0;

    while (p < end && *p != 0 && _length < count) {
        if (*p == '\t') {
            // Calculate tab width based on current display column
            int width = tabWidth - (displayCol % tabWidth);
            if (width == 0) width = tabWidth;

            _chars[_length] = CxUTFCharacter::makeTab(width);
            displayCol += width;
            _length++;
            p++;
        } else {
            int consumed = _chars[_length].fromUTF8(p);
            if (consumed == 0) break;
            displayCol += _chars[_length].displayWidth();
            _length++;
            p += consumed;
        }
    }
}


//-------------------------------------------------------------------------------------------------
// CxUTFString::fromCxString
//
// Parse a CxString (convenience wrapper for fromBytes).
//
//-------------------------------------------------------------------------------------------------
void
CxUTFString::fromCxString(const CxString &s, int tabWidth)
{
    fromBytes(s.data(), s.length(), tabWidth);
}


//-------------------------------------------------------------------------------------------------
// CxUTFString::toBytes
//
// Convert back to raw UTF-8 bytes.
//
//-------------------------------------------------------------------------------------------------
CxString
CxUTFString::toBytes(void) const
{
    if (_length == 0) {
        return CxString("");
    }

    // Calculate total byte length
    int totalBytes = 0;
    for (int i = 0; i < _length; i++) {
        if (_chars[i].isTab()) {
            totalBytes += 1;  // Just the tab character
        } else {
            totalBytes += _chars[i].byteCount();
        }
    }

    // Allocate buffer
    char *buffer = new char[totalBytes + 1];
    char *p = buffer;

    // Copy bytes
    for (int i = 0; i < _length; i++) {
        if (_chars[i].isTab()) {
            *p++ = '\t';
        } else {
            int bc = _chars[i].byteCount();
            const unsigned char *bytes = _chars[i].bytes();
            for (int j = 0; j < bc; j++) {
                *p++ = bytes[j];
            }
        }
    }
    *p = 0;

    CxString result(buffer);
    delete[] buffer;

    return result;
}


//-------------------------------------------------------------------------------------------------
// CxUTFString::toBytesExpanded
//
// Convert to UTF-8 bytes with tabs expanded to spaces for display purposes.
//
//-------------------------------------------------------------------------------------------------
CxString
CxUTFString::toBytesExpanded(void) const
{
    if (_length == 0) {
        return CxString("");
    }

    // Calculate total byte length with tabs expanded
    int totalBytes = 0;
    for (int i = 0; i < _length; i++) {
        if (_chars[i].isTab()) {
            totalBytes += _chars[i].displayWidth();  // Tab width in spaces
        } else {
            totalBytes += _chars[i].byteCount();
        }
    }

    // Allocate buffer
    char *buffer = new char[totalBytes + 1];
    char *p = buffer;

    // Copy bytes with tabs expanded to spaces
    for (int i = 0; i < _length; i++) {
        if (_chars[i].isTab()) {
            // Expand tab to spaces based on its display width
            int tabWidth = _chars[i].displayWidth();
            for (int j = 0; j < tabWidth; j++) {
                *p++ = ' ';
            }
        } else {
            int bc = _chars[i].byteCount();
            const unsigned char *bytes = _chars[i].bytes();
            for (int j = 0; j < bc; j++) {
                *p++ = bytes[j];
            }
        }
    }
    *p = 0;

    CxString result(buffer);
    delete[] buffer;

    return result;
}


//-------------------------------------------------------------------------------------------------
// CxUTFString::charCount
//
//-------------------------------------------------------------------------------------------------
int
CxUTFString::charCount(void) const
{
    return _length;
}


//-------------------------------------------------------------------------------------------------
// CxUTFString::displayWidth
//
// Return the total display width in screen columns.
//
//-------------------------------------------------------------------------------------------------
int
CxUTFString::displayWidth(void) const
{
    int width = 0;
    for (int i = 0; i < _length; i++) {
        width += _chars[i].displayWidth();
    }
    return width;
}


//-------------------------------------------------------------------------------------------------
// CxUTFString::at
//
//-------------------------------------------------------------------------------------------------
CxUTFCharacter*
CxUTFString::at(int charIndex)
{
    if (charIndex < 0 || charIndex >= _length) {
        return 0;
    }
    return &_chars[charIndex];
}


//-------------------------------------------------------------------------------------------------
// CxUTFString::at (const)
//
//-------------------------------------------------------------------------------------------------
const CxUTFCharacter*
CxUTFString::at(int charIndex) const
{
    if (charIndex < 0 || charIndex >= _length) {
        return 0;
    }
    return &_chars[charIndex];
}


//-------------------------------------------------------------------------------------------------
// CxUTFString::displayWidthAt
//
//-------------------------------------------------------------------------------------------------
int
CxUTFString::displayWidthAt(int charIndex) const
{
    if (charIndex < 0 || charIndex >= _length) {
        return 0;
    }
    return _chars[charIndex].displayWidth();
}


//-------------------------------------------------------------------------------------------------
// CxUTFString::displayColumnOfChar
//
// Return the screen column where character at index starts.
//
//-------------------------------------------------------------------------------------------------
int
CxUTFString::displayColumnOfChar(int charIndex) const
{
    if (charIndex < 0) return 0;
    if (charIndex > _length) charIndex = _length;

    int col = 0;
    for (int i = 0; i < charIndex; i++) {
        col += _chars[i].displayWidth();
    }
    return col;
}


//-------------------------------------------------------------------------------------------------
// CxUTFString::charIndexAtDisplayColumn
//
// Return the character index at or before the given display column.
//
//-------------------------------------------------------------------------------------------------
int
CxUTFString::charIndexAtDisplayColumn(int displayCol) const
{
    if (_length == 0) return 0;
    if (displayCol <= 0) return 0;

    int col = 0;
    for (int i = 0; i < _length; i++) {
        int width = _chars[i].displayWidth();
        if (col + width > displayCol) {
            // Display column falls within this character
            return i;
        }
        col += width;
    }

    // Beyond end, return length (append position)
    return _length;
}


//-------------------------------------------------------------------------------------------------
// CxUTFString::insert (single character)
//
//-------------------------------------------------------------------------------------------------
void
CxUTFString::insert(int charIndex, const CxUTFCharacter &ch)
{
    if (charIndex < 0) charIndex = 0;
    if (charIndex > _length) charIndex = _length;

    grow(_length + 1);

    // Shift characters to make room
    for (int i = _length; i > charIndex; i--) {
        _chars[i] = _chars[i - 1];
    }

    _chars[charIndex] = ch;
    _length++;
}


//-------------------------------------------------------------------------------------------------
// CxUTFString::insert (string)
//
//-------------------------------------------------------------------------------------------------
void
CxUTFString::insert(int charIndex, const CxUTFString &s)
{
    if (s._length == 0) return;
    if (charIndex < 0) charIndex = 0;
    if (charIndex > _length) charIndex = _length;

    grow(_length + s._length);

    // Shift characters to make room
    for (int i = _length - 1; i >= charIndex; i--) {
        _chars[i + s._length] = _chars[i];
    }

    // Copy inserted characters
    for (int i = 0; i < s._length; i++) {
        _chars[charIndex + i] = s._chars[i];
    }

    _length += s._length;
}


//-------------------------------------------------------------------------------------------------
// CxUTFString::remove
//
//-------------------------------------------------------------------------------------------------
void
CxUTFString::remove(int charIndex, int count)
{
    if (charIndex < 0 || charIndex >= _length || count <= 0) {
        return;
    }

    // Clamp count to available characters
    if (charIndex + count > _length) {
        count = _length - charIndex;
    }

    // Shift characters down
    for (int i = charIndex; i < _length - count; i++) {
        _chars[i] = _chars[i + count];
    }

    _length -= count;
}


//-------------------------------------------------------------------------------------------------
// CxUTFString::append (single character)
//
//-------------------------------------------------------------------------------------------------
void
CxUTFString::append(const CxUTFCharacter &ch)
{
    grow(_length + 1);
    _chars[_length] = ch;
    _length++;
}


//-------------------------------------------------------------------------------------------------
// CxUTFString::append (string)
//
//-------------------------------------------------------------------------------------------------
void
CxUTFString::append(const CxUTFString &s)
{
    if (s._length == 0) return;

    grow(_length + s._length);

    for (int i = 0; i < s._length; i++) {
        _chars[_length + i] = s._chars[i];
    }

    _length += s._length;
}


//-------------------------------------------------------------------------------------------------
// CxUTFString::subString
//
//-------------------------------------------------------------------------------------------------
CxUTFString
CxUTFString::subString(int charStart, int charCount) const
{
    CxUTFString result;

    if (charStart < 0) charStart = 0;
    if (charStart >= _length) return result;
    if (charCount <= 0) return result;

    if (charStart + charCount > _length) {
        charCount = _length - charStart;
    }

    result.grow(charCount);

    for (int i = 0; i < charCount; i++) {
        result._chars[i] = _chars[charStart + i];
    }
    result._length = charCount;

    return result;
}


//-------------------------------------------------------------------------------------------------
// CxUTFString::clear
//
//-------------------------------------------------------------------------------------------------
void
CxUTFString::clear(void)
{
    _length = 0;
    // Note: we keep the allocated capacity for reuse
}


//-------------------------------------------------------------------------------------------------
// CxUTFString::isEmpty
//
//-------------------------------------------------------------------------------------------------
int
CxUTFString::isEmpty(void) const
{
    return _length == 0;
}


//-------------------------------------------------------------------------------------------------
// CxUTFString::isASCII
//
//-------------------------------------------------------------------------------------------------
int
CxUTFString::isASCII(void) const
{
    for (int i = 0; i < _length; i++) {
        if (!_chars[i].isASCII() && !_chars[i].isTab()) {
            return 0;
        }
    }
    return 1;
}


//-------------------------------------------------------------------------------------------------
// CxUTFString::recalculateTabWidths
//
// Recalculate tab display widths based on their positions.
//
//-------------------------------------------------------------------------------------------------
void
CxUTFString::recalculateTabWidths(int tabWidth)
{
    int displayCol = 0;

    for (int i = 0; i < _length; i++) {
        if (_chars[i].isTab()) {
            // Calculate new tab width
            int width = tabWidth - (displayCol % tabWidth);
            if (width == 0) width = tabWidth;

            _chars[i] = CxUTFCharacter::makeTab(width);
            displayCol += width;
        } else {
            displayCol += _chars[i].displayWidth();
        }
    }
}


//-------------------------------------------------------------------------------------------------
// CxUTFString::operator==
//
//-------------------------------------------------------------------------------------------------
int
CxUTFString::operator==(const CxUTFString &s) const
{
    if (_length != s._length) return 0;

    for (int i = 0; i < _length; i++) {
        if (_chars[i] != s._chars[i]) return 0;
    }

    return 1;
}


//-------------------------------------------------------------------------------------------------
// CxUTFString::operator!=
//
//-------------------------------------------------------------------------------------------------
int
CxUTFString::operator!=(const CxUTFString &s) const
{
    return !(*this == s);
}
