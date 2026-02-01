# UTF-8 Support Implementation Plan

## Overview

Add UTF-8 support to the CX library to allow the CMacs editor to safely edit files containing UTF-8 characters without corrupting them. The goal is **preservation**, not authoring - navigate, delete, copy/paste UTF-8 correctly.

## Architecture

Create parallel UTF-8 aware classes that can be swapped in via `#ifdef` for modern platforms (Linux, macOS), while keeping existing code unchanged for older platforms (IRIX, Solaris).

```
Existing (unchanged)          New UTF-8 Classes
--------------------          -----------------
CxString                      CxUTFCharacter
CxStringList                  CxUTFString
CxEditBuffer                  CxUTFStringList
                              CxUTFEditBuffer
```

## Core Concepts

### CxUTFCharacter
Represents a single **grapheme cluster** - what a user perceives as one character. May contain:
- 1-4 bytes for a single UTF-8 codepoint (most characters)
- Multiple codepoints for base + combining marks (e.g., `e` + accent)
- Tab character with computed display width

### CxUTFString
A line of text stored as an array of `CxUTFCharacter`. Cursor position is an index into this array - no byte-level navigation needed.

### CxUTFEditBuffer
Drop-in replacement for `CxEditBuffer` with the same public API, but using `CxUTFString` for line storage internally.

## Design Decisions

1. **Grapheme clusters**: Each `CxUTFCharacter` holds a complete grapheme (base + combiners). Cursor never lands mid-grapheme.

2. **ASCII fast path**: Most source files are ASCII. Detect ASCII-only lines and optimize storage/navigation for them.

3. **CxString for interchange**: File paths, buffer names, and raw I/O continue using `CxString`. Conversion to/from `CxUTFString` happens at I/O boundaries.

4. **Platform selection**: Use `#ifdef` to select UTF or non-UTF classes at compile time.

---

## Phase 1: CxUTFCharacter

### File: `editbuffer/utfcharacter.h`

```cpp
#ifndef _CxUTFCharacter_h_
#define _CxUTFCharacter_h_

#define CXUTF_MAX_BYTES 16   // enough for base + multiple combiners

class CxUTFCharacter {
public:
    CxUTFCharacter();
    CxUTFCharacter(const CxUTFCharacter &c);
    CxUTFCharacter& operator=(const CxUTFCharacter &c);

    // Construction from UTF-8 bytes
    // Returns number of bytes consumed from input
    int fromUTF8(const char *utf8bytes);

    // Construction for tab with specific display width
    static CxUTFCharacter makeTab(int displayWidth);

    // Construction from ASCII character
    static CxUTFCharacter fromASCII(char c);

    // Accessors
    int byteCount() const;           // raw byte count
    int displayWidth() const;        // screen columns
    const char *bytes() const;       // raw bytes
    unsigned int codepoint() const;  // first/primary codepoint

    // Queries
    int isASCII() const;             // single byte, < 0x80
    int isTab() const;

    // Comparison
    int operator==(const CxUTFCharacter &c) const;
    int operator!=(const CxUTFCharacter &c) const;

private:
    unsigned char _bytes[CXUTF_MAX_BYTES];
    unsigned char _byteCount;
    unsigned char _displayWidth;
};

#endif
```

### File: `editbuffer/utfcharacter.cpp`

Key implementation details:

