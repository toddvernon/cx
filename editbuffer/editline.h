//-------------------------------------------------------------------------------------------------
//
//  editline.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxEditBuffer Class
//
//-------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include <sys/types.h>

#include <cx/base/string.h>
#include <cx/base/slist.h>
#include <cx/base/file.h>

//#include <cx/editbuffer/bufferline.h>
#include <cx/editbuffer/stringlist.h>
#include <cx/editbuffer/edithint.h>
#include <cx/editbuffer/stringutils.h>


#ifndef _CxEditLine_
#define _CxEditLine_


//-------------------------------------------------------------------------------------------------
// CxEditBuffer::
//
//-------------------------------------------------------------------------------------------------
class CxEditLine
{
  public:
    
    enum POSITION {
        
        POS_VALID_INSERT,       // a valid position in the buffer to insert a character
        
        POS_VALID_APPEND_COL,   // a valid position in the buffer to append a character
        
        POS_INVALID_COL,        // beyond the text in any given line
        
    };
    
    
    enum  ACTION {
        
        NONE,               // No screen change
                            // caused by an action that can't be performed or does
                            // not result in a screen change
        
        CURSOR_RIGHT,       // move the cursor to the right on current line
                            // caused when the right arrow is typed and there is a
                            // a character to the right, or the append point is to
                            // to the right (ie appending characters to current line)
        
        CURSOR_LEFT,        // move the cursor left on the current line
                            // caused when the left arrow is typed and there is a
                            // character the left on the current line
        
        CURSOR_UP,          // move the cursor up to the previous line
                            // caused by typing the up arrow when there is an
                            // editable line above the current one
        
        CURSOR_DOWN,        // move the cursor down to the next line
                            // caused by typeing the down arrow when there is another
                            // editiable line below the current line
    };

    CxEditLine( unsigned long editWidth_ );
    // constructor

    CxString text(void);
    // name of the callback
    
    void setText(CxString t);
    
    void appendText(CxString t);

    
    
    CxString positionString(CxEditLine::POSITION position);
    // return name of cursor position
    
    unsigned long cursorRow(void);
    // get the current cursor row
    
    unsigned long cursorCol(void);
    // get the current cursor col
    
    CxEditHint cursorRightRequest(void);
    // request to move the cursor right in the buffer, retuns a hint as what occured with
    // the cursor and what parts of the real screen needs updating.
    
    CxEditHint cursorLeftRequest(void);
    // request to move the cursor left in the buffer, retuns a hint as what occured with
    // the cursor and what parts of the real screen needs updating.

    CxEditHint cursorUpRequest(void);
    // request to move the cursor up in the buffer, retuns a hint as what occured with
    // the cursor and what parts of the real screen needs updating.

    CxEditHint cursorDownRequest(void);
    // request to move the cursor down in the buffer, retuns a hint as what occured with
    // the cursor and what parts of the real screen needs updating.
    
    CxEditLine::POSITION evaluatePosition(unsigned long col_ );
    // evaluate a position in the buffer and return the classification

    CxEditHint addCharacter( CxString c);
    // adds a string to the buffer at the cursor position.
    
    CxEditHint addCharacter(char c);
    // adds a character at the cursor position. This should be only called on
    // printable characters
    
    CxEditHint addTab(void);
     
    
    CxEditHint addReturn( void );
    
    
    CxEditHint addBackspace( void );
    

  private:
    
    CxString   _text;
    // holds the name of the buffer
    
    unsigned long _cursorRow;
    // current cursor row
    
    unsigned long _cursorCol;
    // current cursor col

    unsigned _editWidth;
    
};


#endif
