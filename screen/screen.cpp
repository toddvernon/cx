//-------------------------------------------------------------------------------------------------
//
//  screen.cpp
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
#include <signal.h>
#include <termios.h>

//-------------------------------------------------------------------------
// SunOS 4.x: termios.h and sys/ioctl.h both define these macros.
// Undefine them before including sys/ioctl.h to avoid redefinition warnings.
// Also need extern "C" for sys/ioctl.h and ioctl() declaration.
//-------------------------------------------------------------------------
#if defined(_SUNOS_)
#undef ECHO
#undef NL0
#undef NL1
#undef TAB0
#undef TAB1
#undef TAB2
#undef XTABS
#undef CR0
#undef CR1
#undef CR2
#undef CR3
#undef FF0
#undef FF1
#undef BS0
#undef BS1
#undef TOSTOP
#undef FLUSHO
#undef PENDIN
#undef NOFLSH
extern "C" {
#include <sys/ioctl.h>
int ioctl(int fd, int request, ...);
}
#else
#include <sys/ioctl.h>
#endif

#include <cx/base/string.h>
#include <cx/screen/screen.h>

#define ESC 27

// must declare these here, learn something everyday
struct winsize CxScreen::ws;
struct sigaction CxScreen::sa;
CxSList< CxFunctor * > CxScreen::screenSizeCallbackQueue;
 
//-------------------------------------------------------------------------------------------------
// CxScreen::CxScreen
// 
// 
// 
//-------------------------------------------------------------------------------------------------
CxScreen::CxScreen( void )
{
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

#if defined(_SUNOS_) || defined(_IRIX6_)
    sa.sa_handler = (void (*)()) CxScreen::sigWinchHandler;
#endif

#if defined(_LINUX_) || defined(_OSX_) || defined(_SOLARIS_) || defined(_NETBSD_)
    sa.sa_handler = CxScreen::sigWinchHandler;
#endif
    
    if (ioctl(STDIN_FILENO, TIOCGWINSZ, (caddr_t) &(CxScreen::ws)) == -1) {
        throw CxException("CxSList::error in ioctl call to get screen size)");
    }
    
    if (sigaction(SIGWINCH, &(CxScreen::sa), NULL) == -1) {
        throw CxException("CxSList::at(invalid index)");
    }
}

void CxScreen::hideCursor(void)
{
    printf("%c[?25l", ESC);
    
//    fputs(CSI "?25l", stdout);
}

void CxScreen::showCursor(void)
{
    
//    fputs(CSI "?25h", stdout);
    printf("%c[?25h", ESC);
}


//-------------------------------------------------------------------------------------------------
// CxScreen::~CxScreen
// 
// 
// 
//-------------------------------------------------------------------------------------------------
CxScreen::~CxScreen( void )
{
}


//-------------------------------------------------------------------------------------------------
// CxScreen::addScreenSizeCallback
// 
// 
// 
//-------------------------------------------------------------------------------------------------
void
CxScreen::addScreenSizeCallback( CxFunctor *callback)
{
    screenSizeCallbackQueue.append( callback );
}


//-------------------------------------------------------------------------------------------------
// CxScreen::clearScreen
// 
// 
// 
//-------------------------------------------------------------------------------------------------
/*static*/
void
CxScreen::clearScreen(void)
{
    printf("%c[2J\033[1;1H", ESC);
}


//-------------------------------------------------------------------------------------------------
// CxScreen::getCursorPosition
//
//
//-------------------------------------------------------------------------------------------------
/*static*/
void
CxScreen::getCursorPosition(int *row, int *col)
{

	CxString buffer;
    printf("%c[6n", ESC);
	char c = getchar(); // get the ESC

	if (c != '\033') return;

	c = getchar(); // get the bracket
	if (c != '[') return;

	c = getchar();
	while( c!= 'R') {
		buffer += c;
		c = getchar();
	}

	CxString rowString = buffer.nextToken(";R");
	CxString colString = buffer.nextToken(";R");

	*row = rowString.toInt() -1;
	*col = colString.toInt() -1;
	 
	return;
}