```cpp
// Determine byte length of UTF-8 character from lead byte
static int utf8LeadByteLength(unsigned char c) {
    if (c < 0x80) return 1;        // ASCII
    if ((c & 0xE0) == 0xC0) return 2;
    if ((c & 0xF0) == 0xE0) return 3;
    if ((c & 0xF8) == 0xF0) return 4;
    return 1;  // invalid, treat as single byte
}

// Check if codepoint is a combining mark
static int isCombiningMark(unsigned int cp) {
    // Combining Diacritical Marks
    if (cp >= 0x0300 && cp <= 0x036F) return 1;
    // Combining Diacritical Marks Extended
    if (cp >= 0x1AB0 && cp <= 0x1AFF) return 1;
    // Combining Diacritical Marks Supplement
    if (cp >= 0x1DC0 && cp <= 0x1DFF) return 1;
    // Combining Diacritical Marks for Symbols
    if (cp >= 0x20D0 && cp <= 0x20FF) return 1;
    // Combining Half Marks
    if (cp >= 0xFE20 && cp <= 0xFE2F) return 1;
    // Emoji skin tone modifiers
    if (cp >= 0x1F3FB && cp <= 0x1F3FF) return 1;
    return 0;
}

// Compute display width of a codepoint
static int codepointDisplayWidth(unsigned int cp) {
    // Zero-width characters
    if (cp == 0x200B || cp == 0x200C || cp == 0x200D || cp == 0xFEFF) return 0;

    // Combining marks (already handled, but safety check)
    if (isCombiningMark(cp)) return 0;

    // CJK double-width ranges (simplified)
    if (cp >= 0x1100 && cp <= 0x115F) return 2;   // Hangul Jamo
    if (cp >= 0x2E80 && cp <= 0x9FFF) return 2;   // CJK
    if (cp >= 0xAC00 && cp <= 0xD7A3) return 2;   // Hangul Syllables
    if (cp >= 0xF900 && cp <= 0xFAFF) return 2;   // CJK Compatibility
    if (cp >= 0xFE10 && cp <= 0xFE1F) return 2;   // Vertical forms
    if (cp >= 0xFE30 && cp <= 0xFE6F) return 2;   // CJK Compatibility Forms
    if (cp >= 0xFF00 && cp <= 0xFF60) return 2;   // Fullwidth forms
    if (cp >= 0x20000 && cp <= 0x2FFFF) return 2; // CJK Extension B+
    if (cp >= 0x30000 && cp <= 0x3FFFF) return 2; // CJK Extension G+

    return 1;
}

// Decode UTF-8 bytes to codepoint
static unsigned int decodeUTF8(const unsigned char *bytes, int len) {
    if (len == 1) return bytes[0];
    if (len == 2) return ((bytes[0] & 0x1F) << 6) | (bytes[1] & 0x3F);
    if (len == 3) return ((bytes[0] & 0x0F) << 12) | ((bytes[1] & 0x3F) << 6) | (bytes[2] & 0x3F);
    if (len == 4) return ((bytes[0] & 0x07) << 18) | ((bytes[1] & 0x3F) << 12) |
                         ((bytes[2] & 0x3F) << 6) | (bytes[3] & 0x3F);
    return 0;
}
```

### Tests for CxUTFCharacter

```cpp
void test_utfcharacter_ascii() {
    CxUTFCharacter ch;
    const char *input = "A";
    int consumed = ch.fromUTF8(input);

    assert(consumed == 1);
    assert(ch.byteCount() == 1);
    assert(ch.displayWidth() == 1);
    assert(ch.isASCII() == 1);
    assert(ch.codepoint() == 'A');
}

void test_utfcharacter_2byte() {
    // U+00E9 LATIN SMALL LETTER E WITH ACUTE (é)
    // UTF-8: C3 A9
    CxUTFCharacter ch;
    const char *input = "\xC3\xA9";
    int consumed = ch.fromUTF8(input);

    assert(consumed == 2);
    assert(ch.byteCount() == 2);
    assert(ch.displayWidth() == 1);
    assert(ch.codepoint() == 0x00E9);
}

void test_utfcharacter_3byte_cjk() {
    // U+4E2D CHINESE CHARACTER (中)
    // UTF-8: E4 B8 AD
    CxUTFCharacter ch;
    const char *input = "\xE4\xB8\xAD";
    int consumed = ch.fromUTF8(input);

    assert(consumed == 3);
    assert(ch.byteCount() == 3);
    assert(ch.displayWidth() == 2);  // CJK is double-width
    assert(ch.codepoint() == 0x4E2D);
}

void test_utfcharacter_4byte_emoji() {
    // U+1F44B WAVING HAND SIGN
    // UTF-8: F0 9F 91 8B
    CxUTFCharacter ch;
    const char *input = "\xF0\x9F\x91\x8B";
    int consumed = ch.fromUTF8(input);

    assert(consumed == 4);
    assert(ch.byteCount() == 4);
    assert(ch.displayWidth() == 2);  // emoji typically double-width
    assert(ch.codepoint() == 0x1F44B);
}

void test_utfcharacter_combining() {
    // e + combining acute accent = é (two codepoints, one grapheme)
    // U+0065 U+0301
    // UTF-8: 65 CC 81
    CxUTFCharacter ch;
    const char *input = "\x65\xCC\x81";
    int consumed = ch.fromUTF8(input);

    assert(consumed == 3);           // consumed both codepoints
    assert(ch.byteCount() == 3);
    assert(ch.displayWidth() == 1);  // one visual character
}

void test_utfcharacter_tab() {
    CxUTFCharacter ch = CxUTFCharacter::makeTab(4);

    assert(ch.isTab() == 1);
    assert(ch.displayWidth() == 4);
    assert(ch.bytes()[0] == '\t');
}

void test_utfcharacter_box_drawing() {
    // U+2500 BOX DRAWINGS LIGHT HORIZONTAL (─)
    // UTF-8: E2 94 80
    CxUTFCharacter ch;
    const char *input = "\xE2\x94\x80";
    int consumed = ch.fromUTF8(input);

    assert(consumed == 3);
    assert(ch.byteCount() == 3);
    assert(ch.displayWidth() == 1);  // box drawing is single-width
    assert(ch.codepoint() == 0x2500);
}
```

