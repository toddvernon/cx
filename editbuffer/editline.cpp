//-------------------------------------------------------------------------------------------------
//
// CxEditLine
//
// CxEditLine Class
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
//-------------------------------------------------------------------------------------------------

#include <stdio.h>

#include <cx/base/string.h>
#include <cx/base/slist.h>
#include <cx/editbuffer/editline.h>

//-------------------------------------------------------------------------
// CxEditLine:: (constructor)
//
//-------------------------------------------------------------------------
CxEditLine::CxEditLine(unsigned long editWidth_ )
:_text(""),_cursorCol(0),_cursorRow(0),_editWidth(editWidth_)
{
}


//-------------------------------------------------------------------------------------------------
// CxEditLine::text
//
//-------------------------------------------------------------------------------------------------
CxString
CxEditLine::text(void)
{
    return(_text );
}


//-------------------------------------------------------------------------------------------------
// CxEditLine::appendText
//
//-------------------------------------------------------------------------------------------------
void
CxEditLine::appendText(CxString t)
{
    for (unsigned long i=0; i<t.length(); i++) {
        addCharacter(t.charAt(i));
    }
}

//-------------------------------------------------------------------------------------------------
// CxEditLine::setText
//
//-------------------------------------------------------------------------------------------------
void
CxEditLine::setText(CxString)
{
    _text = "";
    _cursorCol = 0;
}


//-------------------------------------------------------------------------------------------------
// cursorCol
//
// returns the current cursor col in the buffer
//
//-------------------------------------------------------------------------------------------------
unsigned long
CxEditLine::cursorCol(void)
{
    return( _cursorCol );
}


//-------------------------------------------------------------------------------------------------
// positionString
//
// return the position in the buffer as a string.  Used for debug
//
//-------------------------------------------------------------------------------------------------
CxString
CxEditLine::positionString(CxEditLine::POSITION position)
{
    switch( position )
    {

        case POS_VALID_INSERT:
        {
            return("POS_VALID_INSERT");
        }
        case POS_VALID_APPEND_COL:
        {
            return("POS_VALID_APPEND_COL");
        }
        
        case POS_INVALID_COL:
        {
            return("POS_INVALID_COL");
        }
        
    }
    
    return("INVALID_RETURN_CODE");
}


//-------------------------------------------------------------------------------------------------
//
// CxEditLine::evaluatePosition
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

CxEditLine::POSITION
CxEditLine::evaluatePosition(unsigned long col_ )
{
    unsigned long currentLineLength = _text.length();
        
    // if the col is beyond length of the string and the append positions it invalid
    if (col_ > currentLineLength+1) {
        return( CxEditLine::POS_INVALID_COL);
    }
    
    // if column is the one beyond the text its the append column
    if (col_ == currentLineLength) {
        return( CxEditLine::POS_VALID_APPEND_COL);
    }
    
    // all other placements are a valid character insert location
    return( CxEditLine::POS_VALID_INSERT);
}


//-------------------------------------------------------------------------------------------------
//
// CxEditLine::cursorRightRequest
//
// Four things can potentially happen with a cursor right.
// 1) the cursor moves right on the same line becuase there is a character to the right
// 2) the cursor moves right into the append position on the current line
//
//-------------------------------------------------------------------------------------------------
CxEditHint
CxEditLine::cursorRightRequest(void)
{
    unsigned long currentLineLength = _text.length();
    
    // calculate a new potential cursor col
    unsigned long newCursorCol = _cursorCol + 1;
    
    // check to see if the cursor will stay on the same line
    // this happens if there is an editable position to the right
    // <or> the position to the right is the first position after the
    // buffer (ie the append position)
    if (newCursorCol < currentLineLength + 1) {
        
        // it does, so just advance it one to the right
        _cursorCol = newCursorCol;

        // report back that a cursor right was completed
        CxEditHint editHint(
            _cursorRow,
            _cursorCol,
            CxEditHint::UPDATE_HINT_NONE,
            CxEditHint::CURSOR_HINT_RIGHT);
        
        return( editHint );
    }
        
    // can't go right
    CxEditHint editHint(
        _cursorRow,
        _cursorCol,
        CxEditHint::UPDATE_HINT_NONE,
        CxEditHint::CURSOR_HINT_NONE);
    
    return( editHint );
        
}
    

//-------------------------------------------------------------------------------------------------
//
// CxEditLine::cursorLeftRequest
//
// Three outcomes can potentially happen with a cursor left.
// 1) the cursor moves left on the same line becuase there is a character to the left
// 2) the cursor can't move left as its on the first line, first column
//
//-------------------------------------------------------------------------------------------------
CxEditHint
CxEditLine::cursorLeftRequest(void)
{
    unsigned long length = _text.length();
    
    // check if the cursor is in the first column
    if (_cursorCol == 0) {
                    
        // more ignored
        CxEditHint editHint(
            _cursorRow,
            _cursorCol,
            CxEditHint::UPDATE_HINT_NONE,
            CxEditHint::CURSOR_HINT_NONE);

            // if it is there is nothing else to do
        return( editHint );
    }
    
    // cursor is not in column zero so it can move left
    _cursorCol = _cursorCol - 1;
        
    // more ignored
    CxEditHint editHint(
        _cursorRow,
        _cursorCol,
        CxEditHint::UPDATE_HINT_NONE,
        CxEditHint::CURSOR_HINT_LEFT);
    
    // if it is there is nothing else to do
    return( editHint );
}


//-------------------------------------------------------------------------------------------------
//
// cursorUpRequest
//
//
//-------------------------------------------------------------------------------------------------
CxEditHint
CxEditLine::cursorUpRequest(void)
{
    // more ignored
    CxEditHint editHint(
        _cursorRow,
        _cursorCol,
        CxEditHint::UPDATE_HINT_NONE,
        CxEditHint::CURSOR_HINT_NONE);
    
    // if it is there is nothing else to do
    return( editHint );
    
}