//-------------------------------------------------------------------------------------------------
// CxScreen::sigWinchHandler
// 
// 
// 
//-------------------------------------------------------------------------------------------------
/*static*/

#if defined(_SUNOS_)
void CxScreen::sigWinchHandler(int sig, struct sigaction *, struct sigaction *)
{
    if (ioctl(STDIN_FILENO, TIOCGWINSZ, (char*) &(CxScreen::ws)) == -1) {
        printf("error in ioctl call\n");
        exit(0);
    }

    for (int c=0; c<screenSizeCallbackQueue.entries(); c++) {
        CxFunctor *f = screenSizeCallbackQueue.at(c);
        (*f)();
    }
}
#endif

#if defined(_OSX_) || defined(_LINUX_) || defined(_SOLARIS_) || defined(_IRIX6_) || defined(_NETBSD_)
void CxScreen::sigWinchHandler(int sig)
{
    if (ioctl(STDIN_FILENO, TIOCGWINSZ, (char*) &(CxScreen::ws)) == -1) {
        printf("error in ioctl call\n");
        exit(0);
    }
    
    //saveCursorPosition();
    
    for (int c=0; c<screenSizeCallbackQueue.entries(); c++) {
        CxFunctor *f = screenSizeCallbackQueue.at(c);
        (*f)();
    }
    
    //restoreCursorPosition();
}
#endif


//-------------------------------------------------------------------------------------------------
// CxScreen::getWindowSize
// 
// 
// 
//-------------------------------------------------------------------------------------------------
/*static*/
struct winsize
CxScreen::getWindowSize(void)
{
    return(ws);
}


//-------------------------------------------------------------------------------------------------
// CxScreen::rows
// 
// 
// 
//-------------------------------------------------------------------------------------------------
/*static*/
int
CxScreen::rows(void)
{
    return( ws.ws_row );
}


//-------------------------------------------------------------------------------------------------
// CxScreen::cols
// 
// 
// 
//-------------------------------------------------------------------------------------------------
/*static*/
int
CxScreen::cols(void)
{
    return( ws.ws_col );
}


//-------------------------------------------------------------------------------------------------
// CxScreen::placeCursor
// 
// 
// 
//-------------------------------------------------------------------------------------------------
/*static*/
void
CxScreen::placeCursor( int row, int col)
{
    printf("%c[%d;%dH", ESC, row+1, col+1);
}


//-------------------------------------------------------------------------------------------------
// CxScreen::resetForegroundColor
// 
// 
// 
//-------------------------------------------------------------------------------------------------
/*static*/
void
CxScreen::resetForegroundColor(void)
{
    printf("%c[39m", ESC);
}


//-------------------------------------------------------------------------------------------------
// CxScreen::openAlternateScreen
// 
// 
// 
//-------------------------------------------------------------------------------------------------
/*static*/
void
CxScreen::openAlternateScreen(void)
{
    printf("%c[?47h", ESC);
}


//-------------------------------------------------------------------------------------------------
// CxScreen::closeAlternateScreen
// 
// 
// 
//-------------------------------------------------------------------------------------------------
/*static*/
void
CxScreen::closeAlternateScreen(void)
{
    printf("%c[?47l", ESC);
}

 
//-------------------------------------------------------------------------------------------------
// CxScreen::resetBackgroundColor
// 
// 
// 
//-------------------------------------------------------------------------------------------------
/*static*/
void
CxScreen::resetBackgroundColor(void)
{
    printf("%c[49m", ESC);
}


//-------------------------------------------------------------------------------------------------
// CxScreen::moveCursorLeft
// 
// 
// 
//-------------------------------------------------------------------------------------------------
/*static*/
void
CxScreen::moveCursorLeft( int num )
{
	printf("%c[%dD", ESC, num );
}