---

## Phase 2: CxUTFString

### File: `editbuffer/utfstring.h`

```cpp
#ifndef _CxUTFString_h_
#define _CxUTFString_h_

#include <cx/editbuffer/utfcharacter.h>
#include <cx/base/string.h>

class CxUTFString {
public:
    CxUTFString();
    CxUTFString(const CxUTFString &s);
    CxUTFString& operator=(const CxUTFString &s);
    ~CxUTFString();

    // Construction from raw UTF-8 bytes
    void fromBytes(const char *utf8bytes, int len, int tabWidth);
    void fromCxString(const CxString &s, int tabWidth);

    // Conversion back to bytes
    CxString toBytes() const;

    // Character access
    int charCount() const;                    // grapheme count
    int displayWidth() const;                 // total screen columns
    CxUTFCharacter *at(int charIndex);
    const CxUTFCharacter *at(int charIndex) const;

    // Display column mapping
    int displayWidthAt(int charIndex) const;  // width of char at index
    int displayColumnOfChar(int charIndex) const;  // screen col where char starts
    int charIndexAtDisplayColumn(int displayCol) const;  // reverse mapping

    // Modification
    void insert(int charIndex, const CxUTFCharacter &ch);
    void insert(int charIndex, const CxUTFString &s);
    void remove(int charIndex, int count);
    void append(const CxUTFCharacter &ch);
    void append(const CxUTFString &s);

    // Substring
    CxUTFString subString(int charStart, int charCount) const;

    // Queries
    int isASCII() const;    // all characters are ASCII (optimization hint)
    int isEmpty() const;

private:
    CxUTFCharacter *_chars;
    int _length;
    int _capacity;
    int _isASCII;           // cached flag

    void grow(int minCapacity);
    void updateASCIIFlag();
};

#endif
```

### Tests for CxUTFString

