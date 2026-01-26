//-------------------------------------------------------------------------------------------------
//
// CxEditBuffer
//
// CxEditBuffer Class
//
//-------------------------------------------------------------------------------------------------
//
// (c) Copyright 2022 T.Vernon
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
//
// This class handles all the virtual buffer editing functions.  A buffer is
// a series of line objects.  Each line object is a line of text and a line
// of corrisponding character attrubutes for each character in the line.
//
// Editbuffers never have line termination withing the buffer.  Rather the fact that it is
// and individual line in the buffer indicates it is a logical line.  When buffers are read in
// or written out the line termination is removed or applied respectively.
//
// Edit buffers have a virtual cursor that is always located somewhere.  The cursor moves
// according to expected rules of an editor.  The cursor advances as text is typed.  The
// cursor moves backwards if backspace is typed.  The arrow keys move through the buffer as
// expected.
//
// Code can move the virtual cursor to any location in the buffer by specifying a row, col
// combination that is zero based.  If a location is requested that is not possible invalid
// location is indicated by the return values.  The cursor will never leave the edit area and
// is always located a valid position in the buffer.
//
// Any virtual cursor position in the buffer is defined by one of these states.
//
// 1) POS_VALID_INITIAL     - position (0,0) in the buffer that has no text or
//                            backing store allocated.  After text is entered the buffer
//                            never be in this state again.
//
// 2) POS_VALID_INSERT      - the cursor is currently over a character in the buffer and any
//                            character added will inserted at this location.
//
// 3) POS_VALID_APPEND_COL  - the last position of a text line that cursor is on.  Any character
//                            added will result in an append action as this position is one
//                            col past the backing store of the line.
// ----
//
// 4) POS_INVALID_ROW       - the cursor would be at an invalid row if positioned.  The
//                            edit buffer should never be in this state using normal editing
//                            keys.  This is returned from code that is trying to incorrectly
//                            address a location that does not exist.  The cursor will not
//                            be placed at this location.
//
// 5) POS_INVALID_COL       - the cursor would be at an invalid col if positioned.  The
//                            edit buffer should never be in this state using normal editing
//                            keys.  This is returned from code that is trying to incorrectly
//                            address a location that does not exist.  The cursor will not
//                            be placed at this location.
//
//-------------------------------------------------------------------------------------------------
//
// Tabs are represented in the buffer as a tab character followed by n tab extension
// characters that are decimal 255 (octal 377).  Tabs align on n character columns for instance
// 4, 8 etc.  The number of extension characters are dependant on the column the tab resides
// on.  For instance in the string "A/tboy" the tab characters resides on column 1.  In the buffer
// this will be represented as [A][/t][tab-ext][tab-ext][b][o][y].  The number of tab extension
// characters are the number of characters it takes to get the string to the next character
// multiple of the tab.  These characters are used by the code for correct cursor navigation
// and removed before buffer is written to disk.
//
//
//-------------------------------------------------------------------------------------------------
#include <stdio.h>

#include <cx/base/string.h>
#include <cx/base/slist.h>
#include <cx/base/file.h>
#include <cx/editbuffer/editbufferpos.h>
#include <cx/editbuffer/editbuffer.h>

#define DEBUG_EDIT_BUFFER 1