//-------------------------------------------------------------------------------------------------
// CxScreen::moveCursorRight
// 
// 
// 
//-------------------------------------------------------------------------------------------------
/*static*/
void
CxScreen::moveCursorRight( int num )
{
	printf("%c[%dC", ESC, num );
}


//-------------------------------------------------------------------------------------------------
// CxScreen::moveCursorToColumn
// 
// 
// 
//-------------------------------------------------------------------------------------------------
/*static*/
void
CxScreen::moveCursorToColumn( int c )
{
	printf("%c[%dG", ESC, c+1 );
}


//-------------------------------------------------------------------------------------------------
// CxScreen::clearScreenFromCursorToEndOfLine
// 
// 
// 
//-------------------------------------------------------------------------------------------------
/*static*/
void
CxScreen::clearScreenFromCursorToEndOfLine( void )
{
	printf("%c[K", ESC);
}


//-------------------------------------------------------------------------------------------------
// CxScreen::clearScreenFromCursorDown
// 
// clears the remainder of the visible screen from cursor position right
// and down to end.
//
//-------------------------------------------------------------------------------------------------
/*static*/
void
CxScreen::clearScreenFromCursorDown( void )
{
    printf("%c[J", ESC);
}


//-------------------------------------------------------------------------------------------------
// CxScreen::saveCursorPosition
// 
// Tell the terminal to save its current position
// 
//-------------------------------------------------------------------------------------------------
/*static*/
void
CxScreen::saveCursorPosition(void)
{
	printf("%c[s", ESC);
}


//-------------------------------------------------------------------------------------------------
// CxCommandLine::restoreCursorPosition
// 
// Tell the terminal to restore the cursor position to its saved location
// 
//-------------------------------------------------------------------------------------------------
/*static*/
void
CxScreen::restoreCursorPosition(void)
{
	printf("%c[u", ESC);
}
 

//-------------------------------------------------------------------------------------------------
// CxCommandLine::writeTextAt 
// 
// 
// 
//-------------------------------------------------------------------------------------------------
/*static*/
void
CxScreen::writeTextAt( unsigned long row, unsigned long col, CxString text, int clear )
{
    unsigned long length = text.length();
    
    CxScreen::placeCursor( row, col );
    
    // for each character in the line
    for (unsigned long i=0; i<length; i++) {
        
        // get the character
        char ch = text.data()[i];
            
        if (ch == '\377') {
            putc(' ', stdout);
        } else if (ch == '\t') {
            putc(' ', stdout);
        } else {
            putc( ch, stdout );
        }
    }
    
    if (clear) {
        putc('\033', stdout);
        putc('[', stdout);
        putc('K', stdout);
    }

	// debug
//	usleep(1000);

}



/*static*/
void
CxScreen::writeText(CxString text)
{
    fputs( text.data(), stdout);
    flush();

//	usleep(100);
}


//-------------------------------------------------------------------------------------------------
// CxScreen::flush
// 
// 
// 
//-------------------------------------------------------------------------------------------------
/*static*/
void
CxScreen::flush(void)
{
    fflush(stdout);
}


//-------------------------------------------------------------------------------------------------
// CxScreen::setForegroundColor
// 
// 
// 
//-------------------------------------------------------------------------------------------------
/*static*/
void
CxScreen::setForegroundColor( CxColor *color )
{
    printf("%s",color->terminalString().data());
}

//-------------------------------------------------------------------------------------------------
// CxScreen::setBackgroundColor
// 
// 
// 
//-------------------------------------------------------------------------------------------------
/*static*/
void
CxScreen::setBackgroundColor( CxColor *color )
{
    printf("%s",color->terminalString().data());
}

//-------------------------------------------------------------------------------------------------
// CxScreen::resetColors
// 
// 
// 
//-------------------------------------------------------------------------------------------------
/*static*/
void
CxScreen::resetColors( void )
{
    printf("%c[0m", ESC);
}
