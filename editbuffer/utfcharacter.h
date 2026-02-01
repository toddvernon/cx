//-------------------------------------------------------------------------------------------------
//
//  utfcharacter.h
//
//  CxUTFCharacter Class
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
//  CxUTFCharacter represents a single grapheme cluster - what a user perceives as one character.
//  This may contain:
//    - A single ASCII byte (most common case)
//    - A multi-byte UTF-8 sequence (2-4 bytes)
//    - A base character plus one or more combining marks
//    - A tab character with associated display width
//
//  The class handles UTF-8 decoding, combining mark detection, and display width calculation.
//
//-------------------------------------------------------------------------------------------------

#ifndef _CxUTFCharacter_h_
#define _CxUTFCharacter_h_

#define CXUTF_MAX_BYTES 16   // enough for base + multiple combining marks

//-------------------------------------------------------------------------------------------------
// CxUTFCharacter
//
//-------------------------------------------------------------------------------------------------

class CxUTFCharacter
{
  public:

    CxUTFCharacter(void);
    // default constructor - creates empty character

    CxUTFCharacter(const CxUTFCharacter &c);
    // copy constructor

    CxUTFCharacter& operator=(const CxUTFCharacter &c);
    // assignment operator

    int fromUTF8(const char *utf8bytes);
    // Parse a UTF-8 grapheme cluster from the input bytes.
    // Consumes the base character plus any following combining marks.
    // Returns the number of bytes consumed from input.

    static CxUTFCharacter makeTab(int displayWidth);
    // Create a tab character with the specified display width.

    static CxUTFCharacter fromASCII(char c);
    // Create a character from a single ASCII byte.

    static CxUTFCharacter fromCodepoint(unsigned int codepoint);
    // Create a character from a Unicode codepoint.

    int byteCount(void) const;
    // Return the number of bytes in this character.

    int displayWidth(void) const;
    // Return the display width in screen columns.
    // ASCII and most characters return 1.
    // CJK and wide characters return 2.
    // Tabs return their computed width.

    const unsigned char *bytes(void) const;
    // Return pointer to the raw UTF-8 bytes.

    unsigned int codepoint(void) const;
    // Return the primary (first) codepoint of this character.
    // For characters with combining marks, returns the base character.

    int isASCII(void) const;
    // Return true if this is a single ASCII byte (< 0x80).

    int isTab(void) const;
    // Return true if this is a tab character.

    int operator==(const CxUTFCharacter &c) const;
    // Equality comparison - compares bytes.

    int operator!=(const CxUTFCharacter &c) const;
    // Inequality comparison.

  private:

    unsigned char _bytes[CXUTF_MAX_BYTES];
    unsigned char _byteCount;
    unsigned char _displayWidth;
    unsigned char _flags;

    enum Flags {
        FLAG_TAB = 0x01
    };
};


//-------------------------------------------------------------------------------------------------
// UTF-8 utility functions
//-------------------------------------------------------------------------------------------------

int cxUTF8LeadByteLength(unsigned char leadByte);
// Return the expected byte length of a UTF-8 character based on its lead byte.
// Returns 1 for ASCII, 2-4 for multi-byte sequences.
// Returns 1 for invalid lead bytes (treat as single byte).

unsigned int cxUTF8Decode(const unsigned char *bytes, int len);
// Decode UTF-8 bytes into a Unicode codepoint.

int cxUTF8Encode(unsigned int codepoint, unsigned char *outBytes);
// Encode a Unicode codepoint as UTF-8.
// Returns the number of bytes written (1-4).
// outBytes must have room for at least 4 bytes.

int cxUTF8IsCombiningMark(unsigned int codepoint);
// Return true if the codepoint is a combining mark.

int cxUTF8CodepointDisplayWidth(unsigned int codepoint);
// Return the display width of a codepoint (0, 1, or 2).


#endif