//-------------------------------------------------------------------------------------------------
//
// cursorDownRequest
//
// Three outcomes can potentially happen with a cursor left.
//
//
//-------------------------------------------------------------------------------------------------
CxEditHint
CxEditLine::cursorDownRequest(void)
{
    CxEditHint editHint(
        _cursorRow,
        _cursorCol,
        CxEditHint::UPDATE_HINT_NONE,
        CxEditHint::CURSOR_HINT_NONE);
    
    // if it is there is nothing else to do
    return( editHint );
}


//-------------------------------------------------------------------------------------------------
// addBackspace
//
// performs a backspace action on the buffer at the current cursor location.  Many outcomes
// are possible.
//
//-------------------------------------------------------------------------------------------------
CxEditHint
CxEditLine::addBackspace(void)
{
    CxEditLine::POSITION position = evaluatePosition( _cursorCol);
    
    if ((position != CxEditLine::POS_VALID_INSERT) &&
        (position != CxEditLine::POS_VALID_APPEND_COL)) {
        
        //return( CxEditLine::NONE);
        CxEditHint editHint(
            _cursorRow,
            _cursorCol,
            CxEditHint::UPDATE_HINT_NONE,
            CxEditHint::CURSOR_HINT_NONE);
    
    }

    if (_cursorCol == 0) {
            
        //return( CxEditLine::NONE);
        CxEditHint editHint(
            _cursorRow,
            _cursorCol,
            CxEditHint::UPDATE_HINT_NONE,
            CxEditHint::CURSOR_HINT_NONE);
        
        return ( editHint  );
    }
        
    // ok to remove the characters under the cursor and move cursor left one
    // remove the text
    _text.remove( _cursorCol-1, 1 );
            
    // move cursor back one col
    _cursorCol = _cursorCol - 1;
               
    CxEditHint editHint(
        _cursorRow,
        _cursorCol,
        CxEditHint::UPDATE_HINT_LINE_PAST_POINT,
        CxEditHint::CURSOR_HINT_NONE);
    

    // if it is there is nothing else to do
    return( editHint );
}


//-------------------------------------------------------------------------------------------------
// addTab
//
// peforms a tab action on the buffer at the current cursor location.  This inserts the tab
// character and also the approprient number of tab extension characters based on the current
// cursor location.
//
//-------------------------------------------------------------------------------------------------
CxEditHint
CxEditLine::addTab(void)
{
    CxEditHint editHint(
        _cursorRow,
        _cursorCol,
        CxEditHint::UPDATE_HINT_NONE,
        CxEditHint::CURSOR_HINT_NONE);
    
    return( editHint );
}


//-------------------------------------------------------------------------------------------------
// addReturn
//
// this performs the <return> action on the buffer.  The buffer contains no returns, rather a
// return character is represented a line in the edit buffer structure.  Several outcomes
// are possible based on the cursor position.
//
//-------------------------------------------------------------------------------------------------
CxEditHint
CxEditLine::addReturn(void)
{
    CxEditHint editHint(
        _cursorRow,
        _cursorCol,
        CxEditHint::UPDATE_HINT_NONE,
        CxEditHint::CURSOR_HINT_NONE);
    
    return( editHint );
}


//-------------------------------------------------------------------------------------------------
// addCharacter
//
// this is a convience method that passes a characters as cxstring.
//
//-------------------------------------------------------------------------------------------------
CxEditHint
CxEditLine::addCharacter( CxString c)
{
    if (c.length() > 0) {
        char *ptr = c.data();
        return( addCharacter( c.data()[0] ));
    }
    
    printf("ERROR: A string was passed to a character funtion\n");
    exit(0);
}


//-------------------------------------------------------------------------------------------------
// addCharacter
//
// this adds a normal character to the edit buffer.  There are special cases for adding common
// 'other' characters such as CR and TAB.  Many outcomes are possible.
//
//-------------------------------------------------------------------------------------------------
CxEditHint
CxEditLine::addCharacter(char c)
{
    CxEditLine::POSITION position = evaluatePosition( _cursorCol);
        
    // the cursor is in an insert position.  Insert a character and move all the
    // others to the right in the string
    
    if (position == CxEditLine::POS_VALID_INSERT) {
                    
        // get the string
        CxString existingString = _text;
        
        // insert the character
        existingString.insert(c, _cursorCol);
        
        // advance the cursor o
        _cursorCol++;
        
        // replace the text
        _text = existingString;
        
        //return( CxEditLine::NONE);
        CxEditHint editHint(
            _cursorRow,
            _cursorCol-1,
            CxEditHint::UPDATE_HINT_LINE_PAST_POINT,
            CxEditHint::CURSOR_HINT_RIGHT);
        
        return( editHint );
    }
    
    // the cursor is one past the last character on the line. This is the
    // append position and is valid to append a character to the line
    
    if (position == CxEditLine::POS_VALID_APPEND_COL) {
                
        // insert the character
        _text.append(c);
                
        // advance the cursor o
        _cursorCol++;
                
        //return( CxEditLine::NONE);
        CxEditHint editHint(
            _cursorRow,
            _cursorCol-1,
            CxEditHint::UPDATE_HINT_LINE_PAST_POINT,
            CxEditHint::CURSOR_HINT_RIGHT);
                
        return( editHint );
    }

    CxEditHint editHint(
        _cursorRow,
        _cursorCol-1,
        CxEditHint::UPDATE_HINT_NONE,
        CxEditHint::CURSOR_HINT_NONE);
    
    return( editHint );
}