```cpp
void test_utfstring_ascii() {
    CxUTFString s;
    s.fromBytes("Hello", 5, 4);

    assert(s.charCount() == 5);
    assert(s.displayWidth() == 5);
    assert(s.isASCII() == 1);
    assert(s.at(0)->codepoint() == 'H');
    assert(s.at(4)->codepoint() == 'o');
}

void test_utfstring_utf8() {
    // "Héllo" with precomposed é
    CxUTFString s;
    s.fromBytes("H\xC3\xA9llo", 6, 4);

    assert(s.charCount() == 5);       // 5 characters
    assert(s.displayWidth() == 5);    // 5 screen columns
    assert(s.at(1)->codepoint() == 0x00E9);
}

void test_utfstring_cjk() {
    // "A中B" - ASCII, CJK, ASCII
    CxUTFString s;
    s.fromBytes("A\xE4\xB8\xADB", 5, 4);

    assert(s.charCount() == 3);       // 3 characters
    assert(s.displayWidth() == 4);    // 1 + 2 + 1 columns
    assert(s.displayColumnOfChar(0) == 0);  // A at column 0
    assert(s.displayColumnOfChar(1) == 1);  // 中 at column 1
    assert(s.displayColumnOfChar(2) == 3);  // B at column 3
}

void test_utfstring_combining() {
    // "e" + combining acute + "x"
    CxUTFString s;
    s.fromBytes("\x65\xCC\x81x", 4, 4);

    assert(s.charCount() == 2);       // 2 graphemes
    assert(s.displayWidth() == 2);    // 2 screen columns
}

void test_utfstring_tabs() {
    // "A\tB" with tabWidth 4, A at col 0, tab expands to col 4
    CxUTFString s;
    s.fromBytes("A\tB", 3, 4);

    assert(s.charCount() == 3);
    assert(s.at(1)->isTab() == 1);
    assert(s.at(1)->displayWidth() == 3);  // tab from col 1 to col 4
    assert(s.displayColumnOfChar(2) == 4); // B at column 4
}

void test_utfstring_insert() {
    CxUTFString s;
    s.fromBytes("AC", 2, 4);

    CxUTFCharacter b = CxUTFCharacter::fromASCII('B');
    s.insert(1, b);

    assert(s.charCount() == 3);
    assert(s.at(0)->codepoint() == 'A');
    assert(s.at(1)->codepoint() == 'B');
    assert(s.at(2)->codepoint() == 'C');
}

void test_utfstring_remove() {
    CxUTFString s;
    s.fromBytes("ABCD", 4, 4);

    s.remove(1, 2);  // remove "BC"

    assert(s.charCount() == 2);
    assert(s.at(0)->codepoint() == 'A');
    assert(s.at(1)->codepoint() == 'D');
}

void test_utfstring_roundtrip() {
    // Verify toBytes() produces original UTF-8
    const char *original = "Hello \xE4\xB8\xAD \xC3\xA9";  // "Hello 中 é"

    CxUTFString s;
    s.fromBytes(original, strlen(original), 4);

    CxString back = s.toBytes();
    assert(strcmp(back.data(), original) == 0);
}

void test_utfstring_box_drawing() {
    // Table with box drawing: "│cell│"
    const char *input = "\xE2\x94\x82" "cell" "\xE2\x94\x82";

    CxUTFString s;
    s.fromBytes(input, strlen(input), 4);

    assert(s.charCount() == 6);       // │ c e l l │
    assert(s.displayWidth() == 6);    // all single-width
    assert(s.at(0)->codepoint() == 0x2502);
    assert(s.at(5)->codepoint() == 0x2502);
}
```

---

## Phase 3: CxUTFStringList

Simple wrapper around dynamic array of `CxUTFString`, analogous to existing `CxStringList`.

```cpp
class CxUTFStringList {
public:
    CxUTFStringList();
    ~CxUTFStringList();

    unsigned long entries() const;
    CxUTFString *at(unsigned long index);
    void append(const CxUTFString &s);
    void insert(unsigned long index, const CxUTFString &s);
    void removeAt(unsigned long index);
    void clear();

private:
    CxUTFString *_list;
    unsigned long _count;
    unsigned long _capacity;
};
```

---

## Phase 4: CxUTFEditBuffer

Mirror the `CxEditBuffer` API but use `CxUTFString` internally. Key differences in implementation:

1. **Cursor.col** is character index, not byte offset
2. **No tab extension byte handling** - tabs are just characters with display width
3. **Navigation** is simple index arithmetic

