//-------------------------------------------------------------------------------------------------
//
//  utfeditbuffer.h
//
//  CxUTFEditBuffer Class
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
//  CxUTFEditBuffer is a UTF-8 aware edit buffer that parallels CxEditBuffer.
//  It uses CxUTFString for line storage, which means:
//    - cursor.col is a CHARACTER index, not a byte offset
//    - No tab extension byte handling needed - tabs are single characters
//    - Multi-byte UTF-8 characters work correctly for navigation
//
//  The API matches CxEditBuffer so they can be swapped via typedef.
//
//-------------------------------------------------------------------------------------------------

#ifndef _CxUTFEditBuffer_h_
#define _CxUTFEditBuffer_h_

#include <stdio.h>

#include <cx/base/string.h>
#include <cx/base/file.h>

#include <cx/editbuffer/utfstringlist.h>
#include <cx/editbuffer/edithint.h>
#include <cx/editbuffer/editbufferpos.h>


//-------------------------------------------------------------------------------------------------
// CxUTFEditBuffer
//
//-------------------------------------------------------------------------------------------------

class CxUTFEditBuffer
{
  public:

    enum POSITION {
        POS_VALID_INITIAL,      // initial state, no lines, cursor at 0,0
        POS_VALID_INSERT,       // valid position to insert a character
        POS_VALID_APPEND_COL,   // valid position to append a character (end of line)
        POS_INVALID_ROW,        // beyond the end of the buffer
        POS_INVALID_COL,        // beyond the text in any given line
        POS_INVALID_MID_TAB     // not used in UTF version, kept for API compatibility
    };

    enum ACTION {
        NONE,
        CURSOR_RIGHT,
        CURSOR_LEFT,
        CURSOR_UP,
        CURSOR_DOWN,
        CURSOR_WRAP_DOWN,
        CURSOR_WRAP_UP,
        EXPAND_DOWN,
        SPLIT_LINE,
        JOIN_LINE
    };

    CxUTFEditBuffer(void);
    // constructor

    CxUTFEditBuffer(int tabspaces);
    // constructor setting the tabspaces

    void reset(void);
    // start over with an initialized buffer

    CxUTFEditBuffer(const CxUTFEditBuffer &c);
    // copy constructor

    CxUTFEditBuffer operator=(const CxUTFEditBuffer &c);
    // assignment operator

    CxString positionString(CxUTFEditBuffer::POSITION position);
    // return name of cursor position

    //----- Cursor Navigation -----

    CxEditHint cursorRightRequest(void);
    // move cursor right

    CxEditHint cursorLeftRequest(void);
    // move cursor left

    CxEditHint cursorUpRequest(void);
    // move cursor up

    CxEditHint cursorDownRequest(void);
    // move cursor down

    CxUTFEditBuffer::POSITION evaluatePosition(unsigned long row_, unsigned long col_);
    // evaluate a position in the buffer

    CxUTFEditBuffer::POSITION cursorGotoRequest(unsigned long row_, unsigned long col_);
    // set the cursor at a new position

    CxUTFEditBuffer::POSITION cursorGotoLine(unsigned long row_);
    // goto line (user-friendly version)

    //----- File I/O -----

    int loadText(CxString filepath_, int preload);
    // load edit buffer from a file

    void loadTextFromString(CxString text);
    // load text from a string containing newlines

    void saveText(CxString filepath_);
    // save edit buffer to a file

    //----- Buffer Info -----

    unsigned long numberOfLines(void);
    // returns the count of lines

    unsigned long characterCount(void);
    // returns total character count

    CxUTFString* line(unsigned long row);
    // returns a line of text (NULL if invalid)

    char characterAt(unsigned long row, unsigned long col);
    // returns the character at position (ASCII only, 0 if invalid or non-ASCII)

    //----- Character Input -----

    CxEditHint addCharacter(CxString c);
    // adds the first character of the string

    CxEditHint addCharacter(char c);
    // adds a character at cursor position

    CxEditHint addTab(void);
    // inserts a tab

    CxEditHint addReturn(void);
    // adds a return (newline)

    CxEditHint addBackspace(void);
    // performs backspace

    //----- Text Operations -----

    void setMark(void);
    // set mark to cursor position

    void deleteText(void);
    // delete text between cursor and mark

    CxString copyText(void);
    // copy text between cursor and mark

    CxString cutToMark(void);
    // cut text from cursor to mark

    CxString cutTextToEndOfLine(void);
    // cut text from cursor to end of line

    void insertTextAtCursor(CxString text);
    // insert text at cursor position

    void insertCommentBlock(unsigned long lastCol);
    // insert a C++ comment block

    void pasteFromCutBuffer(CxString text);
    // paste text from cut buffer

    CxString flattenBuffer(void);
    // flatten buffer to a single string with newlines

    //----- Find/Replace -----

    int findString(CxString findString);
    // find string starting at cursor

    int findAgain(CxString findString, int skipIfCurrent);
    // find again

    int replaceString(CxString findString, CxString replaceString);
    // find and replace

    int replaceAgain(CxString findString, CxString replaceString);
    // replace again

    //----- Tab Operations -----

    void entab(void);
    // convert leading spaces to tabs

    void detab(void);
    // convert tabs to spaces

    //----- State -----

    int isReadOnly(void);
    int isTouched(void);
    int isInMemory(void);
    void setReadOnly(int v);

    int isCursorFirstRow(void);
    int isCursorLastRow(void);
    int isCursorFirstCol(void);
    int isCursorLastCol(void);
    int isCursorFirstValidPosition(void);
    int isCursorLastValidPosition(void);
    int isCursorRowEmpty(void);

    int compareCursorToMark(void);

    //----- Visual Screen -----

    void setVisualFirstScreenLine(int line);
    int getVisualFirstScreenLine(void);
    void setVisualFirstScreenCol(int col);
    int getVisualFirstScreenCol(void);

    //----- Path -----

    CxString getFilePath(void);
    void setFilePath(CxString fp);

    //----- Display Column Helpers -----

    int cursorDisplayColumn(void);
    // return the screen column of the cursor

    int displayColumnToCharIndex(unsigned long row, int displayCol);
    // convert display column to character index

    //----- Debug -----

    void debug_print(void);

    //----- Public Members -----

    CxEditBufferPosition cursor;
    CxEditBufferPosition mark;
    CxString name;

  private:

    CxEditBufferPosition getPositionPriorToCursor(void);

    CxEditHint joinLines(void);

    CxUTFStringList _bufferLineList;

    int markSet;
    unsigned long lastRequestCol;

    CxEditBufferPosition lastFindLocation;

    int tabSpaces;
    int visualFirstScreenLine;
    int visualFirstScreenCol;

    CxString filePath;
    int readOnly;
    int touched;
    int inMemory;
};


#endif
