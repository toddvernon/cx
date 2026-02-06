//-------------------------------------------------------------------------------------------------
//
// CxUTFCharacter
//
// CxUTFCharacter Class
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

#include <cx/base/utfcharacter.h>


//-------------------------------------------------------------------------------------------------
// UTF-8 utility functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// cxUTF8LeadByteLength
//
// Return the expected byte length of a UTF-8 character based on its lead byte.
//
//-------------------------------------------------------------------------------------------------
int
cxUTF8LeadByteLength(unsigned char leadByte)
{
    if (leadByte < 0x80) return 1;           // ASCII: 0xxxxxxx
    if ((leadByte & 0xE0) == 0xC0) return 2; // 2-byte: 110xxxxx
    if ((leadByte & 0xF0) == 0xE0) return 3; // 3-byte: 1110xxxx
    if ((leadByte & 0xF8) == 0xF0) return 4; // 4-byte: 11110xxx
    return 1;  // invalid lead byte, treat as single byte
}


//-------------------------------------------------------------------------------------------------
// cxUTF8Decode
//
// Decode UTF-8 bytes into a Unicode codepoint.
//
//-------------------------------------------------------------------------------------------------
unsigned int
cxUTF8Decode(const unsigned char *bytes, int len)
{
    if (len <= 0) return 0;

    if (len == 1) {
        return bytes[0];
    }

    if (len == 2) {
        return ((bytes[0] & 0x1F) << 6) |
               (bytes[1] & 0x3F);
    }

    if (len == 3) {
        return ((bytes[0] & 0x0F) << 12) |
               ((bytes[1] & 0x3F) << 6) |
               (bytes[2] & 0x3F);
    }

    if (len == 4) {
        return ((bytes[0] & 0x07) << 18) |
               ((bytes[1] & 0x3F) << 12) |
               ((bytes[2] & 0x3F) << 6) |
               (bytes[3] & 0x3F);
    }

    return 0;
}


//-------------------------------------------------------------------------------------------------
// cxUTF8Encode
//
// Encode a Unicode codepoint as UTF-8.
// Returns the number of bytes written (1-4).
//
//-------------------------------------------------------------------------------------------------
int
cxUTF8Encode(unsigned int codepoint, unsigned char *outBytes)
{
    if (codepoint < 0x80) {
        outBytes[0] = (unsigned char)codepoint;
        return 1;
    }

    if (codepoint < 0x800) {
        outBytes[0] = (unsigned char)(0xC0 | (codepoint >> 6));
        outBytes[1] = (unsigned char)(0x80 | (codepoint & 0x3F));
        return 2;
    }

    if (codepoint < 0x10000) {
        outBytes[0] = (unsigned char)(0xE0 | (codepoint >> 12));
        outBytes[1] = (unsigned char)(0x80 | ((codepoint >> 6) & 0x3F));
        outBytes[2] = (unsigned char)(0x80 | (codepoint & 0x3F));
        return 3;
    }

    if (codepoint < 0x110000) {
        outBytes[0] = (unsigned char)(0xF0 | (codepoint >> 18));
        outBytes[1] = (unsigned char)(0x80 | ((codepoint >> 12) & 0x3F));
        outBytes[2] = (unsigned char)(0x80 | ((codepoint >> 6) & 0x3F));
        outBytes[3] = (unsigned char)(0x80 | (codepoint & 0x3F));
        return 4;
    }

    // Invalid codepoint, encode as replacement character U+FFFD
    outBytes[0] = 0xEF;
    outBytes[1] = 0xBF;
    outBytes[2] = 0xBD;
    return 3;
}