```cpp
class CxUTFEditBuffer {
public:
    // Same public API as CxEditBuffer
    CxEditHint cursorRightRequest();
    CxEditHint cursorLeftRequest();
    CxEditHint cursorUpRequest();
    CxEditHint cursorDownRequest();

    CxEditHint addCharacter(char c);
    CxEditHint addReturn();
    CxEditHint addBackspace();
    CxEditHint addTab();

    int loadText(CxString filepath, int preload);
    void saveText(CxString filepath);

    // ... rest of API matches CxEditBuffer

private:
    CxUTFStringList _bufferLineList;  // UTF-8 aware line storage
    // ... rest same as CxEditBuffer
};
```

---

## Phase 5: CM App Integration

### Platform Selection

```cpp
// In cm configuration header
#if defined(_OSX_) || defined(_LINUX_)
    #define CM_UTF8_SUPPORT
#endif
```

### Type Aliases

```cpp
// cm/cmtypes.h
#ifdef CM_UTF8_SUPPORT
    #include <cx/editbuffer/utfeditbuffer.h>
    typedef CxUTFEditBuffer CmEditBuffer;
#else
    #include <cx/editbuffer/editbuffer.h>
    typedef CxEditBuffer CmEditBuffer;
#endif
```

### Minimal Code Changes

Most cm code uses edit buffer through its public API, which remains identical. Changes needed:

1. Replace `CxEditBuffer` with `CmEditBuffer` typedef
2. Screen rendering needs to account for display width vs character count
3. Mouse click → cursor position mapping uses display columns

---

## File Structure

```
editbuffer/
    editbuffer.cpp           # unchanged
    editbuffer.h             # unchanged
    editbufferpos.cpp        # unchanged (shared)
    editbufferpos.h          # unchanged (shared)
    edithint.cpp             # unchanged (shared)
    edithint.h               # unchanged (shared)
    stringlist.cpp           # unchanged
    stringlist.h             # unchanged

    utfcharacter.cpp         # NEW
    utfcharacter.h           # NEW
    utfstring.cpp            # NEW
    utfstring.h              # NEW
    utfstringlist.cpp        # NEW
    utfstringlist.h          # NEW
    utfeditbuffer.cpp        # NEW
    utfeditbuffer.h          # NEW

    tests/                   # NEW
        test_utfcharacter.cpp
        test_utfstring.cpp
        test_utfeditbuffer.cpp
```

---

## Implementation Order

### Step 1: CxUTFCharacter
- [ ] Create `utfcharacter.h` with class definition
- [ ] Implement `utfcharacter.cpp`
- [ ] Write and pass all CxUTFCharacter tests
- [ ] Verify: ASCII, 2-byte, 3-byte, 4-byte, combining marks, tabs

### Step 2: CxUTFString
- [ ] Create `utfstring.h` with class definition
- [ ] Implement `utfstring.cpp`
- [ ] Write and pass all CxUTFString tests
- [ ] Verify: construction, navigation, insert, remove, roundtrip

### Step 3: CxUTFStringList
- [ ] Create `utfstringlist.h` and `utfstringlist.cpp`
- [ ] Basic tests for list operations

### Step 4: CxUTFEditBuffer
- [ ] Create `utfeditbuffer.h` matching CxEditBuffer API
- [ ] Implement cursor navigation methods
- [ ] Implement character insertion/deletion
- [ ] Implement file load/save
- [ ] Port existing editbuffer tests to UTF version

### Step 5: Integration
- [ ] Add CM_UTF8_SUPPORT ifdef structure
- [ ] Update cm app to use typedef
- [ ] Test with UTF-8 files
- [ ] Test rendering of CJK, emoji, box drawing

---

## Test Files

Create test files with known UTF-8 content:

**test_utf8_basic.txt**
```
Hello World
Héllo Wörld
中文测试
emoji: 👋🏽
box: ─│┌┐└┘
```

**test_utf8_combining.txt**
```
café (precomposed)
café (combining)
```

---

## Success Criteria

1. Load a UTF-8 file without corruption
2. Navigate cursor across multi-byte characters correctly
3. Delete a multi-byte character completely (not partial bytes)
4. Copy/paste preserves UTF-8
5. Save file produces identical bytes (no corruption)
6. CJK characters display double-width
7. Box drawing characters work correctly
8. Existing ASCII-only workflows unaffected
9. Old platforms compile and work unchanged