//-------------------------------------------------------------------------------------------------
// CxEditBuffer:: (constructor)
//
//-------------------------------------------------------------------------------------------------
CxEditBuffer::CxEditBuffer(void):
    name(""),
    filePath(""),
    cursor(CxEditBufferPosition()),
    lastRequestCol(0),
    mark(CxEditBufferPosition()),
    markSet(false),
    _bufferLineList(CxStringList()),
    tabSpaces(4),
    visualFirstScreenLine(0),
    visualFirstScreenCol(0),
    readOnly(false),
    touched(false),
    inMemory(false)
{
    reset();
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer:: (constructor)
//
//-------------------------------------------------------------------------------------------------
CxEditBuffer::CxEditBuffer(int tabspaces):
    name(""),
    filePath(""),
    cursor(CxEditBufferPosition()),
    lastRequestCol(0),
    mark(CxEditBufferPosition()),
    markSet(false),
    _bufferLineList(CxStringList()),
    tabSpaces(4),
    visualFirstScreenLine(0),
    visualFirstScreenCol(0),
    readOnly(false),
    touched(false),
    inMemory(false)
{
    tabSpaces = tabspaces;
    reset();
}

//-------------------------------------------------------------------------------------------------
// CxEditBuffer:: (copy constructor)
//
//-------------------------------------------------------------------------------------------------
CxEditBuffer::CxEditBuffer( const CxEditBuffer& c )
{
    if (&c != this) {

        name    = c.name;
        filePath = c.filePath;
        cursor  = c.cursor;
        markSet = c.markSet;
        mark    = c.mark;
        readOnly = c.readOnly;
        touched  = c.touched;
        inMemory = c.inMemory;
        visualFirstScreenLine = c.visualFirstScreenLine;
        visualFirstScreenCol  = c.visualFirstScreenCol;
        
        lastRequestCol = c.lastRequestCol;
        
        _bufferLineList = c._bufferLineList;

    }
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer:: (assignment operator)
//
//-------------------------------------------------------------------------------------------------
CxEditBuffer
CxEditBuffer::operator=( const CxEditBuffer& c)
{
    if (&c != this) {

        name    = c.name;
        filePath = c.filePath;
        cursor  = c.cursor;
        markSet = c.markSet;
        mark    = c.mark;
        readOnly = c.readOnly;
        touched = c.touched;
        inMemory = c.inMemory;
        
        visualFirstScreenLine = c.visualFirstScreenLine;
        visualFirstScreenCol  = c.visualFirstScreenCol;
        
        lastRequestCol = c.lastRequestCol;
        
        _bufferLineList = c._bufferLineList;
    }

    return( *this );
}

//-------------------------------------------------------------------------------------------------
// CxEditBuffer::reset
//
// put the buffer in its initial state
//
//-------------------------------------------------------------------------------------------------
void
CxEditBuffer::reset(void)
{
    name = "";
    filePath = "";
    cursor = CxEditBufferPosition();
    lastRequestCol = 0;
    mark = CxEditBufferPosition();
    markSet = false ;
    visualFirstScreenLine = 0;
    visualFirstScreenCol  = 0;
    readOnly = false;
    touched  = false;
    inMemory = false;
    _bufferLineList = CxStringList();
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::positionString
//
// return the position in the buffer as a string.  Used for debug
//
//-------------------------------------------------------------------------------------------------
CxString
CxEditBuffer::positionString(CxEditBuffer::POSITION position)
{
    switch( position )
    {
        case POS_VALID_INITIAL:
        {
            return("POS_VALID_INITIAL");
        }
        case POS_VALID_INSERT:
        {
            return("POS_VALID_INSERT");
        }
        case POS_VALID_APPEND_COL:
        {
            return("POS_VALID_APPEND_COL");
        }
        case POS_INVALID_ROW:
        {
            return("POS_INVALID_ROW");
        }
        case POS_INVALID_COL:
        {
            return("POS_INVALID_COL");
        }
        case POS_INVALID_MID_TAB:
        {
            return("POS_INVALID_MID_TAB");
        }
    }

    return("INVALID_RETURN_CODE");
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::isReadOnly
//
// returns true if the buffer is readOnly
//
//-------------------------------------------------------------------------------------------------
int
CxEditBuffer::isReadOnly(void)
{
    return( readOnly );
}

//-------------------------------------------------------------------------------------------------
// CxEditBuffer::isTouched
//
// returns true if the buffer has been changed
//
//-------------------------------------------------------------------------------------------------
int
CxEditBuffer::isTouched(void)
{
    return( touched );
}

//-------------------------------------------------------------------------------------------------
// CxEditBuffer::isInMemory
//
// returns true if the buffer has been loaded from disk
//
//-------------------------------------------------------------------------------------------------
int
CxEditBuffer::isInMemory(void)
{
    return( inMemory );
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::setReadOnly
//
// sets the readOnly flag
//
//-------------------------------------------------------------------------------------------------
void
CxEditBuffer::setReadOnly( int v)
{
    readOnly = v;
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::isCursorFirstRow
//
// returns true if the cursor is currently in teh first row
//
//-------------------------------------------------------------------------------------------------
int
CxEditBuffer::isCursorFirstRow(void)
{
    if (cursor.row == 0) return( true );
    return(false);
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::isCursorLastRow
//
// return true if the cursor is in the last row
//
//-------------------------------------------------------------------------------------------------
int
CxEditBuffer::isCursorLastRow(void)
{
    unsigned long numberOfBufferLines = _bufferLineList.entries();

    // no lines in the buffer yet, so yes we are on the last line
    if (numberOfBufferLines == 0) {
        return(true);
    }

    // there are lines and the cursor is on the last line return true
    if (cursor.row == numberOfBufferLines-1) {
        return( true );
    }

    // all others return false
    return( false );
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::isCursorFirstCol
//
// return true if the cursor is in the first column
//
//-------------------------------------------------------------------------------------------------
int
CxEditBuffer::isCursorFirstCol(void)
{
    if (cursor.col == 0) return( true );
    return( false );
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::isCursorLastCol
//
// return true if the cursor is in the last column
//
//-------------------------------------------------------------------------------------------------
int
CxEditBuffer::isCursorLastCol(void)
{
    CxEditBuffer::POSITION position = evaluatePosition( cursor.row, cursor.col);

    if (position == POS_VALID_APPEND_COL) return( true );
    return(false);
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::exitDumpIfCursorInInvalid
//
//
//-------------------------------------------------------------------------------------------------
void
CxEditBuffer::exitDumpIfCursorIsInvalid( const char* functionName, int lineNumber )
{
#ifdef DEBUG_EDIT_BUFFER
    CxEditBuffer::POSITION position = evaluatePosition( cursor.row, cursor.col);

    if (position == CxEditBuffer::POS_INVALID_MID_TAB){

        printf("ERROR: resulting cursor was invalid\n");
        printf("METHOD: %s, line=%d\n", functionName, lineNumber);

        exit(0);
    }
#endif
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::isCursorLastValidPosition
//
// return true if the cursor is in the last valid position in the edit buffer
//
//-------------------------------------------------------------------------------------------------
int
CxEditBuffer::isCursorLastValidPosition(void)
{
    if (isCursorLastRow() && isCursorLastCol()) return( true );
    return(false);
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::isCursorFirstValidPosition
//
// returns true if the cursor is in the first position
//
//-------------------------------------------------------------------------------------------------
int
CxEditBuffer::isCursorFirstValidPosition(void)
{
    if (isCursorFirstRow() && isCursorFirstCol()) return( true );
    return( false );
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::isCursorRowEmpty
//
// returns true if the cursor line in the buffer is empty.
//
//-------------------------------------------------------------------------------------------------
int
CxEditBuffer::isCursorRowEmpty(void)
{
    // get the number of buffer lines
    unsigned long numberOfBufferLines = _bufferLineList.entries();

    // at this point we know we have a valid line with text (ie not append row0
    // so just check for invalid  column or append column situation
    CxString *line = _bufferLineList.at( cursor.row );

    if (line->length() == 0) return( true );

    return(false);
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::setMark
//
// set the mark to the cursor location
//
//-------------------------------------------------------------------------------------------------
void
CxEditBuffer::setMark(void)
{
    markSet = true;
    mark    = cursor;
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::compareCursorToMark
//
// returns +1 if cursor is beyond mark, returns -1 if cursor is before mark, returns 0 if they
// are the same position
//
//-------------------------------------------------------------------------------------------------
int
CxEditBuffer::compareCursorToMark(void)
{
    if (cursor.row < mark.row) return( -1 );
    if (cursor.row > mark.row) return(  1 );

    // rows are equal
    if (cursor.col < mark.col) return( -1 );
    if (cursor.col > mark.col) return(  1 );

    // both are the same
    return( 0 );
}


//-------------------------------------------------------------------------------------------------
//
// CxEditBuffer::evaluatePosition
//
// evaluate a row and col in the buffer to determin if the position is valid to insert
// or append a character.
//
// if the cursor is in 0,0 there is not a buffer yet, POS_VALID_INITIAL is returned
// if the cursor is one past the last character on a line, POS_VALID_APPEND_COL is returned
// if the cursor somewhere on a text line, POS_VALID_INSERT is returned
//
// Any row and/or col outside the edit buffer returns POS_INVALID_ROW or POS_INVALID_COL
//
//
//-------------------------------------------------------------------------------------------------
//#define DEBUG_POSITION 1

CxEditBuffer::POSITION
CxEditBuffer::evaluatePosition(unsigned long row_, unsigned long col_ )
{
    // get the number of buffer lines
    unsigned long numberOfBufferLines = _bufferLineList.entries();

    // if there are no lines and the cursor is at 0,0 its a new buffer
    // and this is an append row situation
    if ((numberOfBufferLines == 0) && (row_ == 0) && (col_ == 0)) {
        return( CxEditBuffer::POS_VALID_INITIAL );
    }

    // if row is beyond the buffer + the append line
    if (row_ > numberOfBufferLines) {
        return( CxEditBuffer::POS_INVALID_ROW);
    }

    // at this point we know we have a valid line with text (ie not append row0
    // so just check for invalid  column or append column situation
    CxString *line = _bufferLineList.at( row_ );
    unsigned long currentLineLength = line->length();

    // if the col is beyond length of the string and the append positions it invalid
    if (col_ > currentLineLength+1) {
        return( CxEditBuffer::POS_INVALID_COL);
    }

    // if column is the one beyond the text its the append column
    if (col_ == currentLineLength) {
        return( CxEditBuffer::POS_VALID_APPEND_COL);
    }

    // check to see if the location is a tab extension character
    // BUG FIX: was using cursor.col instead of col_ parameter
    if (line->data()[col_] == '\377') {
        return( CxEditBuffer::POS_INVALID_MID_TAB);
    }

    // all other placements are a valid character insert location
    return( CxEditBuffer::POS_VALID_INSERT);
}


//-------------------------------------------------------------------------------------------------
//
// CxEditBuffer::getPositionPriorToCursor
//
// returns the position in the buffer prior (to the left) of the cursor position
//
//-------------------------------------------------------------------------------------------------

CxEditBufferPosition
CxEditBuffer::getPositionPriorToCursor(void)
{
    unsigned long saveRow = cursor.row;
    unsigned long saveCol = cursor.col;

    cursorLeftRequest();

    unsigned long leftRow = cursor.row;
    unsigned long leftCol = cursor.col;

    cursor.row = saveRow;
    cursor.col = saveCol;

    return(CxEditBufferPosition(leftRow, leftCol));
}


//-------------------------------------------------------------------------------------------------
//
// CxEditBuffer::cursorRightRequest
//
// Four things can potentially happen with a cursor right.
// 1) the cursor moves right on the same line becuase there is a character to the right
// 2) the cursor moves right into the append position on the current line
// 3) the cursor wraps to the next line, column zero
// 4) the cursor can't move right as its on the last line, last column
//
//-------------------------------------------------------------------------------------------------
CxEditHint
CxEditBuffer::cursorRightRequest(void)
{
    exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

    unsigned long numberOfBufferLines = _bufferLineList.entries();

    if (numberOfBufferLines==0) {
        return( CxEditHint( CxEditHint::UPDATE_HINT_NONE, CxEditHint::CURSOR_HINT_NONE ));
    }

    CxString *line = _bufferLineList.at( cursor.row );
    unsigned long currentLineLength = line->length();

    // if the cursor is a tab (before we move it) then we need to walk through the tab extensions
    if (line->data()[ cursor.col ] == '\t') {

        do {
            cursor.col = cursor.col + 1;
        } while (line->data()[ cursor.col ] == '\377');

        // report back that a cursor right was completed
        CxEditHint editHint(
            cursor.row,
            cursor.col,
            CxEditHint::UPDATE_HINT_NONE,
            CxEditHint::CURSOR_HINT_JUMP);

        lastRequestCol = cursor.col;

        exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

        return( editHint );
    }

    // calculate a new potential cursor col
    unsigned long newCursorCol = cursor.col + 1;
    unsigned long newCursorRow = cursor.row;

    // check to see if the cursor will stay on the same line
    // this happens if there is an editable position to the right
    // <or> the position to the right is the first position after the
    // buffer (ie the append position)
    if (newCursorCol < currentLineLength + 1) {

        // it does, so just advance it one to the right
        cursor.col = newCursorCol;

        // report back that a cursor right was completed
        CxEditHint editHint(
            cursor.row,
            cursor.col,
            CxEditHint::UPDATE_HINT_NONE,
            CxEditHint::CURSOR_HINT_RIGHT);

        lastRequestCol = cursor.col;

        exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

        return( editHint );

    } else {
    // the cursor will advance to the next line (if there is one)

        newCursorRow = newCursorRow + 1;

        // if the potential new cursor line is greater than
        // the number of lines in the buffer, block the move
        if ( newCursorRow >= numberOfBufferLines) {

            // more ignored
            CxEditHint editHint(
                cursor.row,
                cursor.col,
                CxEditHint::UPDATE_HINT_NONE,
                CxEditHint::CURSOR_HINT_NONE);

            lastRequestCol = cursor.col;

            exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

            return( editHint );
        }
    }

    // move the cursor to the next line, column zero
    cursor.row = newCursorRow;
    cursor.col = 0;

    // more ignored
    CxEditHint editHint(
        cursor.row,
        cursor.col,
        CxEditHint::UPDATE_HINT_NONE,
        CxEditHint::CURSOR_HINT_WRAP_DOWN);

    exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

    lastRequestCol = cursor.col;

    return( editHint );
}


//-------------------------------------------------------------------------------------------------
//
// CxEditBuffer::cursorLeftRequest
//
// Three outcomes can potentially happen with a cursor left.
// 1) the cursor moves left on the same line becuase there is a character to the left
// 2) the cursor wraps to the previous line, at the append position
// 3) the cursor can't move left as its on the first line, first column
//
//-------------------------------------------------------------------------------------------------
CxEditHint
CxEditBuffer::cursorLeftRequest(void)
{
    exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

    CxString *line   = _bufferLineList.at( cursor.row );
    unsigned long length = line->length();

    // check if the cursor is in the first column
    if (cursor.col == 0) {

        // if it is then see if its in the first row
        if (cursor.row == 0) {

            // more ignored
            CxEditHint editHint(
                cursor.row,
                cursor.col,
                CxEditHint::UPDATE_HINT_NONE,
                CxEditHint::CURSOR_HINT_NONE);

            lastRequestCol = cursor.col;

            exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

            // if it is there is nothing else to do
            return( editHint );
        }

        // put cursor on preveous line, insert position
        cursor.row = cursor.row - 1;

        line   = _bufferLineList.at( cursor.row );
        length = line->length();

        cursor.col = length; // insert position

        CxEditHint editHint(
            cursor.row,
            cursor.col,
            CxEditHint::UPDATE_HINT_NONE,
            CxEditHint::CURSOR_HINT_WRAP_UP);

        lastRequestCol = cursor.col;

        exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

        // if it is there is nothing else to do
        return( editHint );
    }

    // cursor is not in column zero so it can move left
    cursor.col = cursor.col - 1;

    if (line->data()[ cursor.col ] == '\377') {

        // while the current character is a tab extension, keep moving back
        while (line->data()[ cursor.col ] == '\377') {

            if (cursor.col == 0) {

                // invalid condition a tab extension can never be the first character on
                // a line

        		exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

                printf("ERROR: first character of line is tab extension\n");
                exit(0);
            }
            cursor.col = cursor.col - 1;
        }

        // more ignored
        CxEditHint editHint(
            cursor.row,
            cursor.col,
            CxEditHint::UPDATE_HINT_NONE,
            CxEditHint::CURSOR_HINT_JUMP);

        lastRequestCol = cursor.col;

        exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

        // if it is there is nothing else to do
        return( editHint );
    }

    exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

    // more ignored
    CxEditHint editHint(
        cursor.row,
        cursor.col,
        CxEditHint::UPDATE_HINT_NONE,
        CxEditHint::CURSOR_HINT_LEFT);

    // if it is there is nothing else to do
    return( editHint );
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::cursorUpRequest
//
// Three outcomes can potentially happen with a cursor left.
// 1) the cursor moves up to a preveous line, same column
// 2) the cursor moves up to a preveous line, moved to tab result column
// 3) the cursor can't move up its on the first line of the buffer
//
//-------------------------------------------------------------------------------------------------
CxEditHint
CxEditBuffer::cursorUpRequest(void)
{
    exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

    if (cursor.row == 0) {

        // more ignored
        CxEditHint editHint(
            cursor.row,
            cursor.col,
            CxEditHint::UPDATE_HINT_NONE,
            CxEditHint::CURSOR_HINT_NONE);

        exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

        // if it is there is nothing else to do
        return( editHint );
    }

    cursor.col = lastRequestCol;
    cursor.row = cursor.row - 1;

    // get the text on the new row
    CxString *line   = _bufferLineList.at( cursor.row );
    unsigned long length = line->length();

    // make sure the cursor is brought back to the append location if too far to the right
    if (cursor.col > length) {
        cursor.col = length;
    }

    // while the current character is a tab extension, keep moving back

    if (line->data()[cursor.col] == '\377') {

        while (line->data()[cursor.col] == '\377') {

            if (cursor.col == 0) {

                // invalid condition a tab extension can never be the first character on
                // a line

                printf("ERROR: first character of line is tab extension\n");
                exit(0);
            }
            cursor.col = cursor.col - 1;
        }

        // more ignored
        CxEditHint editHint(
            cursor.row,
            cursor.col,
            CxEditHint::UPDATE_HINT_NONE,
            CxEditHint::CURSOR_HINT_JUMP);

        exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

        // if it is there is nothing else to do
        return( editHint );
    }

    // more ignored
    CxEditHint editHint(
        cursor.row,
        cursor.col,
        CxEditHint::UPDATE_HINT_NONE,
        CxEditHint::CURSOR_HINT_UP);

    // if it is there is nothing else to do
    return( editHint );

}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::cursorDownRequest
//
// Three outcomes can potentially happen with a cursor left.
// 1) the cursor moves down to the next line, same column
// 2) the cursor moves down to a next line, moved to tab result column
// 3) the cursor can't move down its on the last line of the buffer
//
//-------------------------------------------------------------------------------------------------
CxEditHint
CxEditBuffer::cursorDownRequest(void)
{
    exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

    unsigned long numberOfBufferLines = _bufferLineList.entries();

    // check if we are one line beyond the number of lines
    if (cursor.row == numberOfBufferLines) {

        // move ignored
        CxEditHint editHint(
            cursor.row,
            cursor.col,
            CxEditHint::UPDATE_HINT_NONE,
            CxEditHint::CURSOR_HINT_NONE);

        exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

        // if it is there is nothing else to do
        return( editHint );
    }

    if (numberOfBufferLines == 0) {

        // more ignored
        CxEditHint editHint(
            cursor.row,
            cursor.col,
            CxEditHint::UPDATE_HINT_NONE,
            CxEditHint::CURSOR_HINT_NONE);

        exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

        // if it is there is nothing else to do
        return( editHint );
    }

    // check if we can move down
    if (cursor.row < numberOfBufferLines-1) {
        cursor.row = cursor.row + 1;
    } else {
        // move ignored
        CxEditHint editHint(
            cursor.row,
            cursor.col,
            CxEditHint::UPDATE_HINT_NONE,
            CxEditHint::CURSOR_HINT_NONE);

        exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);


        // if it is there is nothing else to do
        return( editHint );
    }

    cursor.col = lastRequestCol;


    // get the text on the new row
    CxString *line  = _bufferLineList.at( cursor.row );
    unsigned int length = line->length();

    // make sure the cursor is brought back to the append location if too far to the right
    if (cursor.col > length) {
        cursor.col = length;
    }

    if (line->data()[cursor.col] == '\377') {

        // while the current character is a tab extension, keep moving back
        while (line->data()[cursor.col] == '\377') {

            if (cursor.col == 0) {

                // invalid condition a tab extension can never be the first character on
                // a line

                printf("ERROR: first character of line is tab extension\n");
                exit(0);
            }
            cursor.col = cursor.col - 1;
        }

        CxEditHint editHint(
            cursor.row,
            cursor.col,
            CxEditHint::UPDATE_HINT_NONE,
            CxEditHint::CURSOR_HINT_JUMP);

        exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

        return( editHint );
    }

    CxEditHint editHint(
        cursor.row,
        cursor.col,
        CxEditHint::UPDATE_HINT_NONE,
        CxEditHint::CURSOR_HINT_DOWN);

    exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);


    // if it is there is nothing else to do
    return( editHint );
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::loadText
//
// Reads a file character by character into a line removing CR/LF's and changing tabs into
// the internal format of a tab followed by tab extension characters.
//
// Uses lazy loading to minimize memory allocations at load time. Lines are materialized
// on-demand when accessed.
//
//-------------------------------------------------------------------------------------------------
int
CxEditBuffer::loadText(CxString filepath_, int preload)
{
    CxFile inFile;

    filePath = filepath_;

    // open the file
    if (!inFile.open( filePath, "r")) {

        // return false indicating a file was not found
        return( false );
    }

    // if the caller wants the file loaded into memory then do so, otherwise just keep
    // the reference to it to load later.
    if (preload) {

        // Get file size using stat
        struct stat fileStat = inFile.getStat();
        unsigned long fileSize = (unsigned long)fileStat.st_size;

        // Allocate buffer for entire file (+1 for safety)
        char* rawBuffer = new char[fileSize + 1];

        // Read entire file into buffer
        size_t bytesRead = inFile.fread(rawBuffer, 1, fileSize);
        rawBuffer[bytesRead] = '\0';

        // Sanitize buffer: replace high-ASCII bytes (128-254) with spaces.
        // These are typically encoding artifacts (e.g., Mac Roman curly quotes)
        // that can cause display issues. We leave 255 (0xFF) alone since that's
        // our internal tab extension marker.
        for (size_t i = 0; i < bytesRead; i++) {
            unsigned char c = (unsigned char)rawBuffer[i];
            if (c >= 128 && c <= 254) {
                rawBuffer[i] = ' ';
            }
        }

        // Use lazy loading - _bufferLineList takes ownership of rawBuffer
        _bufferLineList.initLazy(rawBuffer, bytesRead, tabSpaces);

        inMemory = TRUE;

    } else {

        inMemory = FALSE;
    }

    // close the file
    inFile.close();

    // put cursor at the beginning
    cursorGotoRequest(0,0);

    exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);


    return( true );
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::loadTextFromString
//
// loads text into the editbuffer from CxString containing newlines.  Each line is processed
// for internal tab protocal. 
//
//-------------------------------------------------------------------------------------------------
void
CxEditBuffer::loadTextFromString( CxString text )
{
    // Sanitize input: replace high-ASCII bytes (128-254) with spaces.
    // Leave 255 (0xFF) alone since that's our internal tab extension marker.
    char *sanitizePtr = text.data();
    while (*sanitizePtr != (char) NULL) {
        unsigned char c = (unsigned char)*sanitizePtr;
        if (c >= 128 && c <= 254) {
            *sanitizePtr = ' ';
        }
        sanitizePtr++;
    }

    // flag that signals when a line of text has been appended
    int lineDone    = false;

    // pointer to the input text
    char *ptr = text.data();
	char *startLinePtr = text.data();
    
    // do forever until we find an end of text NULL
    while ( true ) {
     
        // reset the line done flag
        lineDone = false;
        
        while (!lineDone) {

            // are we pointing at buffer terminator
            if (*ptr == (char) NULL) {
                
                // found end of line so set to true
                lineDone    = true;

				// if there are characters in the line
				if (ptr > startLinePtr) {

                    // create a new CxString on the heap and append
                    CxString *line = new CxString(CxStringUtils::toTabExtensionFormat2(startLinePtr, tabSpaces));

                    // append the buffer line list
                    _bufferLineList.append( line );
                }
                                
                return;
            }
            
    
            // are we pointing at the line terminator
            if (*ptr == '\n') {

				// overwrite the newline with a NULL
				*ptr = (char) NULL;

                // done with line
                lineDone = true;

				CxString *line = NULL;

				int shouldFixTabs = 1;

	            // create a new CxString on the heap and append
				if (shouldFixTabs) {
 	               	line = new CxString(CxStringUtils::toTabExtensionFormat2(startLinePtr, tabSpaces));
				} else {
					line = new CxString(startLinePtr);
				}

                // append the buffer line list
                _bufferLineList.append( line );

				// place the new startptr
				startLinePtr = ptr + 1;              
            }
                
            ptr++;
        }
    }
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::saveText
//
// saves an edit buffer to a file adding CR/LF (depending on setting) and removing tab extension
// characters used internally for display and navitation
//
//-------------------------------------------------------------------------------------------------
void
CxEditBuffer::saveText(CxString filepath_)
{
    CxFile outFile;

    if (!outFile.open( filepath_, "w")) {
        printf("error opening file\n");
        exit(0);
    }

    for (unsigned long row=0; row < _bufferLineList.entries(); row++) {

        CxString line = *(_bufferLineList.at(row));
        line = CxStringUtils::fromTabExtensionFormat2(line);
        outFile.printf("%s\n", line.data() );
    }

    outFile.close();
    
    touched = FALSE;
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::flattenBuffer
//
// flattens the buffer into a giant string
//
//-------------------------------------------------------------------------------------------------
CxString
CxEditBuffer::flattenBuffer(void)
{
    CxString flatBuffer;
    
    for (unsigned long row=0; row < _bufferLineList.entries(); row++) {
        CxString line = *(_bufferLineList.at(row));
        line += "\n";
        flatBuffer += CxStringUtils::fromTabExtensionFormat(line);
    }
    
    return( flatBuffer );
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::findString
//
// This method finds the first occurance of a string in the buffer starting at the cursor
// position.  This method will NOT advance the cursor first if the find location is at the
// cursor because on an initial find its valid to return the current position.
//
//-------------------------------------------------------------------------------------------------
int
CxEditBuffer::findString( CxString findString )
{
    if (findString.length() == 0) return( FALSE );
    
    return( findAgain( findString, FALSE ) );
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::findAgain
//
// This method finds the findString again but also has cached knowledge of the last
// find location and if the cursor is on the last find location is will advance
// the cursor and then do the find.  This is so the user can continue to find and
// not return the same location each time.
//
//-------------------------------------------------------------------------------------------------
int
CxEditBuffer::findAgain( CxString findString, int skipIfCurrent )
{
    if (findString.length() == 0) {
        return( FALSE );
    }

	// make sure we are still in the buffer
	if (cursor.row >= _bufferLineList.entries()) {
		return( FALSE );
	}

    // dispatch to multi-line find if findString contains newlines
    if (findString.firstChar('\n') != -1) {
        return findAgainMultiLine( findString, skipIfCurrent );
    }

    if (cursor == lastFindLocation) {
        if (skipIfCurrent) {
            cursorRightRequest();
        }
    }
    
	// start from the specified position
    for (unsigned long row=cursor.row; row < _bufferLineList.entries(); row++) {
        
        CxString line = *(_bufferLineList.at(row));
        
        int col = line.index( findString, cursor.col );
        
        if (col != -1) {

            cursorGotoRequest( row, col );
            lastFindLocation = cursor;

            return( TRUE );
        }
        
        cursor.col = 0;
    }

	return( FALSE  );
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::replaceString
//
// this does an initial replacement of the findString with the replaceString.  Currently
// this method is functionally equivelent to replaceAgain.  Probably could clean it up.
//
//-------------------------------------------------------------------------------------------------

int
CxEditBuffer::replaceString( CxString findString, CxString replaceString )
{
    if (readOnly) return( FALSE );
    
    if (findString.length() == 0) {
        return( FALSE );
    }
    
    return( replaceAgain(  findString, replaceString ) );
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::replaceAgain
//
// this does an followup replacement of the findString with the replaceString.  Currently
// this method is functionally equivelent to replaceString.  Probably could clean it up.
//
//-------------------------------------------------------------------------------------------------

int
CxEditBuffer::replaceAgain( CxString findString, CxString replaceString  )
{
    
    if (readOnly) return (FALSE);
    
    //---------------------------------------------------------------------------------------------
    // has to be an active find string
    //---------------------------------------------------------------------------------------------
    if (findString.length() == 0) {
        return( FALSE );
    }
    
    //---------------------------------------------------------------------------------------------
    // has to be an active replace string (unless multi-line find, allow empty replace to delete)
    //---------------------------------------------------------------------------------------------
    if (replaceString.length() == 0 && findString.firstChar('\n') == -1) {
        return( FALSE );
    }

    //---------------------------------------------------------------------------------------------
    // make sure we are still in the buffer
    //---------------------------------------------------------------------------------------------
    if (cursor.row >= _bufferLineList.entries()) {
        return( FALSE );
    }

    // dispatch to multi-line replace if findString contains newlines
    if (findString.firstChar('\n') != -1) {
        return replaceAgainMultiLine( findString, replaceString );
    }
    
    //---------------------------------------------------------------------------------------------
    // get the current line we do different things depending on where the cursor is placed in
    // reference to the find string.
    //---------------------------------------------------------------------------------------------
    CxString line = *(_bufferLineList.at(cursor.row));
    int col = line.index( findString, cursor.col );
    
    //---------------------------------------------------------------------------------------------
    // if the find column is where the cursor is sitting, then we will do a
    // replacement, and then another find that will attempt to leave the cursor on the next
    // occurance of the find string.
    //---------------------------------------------------------------------------------------------
    if (col == cursor.col) {
   
        touched = TRUE;
  
#ifdef NEWCODE      
		line.findAndReplaceFirst( findString, replaceString, col);
#endif

#define OLDCODE
#ifdef OLDCODE
        //-----------------------------------------------------------------------------------------
        // cut the findString out of the line
        //-----------------------------------------------------------------------------------------
        line = line.remove( col, findString.length() );
        
        //-----------------------------------------------------------------------------------------
        // insert the replaceString in the position
        //-----------------------------------------------------------------------------------------
        line.insert( replaceString, col);

#endif

        //-----------------------------------------------------------------------------------------
        // advance cursor past the replacement to avoid finding within replacement
        // (important for replace-all when replacement contains the find string)
        //-----------------------------------------------------------------------------------------
        cursor.col = col + (int) replaceString.length();

        //-----------------------------------------------------------------------------------------
        // fix the tabs as they may have changed
        //-----------------------------------------------------------------------------------------
        line = CxStringUtils::fixTabs(line, tabSpaces);

        //-----------------------------------------------------------------------------------------
        // new a new line with the changes and insert in location
        //-----------------------------------------------------------------------------------------
        CxString *oldString =
        _bufferLineList.replaceAt( cursor.row, new CxString( line ) );
        delete oldString;
    }
    
    //---------------------------------------------------------------------------------------------
    // now just repeast the find
    //---------------------------------------------------------------------------------------------

    return( findAgain( findString, TRUE ) );

}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::findAgainMultiLine
//
// Multi-line find.  The findString contains embedded '\n' characters representing line
// boundaries.  The algorithm splits the findString into segments and checks that:
//   - segment 0 matches at the END of a line
//   - middle segments match the ENTIRE content of their respective lines
//   - the last segment matches at the START of the last line
//
//-------------------------------------------------------------------------------------------------
int
CxEditBuffer::findAgainMultiLine( CxString findString, int skipIfCurrent )
{
    // split findString by '\n' into segments (max 64)
    CxString segments[64];
    int segCount = 0;
    {
        CxString remaining = findString;
        int pos;
        while ((pos = remaining.firstChar('\n')) != -1 && segCount < 63) {
            segments[segCount] = remaining.subString(0, pos);
            segCount++;
            remaining = remaining.subString(pos + 1, remaining.length() - pos - 1);
        }
        segments[segCount] = remaining;
        segCount++;
    }

    if (segCount < 2) {
        // no newline found, should not happen since caller checked
        return( FALSE );
    }

    int linesToSpan = segCount - 1;

    // handle skipIfCurrent: if cursor is at last find location, advance
    if (cursor == lastFindLocation) {
        if (skipIfCurrent) {
            cursorRightRequest();
        }
    }

    // search from cursor position
    unsigned long startRow = cursor.row;
    unsigned long startCol = cursor.col;

    for (unsigned long row = startRow; row < _bufferLineList.entries(); row++) {

        // check if enough lines remain for the match
        if (row + linesToSpan >= _bufferLineList.entries()) {
            break;
        }

        CxString *linePtr = _bufferLineList.at(row);
        if (linePtr == NULL) continue;
        CxString firstLine = *linePtr;

        // determine starting column for search on first row
        unsigned long searchStartCol = (row == startRow) ? startCol : 0;

        // segment 0 must match at the end of this line
        int seg0Len = segments[0].length();

        // find positions where seg0 could match at or after searchStartCol
        // and the match extends to end of line
        unsigned long matchCol;
        if (seg0Len == 0) {
            // empty first segment means match starts at EOL
            if ((unsigned long)firstLine.length() < searchStartCol) continue;
            matchCol = firstLine.length();
        } else {
            // seg0 must appear at the end of the line
            int endPos = firstLine.length() - seg0Len;
            if (endPos < 0) continue;
            if ((unsigned long)endPos < searchStartCol) continue;

            // verify seg0 matches at position endPos
            CxString tail = firstLine.subString(endPos, seg0Len);
            if (!(tail == segments[0])) continue;
            matchCol = endPos;
        }

        // check middle segments (1..segCount-2) match entire lines
        int middleMatch = TRUE;
        for (int s = 1; s < segCount - 1; s++) {
            CxString *midPtr = _bufferLineList.at(row + s);
            if (midPtr == NULL) { middleMatch = FALSE; break; }
            CxString midLine = *midPtr;

            if (segments[s].length() == 0) {
                // empty middle segment means line must be empty
                if (midLine.length() != 0) { middleMatch = FALSE; break; }
            } else {
                if (!(midLine == segments[s])) { middleMatch = FALSE; break; }
            }
        }
        if (!middleMatch) continue;

        // check last segment matches at the start of the last line
        int lastSegIdx = segCount - 1;
        int lastSegLen = segments[lastSegIdx].length();

        if (lastSegLen > 0) {
            CxString *lastPtr = _bufferLineList.at(row + linesToSpan);
            if (lastPtr == NULL) continue;
            CxString lastLine = *lastPtr;

            if (lastLine.length() < (unsigned long)lastSegLen) continue;
            CxString head = lastLine.subString(0, lastSegLen);
            if (!(head == segments[lastSegIdx])) continue;
        }
        // if lastSegLen == 0 (trailing newline), the match just consumes the newline
        // which we already verified exists since row+linesToSpan < entries()

        // match found
        cursorGotoRequest( row, matchCol );
        lastFindLocation = cursor;
        return( TRUE );
    }

    return( FALSE );
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::replaceAgainMultiLine
//
// Multi-line replace.  If the cursor is at a valid multi-line match, replaces the matched
// region with the replaceString (which may itself contain '\n' for multi-line output).
// Then advances to the next match.
//
//-------------------------------------------------------------------------------------------------
int
CxEditBuffer::replaceAgainMultiLine( CxString findString, CxString replaceString )
{
    // split findString into segments
    CxString segments[64];
    int segCount = 0;
    {
        CxString remaining = findString;
        int pos;
        while ((pos = remaining.firstChar('\n')) != -1 && segCount < 63) {
            segments[segCount] = remaining.subString(0, pos);
            segCount++;
            remaining = remaining.subString(pos + 1, remaining.length() - pos - 1);
        }
        segments[segCount] = remaining;
        segCount++;
    }

    if (segCount < 2) {
        return( FALSE );
    }

    int linesToSpan = segCount - 1;

    // verify cursor is at a valid match
    unsigned long matchRow = cursor.row;
    unsigned long matchCol = cursor.col;

    // check bounds
    if (matchRow + linesToSpan >= _bufferLineList.entries()) {
        // not at a valid match, just find next
        return findAgainMultiLine( findString, TRUE );
    }

    // verify segment 0 matches at end of first line
    CxString *firstPtr = _bufferLineList.at(matchRow);
    if (firstPtr == NULL) return findAgainMultiLine( findString, TRUE );
    CxString firstLine = *firstPtr;

    int seg0Len = segments[0].length();
    if (seg0Len == 0) {
        // empty first segment: matchCol must be at EOL
        if (matchCol != (unsigned long)firstLine.length()) {
            return findAgainMultiLine( findString, TRUE );
        }
    } else {
        // seg0 must match at matchCol and extend to end of line
        if ((int)matchCol + seg0Len != firstLine.length()) {
            return findAgainMultiLine( findString, TRUE );
        }
        CxString tail = firstLine.subString(matchCol, seg0Len);
        if (!(tail == segments[0])) {
            return findAgainMultiLine( findString, TRUE );
        }
    }

    // verify middle segments
    for (int s = 1; s < segCount - 1; s++) {
        CxString *midPtr = _bufferLineList.at(matchRow + s);
        if (midPtr == NULL) return findAgainMultiLine( findString, TRUE );
        CxString midLine = *midPtr;
        if (segments[s].length() == 0) {
            if (midLine.length() != 0) return findAgainMultiLine( findString, TRUE );
        } else {
            if (!(midLine == segments[s])) return findAgainMultiLine( findString, TRUE );
        }
    }

    // verify last segment
    int lastSegIdx = segCount - 1;
    int lastSegLen = segments[lastSegIdx].length();
    CxString suffix;

    CxString *lastPtr = _bufferLineList.at(matchRow + linesToSpan);
    if (lastPtr == NULL) return findAgainMultiLine( findString, TRUE );
    CxString lastLine = *lastPtr;

    if (lastSegLen > 0) {
        if (lastLine.length() < (unsigned long)lastSegLen) {
            return findAgainMultiLine( findString, TRUE );
        }
        CxString head = lastLine.subString(0, lastSegLen);
        if (!(head == segments[lastSegIdx])) {
            return findAgainMultiLine( findString, TRUE );
        }
        // suffix is the rest of the last line after the match
        suffix = lastLine.subString(lastSegLen, lastLine.length() - lastSegLen);
    } else {
        // empty last segment means we consume the newline, suffix is entire last line
        suffix = lastLine;
    }

    // at a valid match — perform the replacement
    touched = TRUE;

    // prefix is the first line content before the match
    CxString prefix = firstLine.subString(0, matchCol);

    // remove the spanned lines (lines matchRow+1 through matchRow+linesToSpan)
    for (int i = 0; i < linesToSpan; i++) {
        CxString *removed = _bufferLineList.removeAt( matchRow + 1 );
        if (removed) delete removed;
    }

    // split replaceString by '\n' into replacement segments
    CxString replSegs[64];
    int replSegCount = 0;
    {
        CxString remaining = replaceString;
        int pos;
        while ((pos = remaining.firstChar('\n')) != -1 && replSegCount < 63) {
            replSegs[replSegCount] = remaining.subString(0, pos);
            replSegCount++;
            remaining = remaining.subString(pos + 1, remaining.length() - pos - 1);
        }
        replSegs[replSegCount] = remaining;
        replSegCount++;
    }

    if (replSegCount == 1) {
        // single output line: prefix + replSegs[0] + suffix
        CxString newLine = prefix + replSegs[0] + suffix;
        newLine = CxStringUtils::fixTabs(newLine, tabSpaces);
        CxString *old = _bufferLineList.replaceAt( matchRow, new CxString(newLine) );
        if (old) delete old;

        // position cursor after replacement text
        cursor.row = matchRow;
        cursor.col = prefix.length() + replSegs[0].length();
    } else {
        // multiple output lines
        // first line: prefix + replSegs[0]
        CxString firstNewLine = prefix + replSegs[0];
        firstNewLine = CxStringUtils::fixTabs(firstNewLine, tabSpaces);
        CxString *old = _bufferLineList.replaceAt( matchRow, new CxString(firstNewLine) );
        if (old) delete old;

        // middle lines: replSegs[1..N-2]
        int insertIdx = matchRow;
        for (int s = 1; s < replSegCount - 1; s++) {
            CxString midNewLine = replSegs[s];
            midNewLine = CxStringUtils::fixTabs(midNewLine, tabSpaces);
            _bufferLineList.insertAfter( insertIdx, new CxString(midNewLine) );
            insertIdx++;
        }

        // last line: replSegs[N-1] + suffix
        CxString lastNewLine = replSegs[replSegCount - 1] + suffix;
        lastNewLine = CxStringUtils::fixTabs(lastNewLine, tabSpaces);
        _bufferLineList.insertAfter( insertIdx, new CxString(lastNewLine) );
        insertIdx++;

        // position cursor after the last replacement segment
        cursor.row = insertIdx;
        cursor.col = replSegs[replSegCount - 1].length();
    }

    // find the next match
    return findAgainMultiLine( findString, TRUE );
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::numberOfLines
//
// returns the number of edit buffer lines
//
//-------------------------------------------------------------------------------------------------
unsigned long
CxEditBuffer::numberOfLines( void )
{
    return( _bufferLineList.entries() );
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::characterCount
//
// returns the total character count excluding internal tab extension markers (0xFF)
// includes newlines between lines
//
//-------------------------------------------------------------------------------------------------
unsigned long
CxEditBuffer::characterCount( void )
{
    unsigned long count = 0;

    for (unsigned long row = 0; row < _bufferLineList.entries(); row++) {
        CxString *line = _bufferLineList.at(row);
        if (line != NULL) {
            for (unsigned long col = 0; col < line->length(); col++) {
                unsigned char c = (unsigned char) line->charAt(col);
                if (c != 0xFF) {
                    count++;
                }
            }
        }
        count++;  // count the newline
    }

    return( count );
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::rawLengthOfBuffer
//
// returns the raw number of characters in the buffer.  Note this call returns the length
// with tab extension characters, minus the CR's as they are not present in the buffer.
//
//-------------------------------------------------------------------------------------------------
unsigned long
CxEditBuffer::rawLengthOfBuffer(void)
{
    unsigned long length = 0;
    
    for (unsigned long row=0; row < _bufferLineList.entries(); row++) {
        length += (_bufferLineList.at(row))->length();
    }
    
    return( length );
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::characterClassAt
//
// This is used for debug and unit tests to figure out the class of character in the
// buffer at a specific location.
//
//-------------------------------------------------------------------------------------------------
CxString
CxEditBuffer::characterClassAt( unsigned long row, unsigned long col)
{
    unsigned long numberOfBuffers = _bufferLineList.entries();

    // the case where there are no buffers
    if (numberOfBuffers == 0) {
        return("<APPROW>");
    }

    // the case where the cursor is at the end of the buffer
    if ((cursor.row == numberOfBuffers) && (cursor.col == 0)) {
        return("<APPROW>");
    }

    // case where cursor is beyond the last row of the buffer
    if (cursor.row > numberOfBuffers) {
        return("<INSLIN>");
    }

    // get the line element
    CxString *line = _bufferLineList.at( cursor.row );

    // get the string
    //CxString existingString = line->text();

    // the case where the column is the one beyond the end of the string
    if (cursor.col == line->length()) {
        return("<APPCOL>");
    }

    // the case where the cursor is beyond the buffer (more than one)
    if (cursor.col > line->length()) {
        return("<INVCOL>");
    }

    // print the character under the cursor
    char buffer[20];

    char theChar = line->data()[cursor.col];

    if (theChar == '\t')   {
        sprintf(buffer, " [TAB]  ");
    } else if (theChar == '\377') {
        sprintf(buffer, "[TABEXT]");
    } else {
        sprintf(buffer, " [ %c ]  ", theChar);
    }

    return(buffer);
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::printCharacterAtCursor
//
// debug method to print the character under the cursor.  The position is not assumed to
// be valid.
//
//-------------------------------------------------------------------------------------------------
void
CxEditBuffer::printCharacterAtCursor(void)
{
    unsigned long numberOfBuffers = _bufferLineList.entries();

    // the case where there are no buffers
    if (numberOfBuffers == 0) {
        printf("<APPEND_ROW:(%lu:%lu)>", cursor.row, cursor.col);
        return;
    }

    // the case where the cursor is at the end of the buffer
    if ((cursor.row == numberOfBuffers) && (cursor.col == 0)) {
        printf("<APPEND_ROW:(%lu:%lu)>", cursor.row, cursor.col);
        return;
    }

    // case where cursor is beyond the last row of the buffer
    if (cursor.row > numberOfBuffers) {
        printf("<ILLEGAL_ROW:(%lu:%lu)", cursor.row, cursor.col);
        return;
    }

    // get the line element
    CxString *line = _bufferLineList.at( cursor.row );

    // the case where the column is the one beyond the end of the string
    if (cursor.col == line->length()) {
        printf("<APPEND_COL:(%lu:%lu)>", cursor.row, cursor.col);
        return;
    }

    // the case where the cursor is beyond the buffer (more than one)
    if (cursor.col > line->length()) {
        printf("<ILLEGAL_COL:(%lu:%lu)>", cursor.row, cursor.col);
        return;
    }

    // print the character under the cursor
    printf("<CHAR[%c]:(%lu:%lu)>", line->data()[cursor.col], cursor.row, cursor.col);
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::debug_print
//
// crude metho to print lines in the buffer.  Does not sub out tab extensions so output
// can be a bit garbled.
//
//-------------------------------------------------------------------------------------------------
void
CxEditBuffer::debug_print(void)
{
    printf("number of buffers = %lu\n", _bufferLineList.entries());

	for (unsigned long i=0; i<_bufferLineList.entries(); i++) {
        printf("b[%lu]=[", i);
        debug_print_string(i);
        printf("]\n");
	}
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::debug_print_string
//
// prints a line in the buffer in debug format showing tabs as "!" and tab extension characters
// as "^" symbols.
//
//-------------------------------------------------------------------------------------------------
void
CxEditBuffer::debug_print_string( unsigned long lineNumber)
{
    CxString *bufferLine = _bufferLineList.at( lineNumber );

    for (int c=0; c<bufferLine->length(); c++) {

        char ch = bufferLine->charAt(c);

        if (ch=='\t') {
            printf("|");
        } else if (ch == '\377'){
            printf("^");
        } else {
            printf("%c", ch);
        }
    }
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::cursorGotoRequest
//
// goto a row, col in the edit buffer.  Position must be valid
//
//-------------------------------------------------------------------------------------------------
CxEditBuffer::POSITION
CxEditBuffer::cursorGotoRequest(unsigned long row_, unsigned long col_ )
{

    exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

    CxEditBuffer::POSITION position = evaluatePosition( row_, col_);

    if ((position == CxEditBuffer::POS_VALID_INSERT) ||
        (position == CxEditBuffer::POS_VALID_INITIAL) ||
        (position == CxEditBuffer::POS_VALID_APPEND_COL) ) {

        cursor.row = row_;
        cursor.col = col_;

        exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

        lastRequestCol = cursor.col;
    }


    exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);


    if (position == CxEditBuffer::POS_INVALID_MID_TAB) {

        cursor.row = row_;
        cursor.col = 0;
    }

    return( position );
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::cursorGotoLine
//
//
//
//-------------------------------------------------------------------------------------------------
CxEditBuffer::POSITION
CxEditBuffer::cursorGotoLine( unsigned long row_)
{

    exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

    // get the number of buffer lines
    unsigned long numberOfBufferLines = _bufferLineList.entries();

    if (numberOfBufferLines == 0) {
        row_ = 0;
    }

    if (row_ > (numberOfBufferLines-1)) {
        row_ = (numberOfBufferLines-1);
    }

   return( cursorGotoRequest( row_, 0));
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::joinLines
//
// Used in the case of backspace in the column 0 of the buffer.  This action joins the line
// the backspace occured on witht he preveous line. This removes the orginal line resulting
// in one less lines in the file and cursor at the insert point of the append col on
// the preveous line.
//
//-------------------------------------------------------------------------------------------------
CxEditHint
CxEditBuffer::joinLines( )
{
    exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

    CxEditBuffer::POSITION position = evaluatePosition( cursor.row, cursor.col);

    // we are either joining an entire of line of to the preveous or we
    // joining an empty line to the preveous line
    if (position == CxEditBuffer::POS_VALID_INSERT) {

        // can't join if cursor isn't on the zero row
        if (cursor.row == 0) {

            CxEditHint editHint(
                cursor.row,
                cursor.col,
                CxEditHint::UPDATE_HINT_NONE,
                CxEditHint::CURSOR_HINT_NONE);

            lastRequestCol = cursor.col;

            exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

            // if it is there is nothing else to do
            return( editHint );
        }

        CxString *prevLine = _bufferLineList.at( cursor.row-1 );
        CxString *line     = _bufferLineList.at( cursor.row );

        // calculate where the cursor will be when we are done
        unsigned newCursorRow = cursor.row - 1;
        unsigned newCursorCol = prevLine->length();

        // put the prev line in working text
        CxString workingText = *prevLine;

        // append the line to the last line
        workingText.append( line->data() );

        // fix up tabs
        workingText = CxStringUtils::fixTabs( workingText, tabSpaces );

        // replace the string in teh buffer and delete the old version
        CxString *oldString =
            _bufferLineList.replaceAt( cursor.row-1, new CxString(workingText));
        delete oldString;

        // remove the current line
        CxString *deletedString = _bufferLineList.removeAt(cursor.row );
        delete deletedString;

        cursor.row = newCursorRow;
        cursor.col = newCursorCol;

        CxEditHint editHint(
            cursor.row,
            cursor.col,
            CxEditHint::UPDATE_HINT_SCREEN_PAST_POINT,
            CxEditHint::CURSOR_HINT_JUMP);

        lastRequestCol = cursor.col;

        exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

        touched = TRUE;
        
        // if it is there is nothing else to do
        return( editHint );
    }

    // we are either joining an entire of line of to the preveous or we
    // joining an empty line to the preveous line
    if (position == CxEditBuffer::POS_VALID_APPEND_COL) {

        // can't join if cursor isn't on the zero row
        if (cursor.row == 0) {

            CxEditHint editHint(
                cursor.row,
                cursor.col,
                CxEditHint::UPDATE_HINT_NONE,
                CxEditHint::CURSOR_HINT_NONE);

            lastRequestCol = cursor.col;

            exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

            // if it is there is nothing else to do
            return( editHint );
        }

        CxString *prevLine = _bufferLineList.at( cursor.row-1 );
        CxString *line     = _bufferLineList.at( cursor.row );

        // calculate where the cursor will be when we are done
        unsigned newCursorRow = cursor.row - 1;
        unsigned newCursorCol = prevLine->length();

        // put the prev line in working text
        CxString workingText = *prevLine;

        // append the line to the last line
        workingText.append( *line );

        // fix up tabs
        workingText = CxStringUtils::fixTabs( workingText, tabSpaces );

        // append the current line to the preveous line
        CxString *oldString =
            _bufferLineList.replaceAt( cursor.row-1, new CxString( workingText) );
        delete oldString;


        // remove the current line
        CxString *deletedString = _bufferLineList.removeAt(cursor.row );
        delete deletedString;

        cursor.row = newCursorRow;
        cursor.col = newCursorCol;

        CxEditHint editHint(
            cursor.row,
            cursor.col,
            CxEditHint::UPDATE_HINT_SCREEN_PAST_POINT,
            CxEditHint::CURSOR_HINT_JUMP);

        lastRequestCol = cursor.col;

        exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

        touched = TRUE;
        
        // if it is there is nothing else to do
        return( editHint );
    }


    exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);


    if (position == CxEditBuffer::POS_INVALID_MID_TAB){
        printf("ERROR invalid mid tab position in ::joinLines\n");
        exit(0);
    }

    CxEditHint editHint(
        cursor.row,
        cursor.col,
        CxEditHint::UPDATE_HINT_NONE,
        CxEditHint::CURSOR_HINT_LEFT);

    lastRequestCol = cursor.col;


    // if it is there is nothing else to do
    return( editHint );
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::addBackspace
//
// performs a backspace action on the buffer at the current cursor location.  Many outcomes
// are possible.
//
//-------------------------------------------------------------------------------------------------
CxEditHint
CxEditBuffer::addBackspace(void)
{
    if (readOnly) {
        CxEditHint editHint(
            cursor.row,
            cursor.col-1,
            CxEditHint::UPDATE_HINT_NONE,
            CxEditHint::CURSOR_HINT_NONE);
        return( editHint );
    }
    
    exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

    CxEditBuffer::POSITION position = evaluatePosition( cursor.row, cursor.col);

    //printf("pos=%d\n", position);

    // cursor is over an existing character in the buffer
    if ((position == CxEditBuffer::POS_VALID_INSERT) ||
       ( position == CxEditBuffer::POS_VALID_APPEND_COL)) {

        // this is the case where the cursor is in the zero column
        // but there is text after the cursor.  This means we should call join
        // to join the remainder of the characters in this line with the preveous line
        if (cursor.col == 0) {

        	exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);
            return ( joinLines(  ) );
        }

        // get the line element
        CxString *line = _bufferLineList.at( cursor.row );

        // get the text
        CxString text = line->data();

        // if the character to the left is a tab-extension, then eat those
        // up
        if (line->data()[cursor.col-1] == '\377') {

            // move cursor back one col
            cursor.col = cursor.col - 1;

            // while the current character is a tab extension, keep moving back
            while (line->data()[cursor.col] == '\377') {

                // get the string
                text = text.remove( cursor.col-1, 1 );

                // fix up the tabs
                text = CxStringUtils::fixTabs( text, tabSpaces );

                // backup one
                cursor.col = cursor.col - 1;
            }


            CxString *oldString =
                _bufferLineList.replaceAt( cursor.row, new CxString( text ));
            delete oldString;


            CxEditHint editHint(
                cursor.row,
                cursor.col,
                CxEditHint::UPDATE_HINT_LINE,
                CxEditHint::CURSOR_HINT_JUMP);

            lastRequestCol = cursor.col;


	        exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

            touched = TRUE;
            
            // if it is there is nothing else to do
            return( editHint );
        }

        // get the string
        text = text.remove( cursor.col-1, 1 );

        // fix up the tabs
        text = CxStringUtils::fixTabs( text, tabSpaces );

        // put the working text back into the buffer
        CxString *oldString =
            _bufferLineList.replaceAt( cursor.row, new CxString( text ));
        delete oldString;

        // move cursor back one col
        // POSSIBLE PROBLEM
        cursor.col = cursor.col - 1;

        CxEditHint editHint(
            cursor.row,
            cursor.col,
            CxEditHint::UPDATE_HINT_LINE_PAST_POINT,
            CxEditHint::CURSOR_HINT_LEFT);

        lastRequestCol = cursor.col;

        exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

        touched = TRUE;
        
        // if it is there is nothing else to do
        return( editHint );
    }

    // cursor is on line zero column zero and there is no text in the buffer
    if (position == CxEditBuffer::POS_VALID_INITIAL) {

        //return( CxEditBuffer::NONE);
        CxEditHint editHint(
            cursor.row,
            cursor.col,
            CxEditHint::UPDATE_HINT_NONE,
            CxEditHint::CURSOR_HINT_NONE);

        lastRequestCol = cursor.col;

        exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

        return( editHint );
    }

    exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

    printf("ERROR: fell through addBackspace, position = %s\n", positionString( position ).data());
    exit(0);
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::addTab
//
// peforms a tab action on the buffer at the current cursor location.  This inserts the tab
// character and also the approprient number of tab extension characters based on the current
// cursor location.
//
//-------------------------------------------------------------------------------------------------
CxEditHint
CxEditBuffer::addTab(void)
{
    if (readOnly) {
        CxEditHint editHint(
            cursor.row,
            cursor.col-1,
            CxEditHint::UPDATE_HINT_NONE,
            CxEditHint::CURSOR_HINT_NONE);
        return( editHint );
    }
    
    exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

    CxEditBuffer::POSITION position = evaluatePosition( cursor.row, cursor.col);

    // the cursor is in an insert position.  Insert a tab character and move all the
    // others to the right in the string

    if (position == CxEditBuffer::POS_VALID_INSERT) {

        // get the line element
        CxString *line = _bufferLineList.at( cursor.row );

        // get the string
        CxString existingString = line->data();

        // get the number of tab extension characters to add.
        unsigned long tabExtensionsToAdd = CxStringUtils::calcTab(cursor.col, tabSpaces);

        // insert the actual tab
        existingString.insert("\t", cursor.col++);

        // fill in with the number of extension characters needed
        for (int c=0; c<tabExtensionsToAdd-1; c++) {
            existingString.insert('\377', cursor.col++);
        }

        // set the text item
        CxString *oldString =
            _bufferLineList.replaceAt( cursor.row, new CxString( existingString ));
        delete oldString;

        //return( CxEditBuffer::NONE);
        CxEditHint editHint(
            cursor.row,
            cursor.col-1,
            CxEditHint::UPDATE_HINT_LINE_PAST_POINT,
            CxEditHint::CURSOR_HINT_RIGHT);

        lastRequestCol = cursor.col;

        exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

        touched = TRUE;
        
        return( editHint );
    }

    // the cursor is one past the last character on the line. This is the
    // append position and is valid to append a character to the line

    if (position == CxEditBuffer::POS_VALID_APPEND_COL) {

        // get the line element
        CxString *line = _bufferLineList.at( cursor.row );

        // get the string
        CxString existingString = line->data();

        // get the number of tab extension characters to add.
        unsigned long tabExtensionsToAdd = CxStringUtils::calcTab(cursor.col, tabSpaces);

        // insert the actual tab
        existingString.insert('\t', cursor.col++);

        // fill in with the number of extension characters needed
        for (int c=0; c<tabExtensionsToAdd-1; c++) {
            existingString.insert('\377', cursor.col++);
        }

        // set the text item
        CxString *oldString =
            _bufferLineList.replaceAt(cursor.row, new CxString( existingString ));

        delete oldString;

        CxEditHint editHint(
            cursor.row,
            cursor.col-1,
            CxEditHint::UPDATE_HINT_LINE_PAST_POINT,
            CxEditHint::CURSOR_HINT_RIGHT);

        lastRequestCol = cursor.col;

        exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

        touched = TRUE;
        
        return( editHint );
    }

    // the cursor is on the last line and there is nothing on that
    // line (yet) so we are in the append row position.  First thing is to
    // add a actual buffer then place the character in that buffer

    if (position == CxEditBuffer::POS_VALID_INITIAL) {

        // appending a row, so make a new string
        CxString text = "\t";

        cursor.col = cursor.col + 1;

        // get the number of tab extension characters to add.
        unsigned long tabExtensionsToAdd = CxStringUtils::calcTab(cursor.col, tabSpaces);

        // fill in with the number of extension characters needed
        for (int c=0; c<tabExtensionsToAdd; c++) {
            text.append("\377");
            cursor.col = cursor.col + 1;
        }

        // make a line structure
        CxString *line = new CxString(text);
        _bufferLineList.append( line );

        CxEditHint editHint(
            0,
            0,
            CxEditHint::UPDATE_HINT_LINE_PAST_POINT,
            CxEditHint::CURSOR_HINT_RIGHT);

        lastRequestCol = cursor.col;

        exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

        touched = TRUE;
        
        return( editHint );
    }

    exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

    if (position == CxEditBuffer::POS_INVALID_MID_TAB){
        printf("ERROR invalid mid tab position in ::addTab\n");
        exit(0);
    }

    CxEditHint editHint(
        cursor.row,
        cursor.col-1,
        CxEditHint::UPDATE_HINT_NONE,
        CxEditHint::CURSOR_HINT_NONE);

    lastRequestCol = cursor.col;

    return( editHint );
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::addReturn
//
// this performs the <return> action on the buffer.  The buffer contains no returns, rather a
// return character is represented a line in the edit buffer structure.  Several outcomes
// are possible based on the cursor position.
//
//-------------------------------------------------------------------------------------------------
CxEditHint
CxEditBuffer::addReturn(void)
{
    if (readOnly) {
        CxEditHint editHint(
            cursor.row,
            cursor.col-1,
            CxEditHint::UPDATE_HINT_NONE,
            CxEditHint::CURSOR_HINT_NONE);
        return( editHint );
    }
    
    exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

    CxEditBuffer::POSITION position = evaluatePosition( cursor.row, cursor.col);

    // we are are at a valid insert position so a CR splits the line creating
    // a new line below the current line and inserting the remaining trailing
    // character from the preveous line

    if (position == CxEditBuffer::POS_VALID_INSERT) {

        // get the line element
        CxString *line = _bufferLineList.at( cursor.row );

        // get the string
        CxString existingString = *line;

        CxString leftString  = existingString.subString(0, cursor.col);
        CxString rightString = existingString.subString(cursor.col, existingString.length() - cursor.col);

        // create a new CxString on the heap and append FIX
        rightString = CxStringUtils::fixTabs(rightString, tabSpaces);
        
        CxString *oldString =
            _bufferLineList.replaceAt( cursor.row, new CxString(leftString) );
        delete oldString;

        CxString *line2 = new CxString( rightString );

        _bufferLineList.insertAfter( cursor.row, line2 );

        CxEditHint editHint(
            cursor.row,
            cursor.col-1,
            CxEditHint::UPDATE_HINT_SCREEN_PAST_POINT,
            CxEditHint::CURSOR_HINT_NONE);

        cursor.row = cursor.row + 1;
        cursor.col = 0;

        exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

        lastRequestCol = cursor.col;

        touched = TRUE;
        
        return( editHint );
    }

    // we are in the append column which means creating a new line immediately
    // below the current line and placing the cursor there.
    if (position == CxEditBuffer::POS_VALID_APPEND_COL) {

        CxString *line = new CxString();

        _bufferLineList.insertAfter( cursor.row, line);

        cursor.row = cursor.row + 1;
        cursor.col = 0;

        //return( CxEditBuffer::NONE);
        CxEditHint editHint(
            cursor.row-1,
            cursor.col,
            CxEditHint::UPDATE_HINT_SCREEN_PAST_POINT,
            CxEditHint::CURSOR_HINT_JUMP);

        lastRequestCol = cursor.col;

        exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

        touched = TRUE;
        
        return( editHint );
    }

    // we are adding a new blank line below the last line
    if (position == CxEditBuffer::POS_VALID_INITIAL) {

        // this is a bit of a special case.  There is no allocated buffer yet so
        // we need to add a blank line at 0 and a blank line at 1, then place
        // the cursor on the second blank allocated line

        CxString *line = new CxString();
        _bufferLineList.append( line );

        line = new CxString();
        _bufferLineList.append( line );

        cursor.col = 0;
        cursor.row = 1;

        //return( CxEditBuffer::NONE);
        CxEditHint editHint(
            cursor.row,
            cursor.col,
            CxEditHint::UPDATE_HINT_SCREEN_PAST_POINT,
            CxEditHint::CURSOR_HINT_JUMP);

        lastRequestCol = cursor.col;

        exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

        touched = TRUE;
        
        return( editHint );
    }

    exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

    if (position == CxEditBuffer::POS_INVALID_MID_TAB){
        printf("ERROR invalid mid tab position in ::addReturn\n");
        exit(0);
    }

    printf("ERROR fell out of bottom of addreturn %d\n", position);
    exit(0);
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::addCharacter
//
// this is a convience method that passes a characters as cxstring.
//
//-------------------------------------------------------------------------------------------------
CxEditHint
CxEditBuffer::addCharacter( CxString c)
{
    if (readOnly) {
        CxEditHint editHint(
            cursor.row,
            cursor.col-1,
            CxEditHint::UPDATE_HINT_NONE,
            CxEditHint::CURSOR_HINT_NONE);
        return( editHint );
    }
    
    exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

    if (c.length() > 0) {
        char *ptr = c.data();
        return( addCharacter( c.data()[0] ));
    }

    exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

    printf("ERROR: A string was passed to a character funtion\n");
    exit(0);
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::addCharacter
//
// this adds a normal character to the edit buffer.  There are special cases for adding common
// 'other' characters such as CR and TAB.  Many outcomes are possible.
//
//-------------------------------------------------------------------------------------------------
CxEditHint
CxEditBuffer::addCharacter(char c)
{
    if (readOnly) {
        CxEditHint editHint(
            cursor.row,
            cursor.col-1,
            CxEditHint::UPDATE_HINT_NONE,
            CxEditHint::CURSOR_HINT_NONE);
        return( editHint );
    }
    
    exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

    CxEditBuffer::POSITION position = evaluatePosition( cursor.row, cursor.col);

    // if character is a newline
    if (c == '\n') {
        return( addReturn( ) );
    }

    // if character is a tab
    if (c == '\t') {
        return( addTab() );
    }

    // the cursor is in an insert position.  Insert a character and move all the
    // others to the right in the string

    if (position == CxEditBuffer::POS_VALID_INSERT) {

        // get the line element
        CxString *line = _bufferLineList.at( cursor.row );

        // get the string
        CxString existingString = line->data();

        // insert the character
        existingString.insert(c, cursor.col);

		// fix tabs, and insert can effect tab stops to the right
        existingString = CxStringUtils::fixTabs( existingString, tabSpaces );

        // advance the cursor o
        //cursor.col++;

        // set the text item (NEED TO DELETE RETURN)
        CxString *oldString =
            _bufferLineList.replaceAt( cursor.row, new CxString(existingString));
        delete oldString;

        // advance the cursor o
        cursor.col++;

        CxEditHint editHint(
            cursor.row,
            cursor.col-1,
            CxEditHint::UPDATE_HINT_LINE_PAST_POINT,
            CxEditHint::CURSOR_HINT_RIGHT);

        lastRequestCol = cursor.col;

        exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

        touched = TRUE;
        
        return( editHint );
    }

    // the cursor is one past the last character on the line. This is the
    // append position and is valid to append a character to the line

    if (position == CxEditBuffer::POS_VALID_APPEND_COL) {

        // get the line element
        CxString *line = _bufferLineList.at( cursor.row );

        // get the string
        CxString existingString = line->data();

        // insert the character
        existingString.append(c);


        CxString *oldString =
            _bufferLineList.replaceAt(cursor.row, new CxString(existingString));
        delete oldString;

        // advance the cursor o
        cursor.col++;

        CxEditHint editHint(
            cursor.row,
            cursor.col-1,
            CxEditHint::UPDATE_HINT_LINE_PAST_POINT,
            CxEditHint::CURSOR_HINT_RIGHT);

        lastRequestCol = cursor.col;

        exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

        touched = TRUE;
        
        return( editHint );
    }

    // the cursor is on the last line and there is nothing on that
    // line (yet) so we are in the append row position.  First thing is to
    // add a actual buffer then place the character in that buffer

    if (position == CxEditBuffer::POS_VALID_INITIAL) {

        // appending a row, so make a new string
        CxString text;

        // append character
        text.append(c);

        // make a line structure
        CxString *line = new CxString(text);

        // put it back in the edit buffer
        _bufferLineList.append( line );

        // advance the cursor to the next col
        cursor.col++;

        //return( CxEditBuffer::NONE);
        CxEditHint editHint(
            cursor.row,
            cursor.col-1,
            CxEditHint::UPDATE_HINT_LINE_PAST_POINT,
            CxEditHint::CURSOR_HINT_RIGHT);

        lastRequestCol = cursor.col;

        exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

        touched = TRUE;
        
        return( editHint );
    }

    exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

    if (position == CxEditBuffer::POS_INVALID_MID_TAB){
        printf("ERROR invalid mid tab position in ::addCharacter\n");
        exit(0);
    }

    CxEditHint editHint(
        cursor.row,
        cursor.col-1,
        CxEditHint::UPDATE_HINT_NONE,
        CxEditHint::CURSOR_HINT_NONE);

    lastRequestCol = cursor.col;


    return( editHint );
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::line
//
// returns the line object from the edit buffer based on row
//
//-------------------------------------------------------------------------------------------------
CxString *
CxEditBuffer::line(unsigned long row)
{
    if (row < _bufferLineList.entries()) {
        return( _bufferLineList.at(row) );
    }

    return( NULL );
}


//-------------------------------------------------------------------------------------------------
//
// CxEditBuffer::pasteFromCutBuffer
//
// returns the character at the position in the buffer
//
//-------------------------------------------------------------------------------------------------
void
CxEditBuffer::pasteFromCutBuffer( CxString text )
{
    if (readOnly) return;
    
    exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);
    insertTextAtCursor( text );
}


//-------------------------------------------------------------------------------------------------
//
// CxEditBuffer::insertTextAtCursor
//
// returns the character at the position in the buffer
//
//-------------------------------------------------------------------------------------------------
void
CxEditBuffer::insertTextAtCursor( CxString text )
{
    if (readOnly) return;
    
    exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

    CxString workingString = CxStringUtils::fromTabExtensionFormat( text );

    for (unsigned long i=0; i<workingString.length(); i++) {
        char c = workingString.charAt(i);
        addCharacter( c );
    }
}


//-------------------------------------------------------------------------------------------------
//
// CxEditBuffer::characterAt
//
// returns the character at the position in the buffer
//
//-------------------------------------------------------------------------------------------------
char
CxEditBuffer::characterAt( unsigned long row, unsigned long col)
{
    CxEditBuffer::POSITION position = evaluatePosition( row, col);

    if (position == CxEditBuffer::POS_VALID_APPEND_COL) {
        return('\n');
    }

    if (position == CxEditBuffer::POS_VALID_INSERT) {
        CxString *line = _bufferLineList.at( row );
        return( line->data()[col] );
    }

    return( (char) 0);
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::cutToMark
//
// sets the mark at the end of the current cursor line
//
//-------------------------------------------------------------------------------------------------
CxString
CxEditBuffer::cutToMark( void )
{
    if (readOnly) return( "" );
    
    exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

    CxString theCutText = copyText( );
    deleteText( );
    return( theCutText );
}


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::setMarkAtEndOfLine
//
// sets the mark at the end of the current cursor line
//
//-------------------------------------------------------------------------------------------------
void
CxEditBuffer::setMarkAtEndOfLine( void )
{
    exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

    CxEditBufferPosition saveCursor = cursor;

    CxEditBuffer::POSITION position = CxEditBuffer::POS_VALID_INITIAL;

    while( 1 ) {

        CxEditBuffer::POSITION position = evaluatePosition( cursor.row, cursor.col);

        exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

        // handle the nothing in buffer case
        if (position == CxEditBuffer::POS_VALID_INITIAL) {
            return;
        }

        // hand the last postion in line case
        if (position == POS_VALID_APPEND_COL) {
            mark   = cursor;
            markSet = true;
            cursor = saveCursor;

            exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

            return;
        }

        // cursor right
        CxEditHint hint = cursorRightRequest();
        if (hint.cursorHint() == CxEditHint::CURSOR_HINT_NONE) {
            mark = cursor;
            markSet = true;
            cursor = saveCursor;

            exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

            return;
        }
    }
}


//-------------------------------------------------------------------------------------------------
//
// CxEditBuffer::cutTextToEndOfLine
//
// copys text between two points in the buffer
//
//-------------------------------------------------------------------------------------------------

CxString
CxEditBuffer::cutTextToEndOfLine(void)
{
    if (readOnly) return( "" );

    exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

    // set the mark at the append position on the current line
    setMarkAtEndOfLine();

    CxString theCutText = copyText( );
    deleteText( );

    exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

    return( theCutText );
}


//-------------------------------------------------------------------------------------------------
//
// CxEditBuffer::copyText
//
// copys text between two points in the buffer
//
//-------------------------------------------------------------------------------------------------
CxString
CxEditBuffer::copyText(void)
{
    exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

    // set the cut text to nothing
    //cutText = "";
    
    CxString cutText;

    // if there is no mark set this is an invalid thing to do, return

	CxString s;
    if (markSet == false) return( s );

    // save the current cursor position
    CxEditBufferPosition saveCursor = cursor;
    CxEditBufferPosition lastCharacterPosition;

    // see if we need to swap the cursor and mark
    switch (compareCursorToMark()) {

        // cursor if before mark
        case -1:
        {
            cursor = mark;

            lastCharacterPosition = getPositionPriorToCursor();

            cursor = saveCursor;

            break;
        }

        // they are same position, nothing to do here
        case 0:
        {
			CxString s;
            return( s );
        }

        // cursor is after mark
        case 1:
        {
            lastCharacterPosition = getPositionPriorToCursor();

            cursor = mark;

        	exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

            break;
        }

        // should not happen
        default:
        {
			CxString s;
            return( s );
        }
    }

    CxEditBuffer::POSITION position = CxEditBuffer::POS_VALID_INITIAL;

    while( 1 ) {

        CxEditBuffer::POSITION position = evaluatePosition( cursor.row, cursor.col);

        // handle the nothing in buffer case
        if (position == CxEditBuffer::POS_VALID_INITIAL) {
            return(CxString(""));
        }

        // hand the last postion in line case
        if (position == POS_VALID_APPEND_COL) {
            cutText += CxString("\n");
        }

        // hand the tab extension case, should not happen as we are using the
        // cursor right function
        if (position == POS_INVALID_MID_TAB) {
        	exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);
        }

        // handle the character case
        if (position == POS_VALID_INSERT) {
            cutText += characterAt( cursor.row, cursor.col);
        }

        // if we have completed the request

        if (cursor >= lastCharacterPosition) {
            cursor = saveCursor;

        	exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

            return( cutText );
        }

        // cursor right
        CxEditHint hint = cursorRightRequest();
        if (hint.cursorHint() == CxEditHint::CURSOR_HINT_NONE) {
            cursor = saveCursor;

	        exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

            return( cutText );
        }
    }
}


//-------------------------------------------------------------------------------------------------
//
// CxEditBuffer::deleteText:
//
// removes text from the edit buffer between the two points in the buffer
//
//-------------------------------------------------------------------------------------------------
void
CxEditBuffer::deleteText(void)
{
    if (readOnly) return;
    
    exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

    CxEditBufferPosition saveCursor = cursor;

    //---------------------------------------------------------------------------------------------
    // Handle some special cases first,
    // if the cursor is in the first col
    //---------------------------------------------------------------------------------------------
    if (isCursorFirstCol()) {

        //-----------------------------------------------------------------------------------------
        // if the cursor row is empty
        //-----------------------------------------------------------------------------------------
        if (isCursorRowEmpty()) {

            //-------------------------------------------------------------------------------------
            // are the cursor and mark the same
            //-------------------------------------------------------------------------------------
            if (cursor == mark) {

                //---------------------------------------------------------------------------------
                // and the cursor is on the last row
                //---------------------------------------------------------------------------------
                if (isCursorLastRow()) {

                    //-----------------------------------------------------------------------------
                    // just hit backspace and leave the cursor at the end of the preveous row
                    //-----------------------------------------------------------------------------
                    addBackspace();

        			exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

                    return;

                    //-----------------------------------------------------------------------------
                    // the cursor is not on the last row
                    //-----------------------------------------------------------------------------
                
				} else {

                    //-----------------------------------------------------------------------------
                    // hit backspace, and then cursor right one
                    //-----------------------------------------------------------------------------
                    addBackspace();
                    cursorRightRequest();

        			exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

                    return;
                }
            }
        }
    }

    //---------------------------------------------------------------------------------------------
    // just deleting a chunck without any special handling
    //
    //---------------------------------------------------------------------------------------------
    switch (compareCursorToMark()) {

        // cursor if before mark, swap them so cursor is end of region
        case -1:
        {
            cursor = mark;
            mark   = saveCursor;

	        exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

            break;
        }

        // they are same position, nothing to do here
        case 0:
        {
            return;
        }

        // cursor is after mark, so the cursor is in the correct location to
        // start backspacing through the buffer
        case 1:
        {
            break;
        }

        // should not happen
        default:
        {
            return;
        }
    }

    CxEditBuffer::POSITION position = CxEditBuffer::POS_VALID_INITIAL;

    while( 1 ) {

        CxEditBuffer::POSITION position = evaluatePosition( cursor.row, cursor.col);

        // handle the nothing in buffer case
        if (position == CxEditBuffer::POS_VALID_INITIAL) {
            return;
        }

        addBackspace();

        exitDumpIfCursorIsInvalid( __FUNCTION__ , __LINE__);

        // if we have completed the request
        if (cursor == mark) {
            //cursor = saveCursor;
            return;
        }
    }
}


//-------------------------------------------------------------------------------------------------
//
// CxEditBuffer::insertCommentBlock
//
// inserts a comment block and positions the cursor for inserting the commet text.
//
//-------------------------------------------------------------------------------------------------
void
CxEditBuffer::insertCommentBlock( unsigned long lastCol )
{
    if (readOnly) return;
    
    // get the current line text
    CxString text = _bufferLineList.at( cursor.row )->data();

    // cut the preceeding text before the cursor
    text = text.subString(0, cursor.col);

    // remove tab extensions
    //text = CxStringUtils::fromTabExtensionFormat( text );

    // calculate how many remaining positions there are
    unsigned long remainingCharacterNumber = lastCol - text.length() - 2;

    // insert the first line
    insertTextAtCursor( "//");
    for (unsigned long i=0; i<remainingCharacterNumber; i++) {
        addCharacter('-');
    }
    addCharacter('\n');

    insertTextAtCursor(text);
    insertTextAtCursor("// \n");

    insertTextAtCursor(text);
    insertTextAtCursor("// \n");

    insertTextAtCursor(text);
    insertTextAtCursor("// \n");

    insertTextAtCursor(text);
    insertTextAtCursor("// \n");

    insertTextAtCursor(text);
    insertTextAtCursor( "//");
    for (unsigned long i=0; i<remainingCharacterNumber; i++) {
        addCharacter('-');
    }
    insertTextAtCursor("\n");

    cursorUpRequest();
    cursorUpRequest();
    cursorUpRequest();
    cursorUpRequest();
    cursorUpRequest();
    cursorRightRequest();
    cursorRightRequest();
    cursorRightRequest();
}

void
CxEditBuffer::setVisualFirstScreenLine(int line)
{
    visualFirstScreenLine = line;
}

int
CxEditBuffer::getVisualFirstScreenLine( void )
{
    return( visualFirstScreenLine );
}

void
CxEditBuffer::setVisualFirstScreenCol( int col )
{
    visualFirstScreenCol = col;
}

int
CxEditBuffer::getVisualFirstScreenCol( void )
{
    return( visualFirstScreenCol );
}


CxString
CxEditBuffer::getFilePath( void )
{
    return( filePath );
}

void
CxEditBuffer::setFilePath( CxString fp )
{
    filePath = fp;
}