//-------------------------------------------------------------------------------------------------
// cxUTF8IsCombiningMark
//
// Return true if the codepoint is a combining mark.
// Combining marks attach to the previous base character to form a single grapheme.
//
//-------------------------------------------------------------------------------------------------
int
cxUTF8IsCombiningMark(unsigned int cp)
{
    // Combining Diacritical Marks (common accents)
    if (cp >= 0x0300 && cp <= 0x036F) return 1;

    // Combining Diacritical Marks Extended
    if (cp >= 0x1AB0 && cp <= 0x1AFF) return 1;

    // Combining Diacritical Marks Supplement
    if (cp >= 0x1DC0 && cp <= 0x1DFF) return 1;

    // Combining Diacritical Marks for Symbols
    if (cp >= 0x20D0 && cp <= 0x20FF) return 1;

    // Combining Half Marks
    if (cp >= 0xFE20 && cp <= 0xFE2F) return 1;

    // Cyrillic combining marks
    if (cp >= 0x0483 && cp <= 0x0489) return 1;

    // Hebrew combining marks
    if (cp >= 0x0591 && cp <= 0x05BD) return 1;
    if (cp == 0x05BF) return 1;
    if (cp >= 0x05C1 && cp <= 0x05C2) return 1;
    if (cp >= 0x05C4 && cp <= 0x05C5) return 1;
    if (cp == 0x05C7) return 1;

    // Arabic combining marks
    if (cp >= 0x0610 && cp <= 0x061A) return 1;
    if (cp >= 0x064B && cp <= 0x065F) return 1;
    if (cp == 0x0670) return 1;
    if (cp >= 0x06D6 && cp <= 0x06DC) return 1;
    if (cp >= 0x06DF && cp <= 0x06E4) return 1;
    if (cp >= 0x06E7 && cp <= 0x06E8) return 1;
    if (cp >= 0x06EA && cp <= 0x06ED) return 1;

    // Thai combining marks
    if (cp >= 0x0E31 && cp <= 0x0E3A) return 1;
    if (cp >= 0x0E47 && cp <= 0x0E4E) return 1;

    // Emoji skin tone modifiers
    if (cp >= 0x1F3FB && cp <= 0x1F3FF) return 1;

    // Variation selectors
    if (cp >= 0xFE00 && cp <= 0xFE0F) return 1;

    // Variation selectors supplement
    if (cp >= 0xE0100 && cp <= 0xE01EF) return 1;

    // Zero-width joiner (used in emoji sequences)
    if (cp == 0x200D) return 1;

    return 0;
}


//-------------------------------------------------------------------------------------------------
// cxUTF8CodepointDisplayWidth
//
// Return the display width of a codepoint in screen columns.
// Returns 0 for combining marks and zero-width characters.
// Returns 2 for CJK and other wide characters.
// Returns 1 for everything else.
//
//-------------------------------------------------------------------------------------------------
int
cxUTF8CodepointDisplayWidth(unsigned int cp)
{
    // Zero-width characters
    if (cp == 0x200B) return 0;  // Zero-width space
    if (cp == 0x200C) return 0;  // Zero-width non-joiner
    if (cp == 0x200D) return 0;  // Zero-width joiner
    if (cp == 0xFEFF) return 0;  // BOM / Zero-width no-break space
    if (cp == 0x00AD) return 0;  // Soft hyphen

    // Combining marks have zero width
    if (cxUTF8IsCombiningMark(cp)) return 0;

    // Control characters
    if (cp < 0x20) return 0;
    if (cp >= 0x7F && cp < 0xA0) return 0;

    // CJK double-width ranges
    // Hangul Jamo
    if (cp >= 0x1100 && cp <= 0x115F) return 2;
    if (cp >= 0x2329 && cp <= 0x232A) return 2;

    // CJK Radicals Supplement through Enclosed CJK Letters
    if (cp >= 0x2E80 && cp <= 0x303E) return 2;

    // Hiragana, Katakana, Bopomofo, Hangul Compatibility Jamo, Kanbun, etc.
    if (cp >= 0x3040 && cp <= 0xA4CF) return 2;

    // Hangul Syllables
    if (cp >= 0xAC00 && cp <= 0xD7A3) return 2;

    // CJK Unified Ideographs and extensions
    if (cp >= 0xF900 && cp <= 0xFAFF) return 2;   // CJK Compatibility Ideographs
    if (cp >= 0xFE10 && cp <= 0xFE1F) return 2;   // Vertical forms
    if (cp >= 0xFE30 && cp <= 0xFE6F) return 2;   // CJK Compatibility Forms
    if (cp >= 0xFF00 && cp <= 0xFF60) return 2;   // Fullwidth forms
    if (cp >= 0xFFE0 && cp <= 0xFFE6) return 2;   // Fullwidth forms continued

    // CJK Unified Ideographs Extension B and beyond
    if (cp >= 0x20000 && cp <= 0x2FFFD) return 2;
    if (cp >= 0x30000 && cp <= 0x3FFFD) return 2;

    // Symbol blocks commonly rendered as double-width emoji in terminals
    if (cp >= 0x2600 && cp <= 0x26FF) return 2;   // Miscellaneous Symbols (⚠️, ☀️, ⚡, etc.)
    if (cp >= 0x2700 && cp <= 0x27BF) return 2;   // Dingbats (✅, ✂️, ✈️, ✉️, etc.)
    if (cp >= 0x2B00 && cp <= 0x2BFF) return 2;   // Miscellaneous Symbols and Arrows (⭐, ⬛, ⬜, etc.)

    // Miscellaneous Technical - specific emoji codepoints (not the whole range)
    if (cp == 0x231A || cp == 0x231B) return 2;   // ⌚ Watch, ⌛ Hourglass
    if (cp >= 0x23E9 && cp <= 0x23F3) return 2;   // ⏩⏪⏫⏬⏭⏮⏯⏰⏱⏲⏳
    if (cp >= 0x23F8 && cp <= 0x23FA) return 2;   // ⏸⏹⏺ Pause, Stop, Record

    // Geometric Shapes - specific emoji codepoints (not the whole range)
    if (cp == 0x25AA || cp == 0x25AB) return 2;   // ▪▫ Black/White Small Square
    if (cp == 0x25B6) return 2;                   // ▶ Play button
    if (cp == 0x25C0) return 2;                   // ◀ Reverse button
    if (cp >= 0x25FB && cp <= 0x25FE) return 2;   // ◻◼◽◾ Medium squares

    // Emoji ranges (comprehensive)
    if (cp >= 0x1F000 && cp <= 0x1F0FF) return 2;  // Mahjong 🀀, Domino 🁣, Playing Cards 🂡
    if (cp >= 0x1F100 && cp <= 0x1F1FF) return 2;  // Enclosed Alphanumerics 🅰🅱, Regional Indicators 🇺🇸
    if (cp >= 0x1F200 && cp <= 0x1F2FF) return 2;  // Enclosed Ideographic Supplement 🈚🈲
    if (cp >= 0x1F300 && cp <= 0x1F9FF) return 2;  // Miscellaneous Symbols and Pictographs, Emoticons, etc.
    if (cp >= 0x1FA00 && cp <= 0x1FAFF) return 2;  // Chess, Extended-A

    // Default width is 1
    return 1;
}


