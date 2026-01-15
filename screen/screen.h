//-------------------------------------------------------------------------------------------------
//
// CxScreen.h
//
// CxScreen class
//
//-------------------------------------------------------------------------------------------------
//
// (c) Copyright 2018-2023 T.Vernon
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
   
};

#endif

