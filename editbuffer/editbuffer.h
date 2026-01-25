//-------------------------------------------------------------------------------------------------
//
//  CxEditBuffer.h
//
//  CxEditBuffer Class
//
//-------------------------------------------------------------------------------------------------
//
//  (c) Copyright 2022 T.Vernon
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

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>

#include <cx/base/string.h>
#include <cx/base/slist.h>
#include <cx/base/file.h>

#include <cx/editbuffer/stringlist.h>
#include <cx/editbuffer/edithint.h>
#include <cx/editbuffer/editbufferpos.h>
#include <cx/editbuffer/stringutils.h>


#ifndef _CxEditBuffer_
#define _CxEditBuffer_




//-------------------------------------------------------------------------------------------------
// CxEditBuffer::
//
//-------------------------------------------------------------------------------------------------

class CxEditBuffer
{
  public:
    
    enum POSITION {
        
        POS_VALID_INITIAL,      // this is the inital state of an edit buffer.  No
                                // backing lines have been allocated and cursor is at 0,0
        
        POS_VALID_INSERT,       // a valid position in the buffer to insert a character
        
        POS_VALID_APPEND_COL,   // a valid position in the buffer to append a character
        
        POS_INVALID_ROW,        // beyond the end of the buffer
        
        POS_INVALID_COL,        // beyond the text in any given line
        
        POS_INVALID_MID_TAB     // inside an expanded tab (requires cursor move to be valid)
    
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
        
        CURSOR_WRAP_DOWN,   // move the cursor down a line to the first column
                            // caused pressing the right arrow key on the last column of
                            // the current line (the append pos) when there is another editable
                            // below it
        
        CURSOR_WRAP_UP,     // move the cursor up to last column of the previous line
                            // caused by typing left arrow on col position 0 on a row
                            // other than the last row
        
        EXPAND_DOWN,        // create a new line at the end of the buffer
                            // this is caused by typing return on last line of buffer
        
        SPLIT_LINE,         // split the line and insert remainder into a new line below
                            // thsi is caused by typeing return character in mid point of a line
        
        JOIN_LINE           // drag the current line up, concat to previous line, deleting line
                            // this is caused by typing the delete key in the first character position
    };

    CxEditBuffer(void);
    // constructor
    
    CxEditBuffer(int tabspaces);
    // constructor setting the tabspaces
    
    void reset(void);
    // start over with an initialized buffer
    
    CxEditBuffer( const CxEditBuffer& c);
    // copy constructor
    
    CxEditBuffer operator=( const CxEditBuffer& c);
    // assignment operator
    
    void exitDumpIfCursorIsInvalid( const char* functionName, int lineNumber );
    // debug method to do an exit and dump if cursor is in an invalid location
    
    CxString
    positionString(CxEditBuffer::POSITION position);
    // return name of cursor position
    
    void
    pasteFromCutBuffer( CxString text );
    // paste text from cut buffer into cursor position
    
	CxSList<CxString>
    reduceText( CxString text_ );
    // reformat text for insertion into buffer
    
    CxEditHint
    cursorRightRequest(void);
    // request to move the cursor right in the buffer, retuns a hint as what occured with
    // the cursor and what parts of the real screen needs updating.
    
    CxEditHint
    cursorLeftRequest(void);
    // request to move the cursor left in the buffer, retuns a hint as what occured with
    // the cursor and what parts of the real screen needs updating.

    CxEditHint
    cursorUpRequest(void);
    // request to move the cursor up in the buffer, retuns a hint as what occured with
    // the cursor and what parts of the real screen needs updating.

    CxEditHint
    cursorDownRequest(void);
    // request to move the cursor down in the buffer, retuns a hint as what occured with
    // the cursor and what parts of the real screen needs updating.
    
    CxEditBuffer::POSITION
    evaluatePosition(unsigned long row_, unsigned long col_ );
    // evaluate a position in the buffer and return the classification
    
    CxEditBuffer::POSITION
    cursorGotoRequest(unsigned long row_, unsigned long col_ );
    // set the cursor at a new position in the buffer, returns if the request was
    // successful
    
    CxEditBuffer::POSITION
    cursorGotoLine( unsigned long row_);
    // a friendly version of cursorGotoRequest that won't put the cursor beyond the buffer,
    // its used for a user goto-line request.
    
    int
    loadText(CxString filepath_, int preload );
    // load edit buffer to a file, if preload if false then a temp empty buffer is assigned
    // and the inMemory flag will be false 
    
    void
    loadTextFromString( CxString text );
    // load text from a flat buffer containing CR information
        
    void
    saveText(CxString filepath_);
    // load edit buffer from a file
    
    unsigned long
    numberOfLines();
    // returns the count of lines in the current edit buffer

    CxEditHint
    addCharacter( CxString c);
    // adds the first character of the string to the buffer at the cursor position.
    
    CxEditHint
    addCharacter(char c);
    // adds a character at the cursor position. This should be only called on
    // printable characters and CR
    
    CxEditHint
    addTab(void);
    // inserts and tab into the buffer plus tab extension characters to handle buffer
    // editing easier.
    
    CxEditHint
    addReturn( void );
    // adds a return to the buffer to cursor position.  This is called but addCharacter
    // if the character being added is a CR.
    
    CxEditHint
    addBackspace( void );
    // performs a backspace function on the buffer at the current cursor position. This will
    // delete the character to the left of the cursor dragging all the characters to the right
    // to the left.  If the cursor is in the first column of a row that is not the zero row
    // it will join the characters from the row to the preveous row eliminating the current
    // row from the buffer.