//-------------------------------------------------------------------------------------------------
// CxUTFCharacter:: (default constructor)
//
//-------------------------------------------------------------------------------------------------
CxUTFCharacter::CxUTFCharacter(void)
{
    memset(_bytes, 0, CXUTF_MAX_BYTES);
    _byteCount = 0;
    _displayWidth = 0;
    _flags = 0;
}


//-------------------------------------------------------------------------------------------------
// CxUTFCharacter:: (copy constructor)
//
//-------------------------------------------------------------------------------------------------
CxUTFCharacter::CxUTFCharacter(const CxUTFCharacter &c)
{
    if (&c != this) {
        memcpy(_bytes, c._bytes, CXUTF_MAX_BYTES);
        _byteCount = c._byteCount;
        _displayWidth = c._displayWidth;
        _flags = c._flags;
    }
}


//-------------------------------------------------------------------------------------------------
// CxUTFCharacter:: (assignment operator)
//
//-------------------------------------------------------------------------------------------------
CxUTFCharacter&
CxUTFCharacter::operator=(const CxUTFCharacter &c)
{
    if (&c != this) {
        memcpy(_bytes, c._bytes, CXUTF_MAX_BYTES);
        _byteCount = c._byteCount;
        _displayWidth = c._displayWidth;
        _flags = c._flags;
    }
    return *this;
}


//-------------------------------------------------------------------------------------------------
// CxUTFCharacter::fromUTF8
//
// Parse a UTF-8 grapheme cluster from the input bytes.
// A grapheme cluster is a base character followed by zero or more combining marks.
// Returns the number of bytes consumed from input.
//
//-------------------------------------------------------------------------------------------------
int
CxUTFCharacter::fromUTF8(const char *utf8bytes)
{
    const unsigned char *p = (const unsigned char *)utf8bytes;

    if (p == 0 || *p == 0) {
        _byteCount = 0;
        _displayWidth = 0;
        _flags = 0;
        return 0;
    }

    // Read the base character
    int baseLen = cxUTF8LeadByteLength(*p);
    int totalBytes = baseLen;

    // Copy base character bytes
    for (int i = 0; i < baseLen && i < CXUTF_MAX_BYTES; i++) {
        _bytes[i] = p[i];
    }

    // Get the base codepoint for display width calculation
    unsigned int baseCp = cxUTF8Decode(p, baseLen);
    _displayWidth = cxUTF8CodepointDisplayWidth(baseCp);

    p += baseLen;

    // Consume any following combining marks
    while (*p != 0 && totalBytes < CXUTF_MAX_BYTES - 4) {
        int nextLen = cxUTF8LeadByteLength(*p);
        unsigned int nextCp = cxUTF8Decode(p, nextLen);

        if (!cxUTF8IsCombiningMark(nextCp)) {
            // Not a combining mark, stop here
            break;
        }

        // Copy the combining mark bytes
        for (int i = 0; i < nextLen && totalBytes + i < CXUTF_MAX_BYTES; i++) {
            _bytes[totalBytes + i] = p[i];
        }
        totalBytes += nextLen;
        p += nextLen;
    }

    _byteCount = totalBytes;
    _flags = 0;

    return totalBytes;
}


