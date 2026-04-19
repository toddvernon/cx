//-------------------------------------------------------------------------------------------------
//
//  screen.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxScreen class
//
//-------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <termios.h>    
#include <unistd.h>     

#include <iostream>
#include <fcntl.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#if defined(_LINUX_) || defined(_OSX_) || defined(_SOLARIS_) || defined(_IRIX6_) || defined(_NETBSD_)
#include <sys/ioctl.h>
#endif

//#if defined(_SUNOS_)
//int ioctl(...);
//#endif

#include <cx/base/string.h>
#include <cx/base/exception.h>
#include <cx/base/slist.h>
#include <cx/functor/defercall.h>

#include <cx/screen/color.h>


#ifndef _CxSCREEN_
#define _CxSCREEN_


//-------------------------------------------------------------------------------------------------
// CxScreen::
//
//-------------------------------------------------------------------------------------------------
class CxScreen
{
  public:

	CxScreen( void );
	// constructor
    
	~CxScreen( void );
	// destructor

    static void clearScreen(void);
    // clear the entire visible screen
    
	static void getCursorPosition(int *row, int *col);
	// get the row and column of the cursor on the screen

	static void moveCursorToColumn( int c );
	// move the cursor to the designated column on the row

	static void clearScreenFromCursorToEndOfLine( void );
	// erase the rest of the line from the cursor right	

    static void clearScreenFromCursorDown( void );
    // erase the rest of the line from the cursor right

    static void setScrollRegion(int topRow, int bottomRow);
    // set scrolling region (0-indexed rows, converts to 1-indexed for terminal)

    static void resetScrollRegion(void);
    // reset scroll region to full screen

    static void scrollUp(int lines);
    // scroll region content up by N lines (new blank lines appear at bottom)

    static void scrollDown(int lines);
    // scroll region content down by N lines (new blank lines appear at top)

    static void insertLines(int lines);
    // insert N blank lines at cursor position, pushing existing lines down

    static void deleteLines(int lines);
    // delete N lines at cursor position, pulling lines up

    static void beginSyncUpdate(void);
    // begin synchronized update - terminal buffers output until endSyncUpdate
    // supported by iTerm2, kitty, newer xterm, and other modern terminals

    static void endSyncUpdate(void);
    // end synchronized update - terminal renders buffered content atomically

    static void setWindowTitle(CxString title);
    // set the terminal window/tab title using OSC 0

    static void saveWindowTitle(void);
    // push current window title onto terminal's title stack

    static void restoreWindowTitle(void);
    // pop and restore previously saved window title from terminal's title stack

    static void enableMouseTracking(void);
    // enable terminal mouse tracking (SGR extended mode)

    static void disableMouseTracking(void);
    // disable terminal mouse tracking

	static void moveCursorLeft( int num );
	// move cursor to the left num characters

	static void moveCursorRight( int num );
	// move cursor to the right num characters

	static void moveCursorUp( int num );
	// move the cursor up num characters

	static void moveCursorDown( int num );
	// move the cursor down num characters

    static void saveCursorPosition(void);
    // save the current position (in the terminal)
    
    static void restoreCursorPosition(void);
    // restore the preveously saved (int the terminal) cursor position
    
    static void resetForegroundColor(void);
    
    static void resetBackgroundColor(void);

    static void openAlternateScreen(void);
    
    static void closeAlternateScreen(void);
    
    void flush(void);
    // flush the io buffer
    
    void hideCursor(void);
    void showCursor(void);

    static void setForegroundColor( CxColor *color );
    // set the text foreground color
    
    static void setBackgroundColor( CxColor *color );
    // set the text background color
    
    static void resetColors( void );
    // reset colors
    
    // receive sig winch message about terminal size change  

#if defined(_OSX_) || defined(_LINUX_) || defined(_SOLARIS_) || defined(_IRIX6_) || defined(_NETBSD_)
	static void sigWinchHandler(int sig);
#endif

#if defined(_SUNOS_)
    static void sigWinchHandler(int sig, struct sigaction *act, struct sigaction *oact );
#endif


    struct winsize getWindowSize(void);
    
    static int rows(void);
    
    static int cols(void);
    
    static void placeCursor( int row, int col);
        
    static void writeTextAt( unsigned long row, unsigned long col, CxString text, int clear );
    // write text at row, col for num characters and optionally clears the rest of the line
    // DEPRECATED
    
    /*static*/
    void writeText(CxString text);
    // write text at the cursor and performs a flush
    
    
  //  void sendEscapeCode(CxString s);
    // write an escape sequence to the screen
    
    static struct winsize ws;
    static struct sigaction sa;

    static CxSList< CxFunctor * > screenSizeCallbackQueue;
    void addScreenSizeCallback( CxFunctor *callback);

    static void refreshWindowSize(void);
    // re-read terminal dimensions from the kernel (call after external resize)

    static void setScreenAdjustments(int subRows, int subCols, int overRows, int overCols);

private:

    static int _subtractRows;
    static int _subtractCols;
    static int _overrideRows;
    static int _overrideCols;
};

#endif