    CxString *
    line(unsigned long row);
    // returns a line of text based on row.  Null is returned if the line is invalid
    
    char
    characterAt( unsigned long row, unsigned long col);
    // returns the character at the position in the buffer or 0 if an invalid position
        
    void
    deleteText(void);
    // deletes text between cursor and mark
    
    CxString
    copyText(void);
    // returns a copy of text between cursor and mark
    
    void
    setMark( void );
    // set the mark variables to the current cursor position;
    
    CxString
    cutToMark( void );
    // cut text from the cursor to the mark
    
    CxString
    cutTextToEndOfLine(void);
    // cut text from current cursor position to the end of the line
    
    void
    insertTextAtCursor( CxString text );
    // insert text at the current cursor position
    
    void
    insertCommentBlock( unsigned long lastCol );
    // insert a comment black at the current cursor position
    
    CxString
    flattenBuffer(void);
    // flatten the buffer
            	
	int 
	findString( CxString findString );
	// find a string starting at a the cursor position.  returns TRUE if
	// the string was found beyond the cursor position and the cursor is
    // moved to the first character of the found string.
    
    int
    findAgain( CxString findString, int skipIfCurrent );
    // find again is the same as findString but the last findstring
    // is used.  if cursor is at the location that last found the string and
    // skip if current is true, then the cursor will be advanced one space
    // before doing to the search
    
    int
    replaceString( CxString findString, CxString string );
    // performs a find, using the value of findstring from the last findString
    // command.  If the string is found, then it is replaced with the replace
    // string and the cursor is left at the first postion of the replace string
    
    int
    replaceAgain( CxString findString, CxString replaceString );
    // similar to replaceString but the same replace string is used from
    // the last replace.  A successive call to replaceAgain should move
    // the cursor one character forward before calling.
    
    CxEditBufferPosition cursor;
    // the current cursor position in the buffer
     
    CxEditBufferPosition mark;
    // the current mark position in the buffer
    
    // Kind of a hack
    void setVisualFirstScreenLine(int line);
    int getVisualFirstScreenLine(void);
    
    void setVisualFirstScreenCol(int col);
    int getVisualFirstScreenCol( void );
    // used to persist the visual screen line.  This variable is not used by edit buffer
    // its just maintained here for buffer switches
    
    int isReadOnly(void);
    // returns if the buffer is editable or not
    
    int isTouched(void);
    // returns true if the buffer was modified
    
    int isInMemory(void);
    // returns true of the buffer is currently in memory
    
    void setReadOnly( int v);
    // set the buffer readonly or not
    
    //-------------- DEBUG METHODS ----------------------------------------
    
    void debug_print(void);
    // DEBUG: used in debugging an unit tests
    
    void debug_print_string( unsigned long lineNumber);
    // DEBUG: used to print out non ascii representation of the string
    
    void printCharacterAtCursor(void);
    // DEBUG: print a debug of the character at the cursor
    
    CxString characterClassAt( unsigned long row, unsigned long col);
    // DEBUG: get the class o
        
    int compareCursorToMark(void);
    // compare the cursor and mark
    
    //----------------------------------------------------------------------
    
    CxString name;
    // holds the name of the buffer
    
    CxString getFilePath(void);
    // returns the filepath of the buffer
    
    void setFilePath( CxString fp );
    // sets the filepath of the buffer
    
    
  private:
    
    CxEditBufferPosition getPositionPriorToCursor(void);

    void setMarkAtEndOfLine(void);
    
    unsigned long rawLengthOfBuffer(void);
    // get the raw length of the buffer.  This is the length including tab extension
    // characters so it will be longer than the true length of text.
    
    CxEditHint joinLines( void );
    // this private function joins two lines.  It is called by addBackspace in certian conditions
    // to join to successive lines of text together.
    
    int isCursorFirstRow(void);
    int isCursorLastRow(void);
    // helper methods to determin if the cursor is on the first of last row
    
    int isCursorFirstCol(void);
    int isCursorLastCol(void);
    // helper methods to determin if the cursor is in the first or last column of current row
    
    int isCursorFirstValidPosition(void);
    int isCursorLastValidPosition(void);
    // helper methods to determine if the cursor is the first position or last position of
    // the entire buffer
    
    int isCursorRowEmpty(void);
    // returns true if the cursor is on an emptry row
    
    CxStringList _bufferLineList;
    // holds the list of lines
 
    int markSet;
    // true if a mark has been set
 
    unsigned long lastRequestCol;
    // the last column inserted, backspaced, or navigated to intentionally by the user
    
    int findAgainMultiLine( CxString findString, int skipIfCurrent );
    // multi-line find across line boundaries

    int replaceAgainMultiLine( CxString findString, CxString replaceString );
    // multi-line replace across line boundaries

    CxEditBufferPosition lastFindLocation;
    // holds the last find location
    
    int tabSpaces;
    // how many screen locations makes up a tab
    
    int visualFirstScreenLine;
    // used to persist where the buffer is vertically on the screen
    
    int visualFirstScreenCol;
    // used to persist where the buffer is horizontally on the screen
    
    CxString filePath;
    // the filepath of the file
    
    int readOnly;
    // flag that buffer is read only
    
    int touched;
    // has file been changed
    
    int inMemory;
    // is the file in memory

};


#endif