//-------------------------------------------------------------------------------------------------
// CxUTFCharacter::makeTab
//
// Create a tab character with the specified display width.
//
//-------------------------------------------------------------------------------------------------
CxUTFCharacter
CxUTFCharacter::makeTab(int displayWidth)
{
    CxUTFCharacter ch;
    ch._bytes[0] = '\t';
    ch._byteCount = 1;
    ch._displayWidth = (unsigned char)displayWidth;
    ch._flags = FLAG_TAB;
    return ch;
}


//-------------------------------------------------------------------------------------------------
// CxUTFCharacter::fromASCII
//
// Create a character from a single ASCII byte.
//
//-------------------------------------------------------------------------------------------------
CxUTFCharacter
CxUTFCharacter::fromASCII(char c)
{
    CxUTFCharacter ch;
    ch._bytes[0] = (unsigned char)c;
    ch._byteCount = 1;
    ch._displayWidth = 1;
    ch._flags = 0;
    return ch;
}


//-------------------------------------------------------------------------------------------------
// CxUTFCharacter::fromCodepoint
//
// Create a character from a Unicode codepoint.
//
//-------------------------------------------------------------------------------------------------
CxUTFCharacter
CxUTFCharacter::fromCodepoint(unsigned int codepoint)
{
    CxUTFCharacter ch;
    ch._byteCount = cxUTF8Encode(codepoint, ch._bytes);
    ch._displayWidth = cxUTF8CodepointDisplayWidth(codepoint);
    ch._flags = 0;
    return ch;
}


//-------------------------------------------------------------------------------------------------
// CxUTFCharacter::byteCount
//
//-------------------------------------------------------------------------------------------------
int
CxUTFCharacter::byteCount(void) const
{
    return _byteCount;
}


//-------------------------------------------------------------------------------------------------
// CxUTFCharacter::displayWidth
//
//-------------------------------------------------------------------------------------------------
int
CxUTFCharacter::displayWidth(void) const
{
    return _displayWidth;
}


//-------------------------------------------------------------------------------------------------
// CxUTFCharacter::bytes
//
//-------------------------------------------------------------------------------------------------
const unsigned char *
CxUTFCharacter::bytes(void) const
{
    return _bytes;
}


//-------------------------------------------------------------------------------------------------
// CxUTFCharacter::codepoint
//
// Return the primary (first) codepoint of this character.
//
//-------------------------------------------------------------------------------------------------
unsigned int
CxUTFCharacter::codepoint(void) const
{
    if (_byteCount == 0) return 0;
    int len = cxUTF8LeadByteLength(_bytes[0]);
    return cxUTF8Decode(_bytes, len);
}


//-------------------------------------------------------------------------------------------------
// CxUTFCharacter::isASCII
//
//-------------------------------------------------------------------------------------------------
int
CxUTFCharacter::isASCII(void) const
{
    return (_byteCount == 1 && _bytes[0] < 0x80);
}


//-------------------------------------------------------------------------------------------------
// CxUTFCharacter::isTab
//
//-------------------------------------------------------------------------------------------------
int
CxUTFCharacter::isTab(void) const
{
    return (_flags & FLAG_TAB) != 0;
}


//-------------------------------------------------------------------------------------------------
// CxUTFCharacter::operator==
//
//-------------------------------------------------------------------------------------------------
int
CxUTFCharacter::operator==(const CxUTFCharacter &c) const
{
    if (_byteCount != c._byteCount) return 0;
    if (_flags != c._flags) return 0;
    for (int i = 0; i < _byteCount; i++) {
        if (_bytes[i] != c._bytes[i]) return 0;
    }
    return 1;
}


//-------------------------------------------------------------------------------------------------
// CxUTFCharacter::operator!=
//
//-------------------------------------------------------------------------------------------------
int
CxUTFCharacter::operator!=(const CxUTFCharacter &c) const
{
    return !(*this == c);
}
