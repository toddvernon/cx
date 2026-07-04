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
#include <sys/types.h>
#include <sys/time.h>   // timeval for select() timeout
#if !defined(_SUNOS_)
#include <sys/select.h> // select() - not needed on SunOS 4.1.x (in sys/types.h)
#endif

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
    fputs("\033[?25l", stdout);
}

void CxScreen::showCursor(void)
{
    fputs("\033[?25h", stdout);
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
    fputs("\033[2J\033[1;1H", stdout);
}


//-------------------------------------------------------------------------------------------------
// CxScreen::readByteTimeout
//
// Read one byte from fd, waiting at most timeoutMs for it to arrive.
// Returns the byte, or -1 on timeout/error. Never blocks forever: a
// terminal (or pipe) that doesn't answer a query must not wedge us.
//
//-------------------------------------------------------------------------------------------------
/*static*/
int
CxScreen::readByteTimeout(int fd, int timeoutMs)
{
    fd_set rfds;
    struct timeval tv;

    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);
    tv.tv_sec  = timeoutMs / 1000;
    tv.tv_usec = (timeoutMs % 1000) * 1000;

    if (select(fd + 1, &rfds, NULL, NULL, &tv) != 1) return( -1 );

    unsigned char c;
    if (read(fd, (char *) &c, 1) != 1) return( -1 );
    return( c );
}


//-------------------------------------------------------------------------------------------------
// CxScreen::readCursorReport
//
// Read a DSR cursor-position report (ESC [ row ; col R) from stdin and
// return the 1-indexed row/col exactly as the terminal reported them.
// Leading bytes that aren't ESC (typed-ahead keys) are discarded. Bounded
// both by a per-byte timeout and a total byte budget, so a terminal that
// never answers -- or answers garbage -- gets a FALSE, not a hang.
//
//-------------------------------------------------------------------------------------------------
/*static*/
int
CxScreen::readCursorReport(int *row, int *col, int timeoutMs)
{
    CxString buffer;
    int c;
    int budget = 32;   // a real report is at most ~10 bytes

    // sync to the ESC that starts the report
    do {
        c = readByteTimeout(STDIN_FILENO, timeoutMs);
        if (c == -1) return( 0 );
    } while (c != '\033' && --budget > 0);
    if (c != '\033') return( 0 );

    c = readByteTimeout(STDIN_FILENO, timeoutMs);
    if (c != '[') return( 0 );

    while (budget-- > 0) {
        c = readByteTimeout(STDIN_FILENO, timeoutMs);
        if (c == -1) return( 0 );
        if (c == 'R') break;
        buffer += (char) c;
    }
    if (c != 'R') return( 0 );

    CxString rowString = buffer.nextToken(";");
    CxString colString = buffer.nextToken(";");
    if (!rowString.length() || !colString.length()) return( 0 );

    *row = rowString.toInt();
    *col = colString.toInt();
    return( 1 );
}


//-------------------------------------------------------------------------------------------------
// CxScreen::getCursorPosition
//
// Ask the terminal where the cursor is (DSR). 0-indexed. If the terminal
// never answers, row/col are left untouched -- callers pre-initialize.
// Requires raw/cbreak mode.
//
//-------------------------------------------------------------------------------------------------
/*static*/
void
CxScreen::getCursorPosition(int *row, int *col)
{
    int r, c;

    fputs("\033[6n", stdout);
    fflush(stdout);

    if (readCursorReport(&r, &c, 1000)) {
        *row = r - 1;
        *col = c - 1;
    }
    return;
}


//-------------------------------------------------------------------------------------------------
// CxScreen::syncTerminalSize
//
// Make the kernel's winsize agree with the terminal's real size. Serial
// consoles report 0x0 (nothing ever sets a size on them) and vintage
// telnetds report stale or missing sizes, so the terminal itself is the
// only authority: park the cursor at 999;999 (the terminal clamps the
// move to its true bottom-right corner), ask where the cursor actually
// landed (DSR), and write the answer back with TIOCSWINSZ so the kernel,
// SIGWINCH consumers and child processes all agree. This is exactly what
// xterm's resize(1) does, done in-process: no external binary, no
// per-platform path, no output flash.
//
// If the terminal never answers (dumb pipe), fall back to whatever the
// kernel had; if the kernel had nothing, force 24x80 rather than let a
// full-screen app run with 0 rows. Requires raw/cbreak mode.
//
// Returns TRUE if the terminal answered the probe.
//
//-------------------------------------------------------------------------------------------------
/*static*/
int
CxScreen::syncTerminalSize(void)
{
    int probedRows = 0;
    int probedCols = 0;
    int answered;

    fputs("\0337", stdout);           // DEC save cursor
    fputs("\033[999;999H", stdout);   // clamped to the real corner
    fputs("\033[6n", stdout);         // where did I land?
    fflush(stdout);

    answered = readCursorReport(&probedRows, &probedCols, 1000);

    fputs("\0338", stdout);           // DEC restore cursor
    fflush(stdout);

    refreshWindowSize();

    if (answered && probedRows > 0 && probedCols > 0) {
        if (probedRows != ws.ws_row || probedCols != ws.ws_col) {
            ws.ws_row = probedRows;
            ws.ws_col = probedCols;
            ioctl(STDIN_FILENO, TIOCSWINSZ, (caddr_t) &(CxScreen::ws));
        }
        return( 1 );
    }

    if (ws.ws_row == 0 || ws.ws_col == 0) {
        ws.ws_row = 24;
        ws.ws_col = 80;
        ioctl(STDIN_FILENO, TIOCSWINSZ, (caddr_t) &(CxScreen::ws));
    }
    return( 0 );
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
        fputs("error in ioctl call\n", stderr);
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
        fputs("error in ioctl call\n", stderr);
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
void
CxScreen::refreshWindowSize(void)
{
    ioctl(STDIN_FILENO, TIOCGWINSZ, (caddr_t) &(CxScreen::ws));
}


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
    char buf[32];
    sprintf(buf, "\033[%d;%dH", row+1, col+1);
    fputs(buf, stdout);
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
    fputs("\033[39m", stdout);
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
    fputs("\033[?47h", stdout);
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
    fputs("\033[?47l", stdout);
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
    fputs("\033[49m", stdout);
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
    char buf[16];
    sprintf(buf, "\033[%dD", num);
    fputs(buf, stdout);
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
    char buf[16];
    sprintf(buf, "\033[%dC", num);
    fputs(buf, stdout);
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
    char buf[16];
    sprintf(buf, "\033[%dG", c+1);
    fputs(buf, stdout);
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
    fputs("\033[K", stdout);
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
    fputs("\033[J", stdout);
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
    fputs("\033[s", stdout);
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
    fputs("\033[u", stdout);
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
    fputs(color->terminalString().data(), stdout);
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
    fputs(color->terminalString().data(), stdout);
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
    fputs("\033[0m", stdout);
}


//-------------------------------------------------------------------------------------------------
// CxScreen::setScrollRegion
//
// Set the scrolling region to specified rows (0-indexed, converted to 1-indexed for terminal).
// Only content within this region will scroll when scrollUp/scrollDown are called.
//
//-------------------------------------------------------------------------------------------------
/*static*/
void
CxScreen::setScrollRegion(int topRow, int bottomRow)
{
    char buf[32];
    sprintf(buf, "\033[%d;%dr", topRow + 1, bottomRow + 1);
    fputs(buf, stdout);
}


//-------------------------------------------------------------------------------------------------
// CxScreen::resetScrollRegion
//
// Reset the scroll region to the full screen (default terminal behavior).
//
//-------------------------------------------------------------------------------------------------
/*static*/
void
CxScreen::resetScrollRegion(void)
{
    fputs("\033[r", stdout);
}


//-------------------------------------------------------------------------------------------------
// CxScreen::scrollUp
//
// Scroll the content within the current scroll region up by N lines.
// New blank lines appear at the bottom of the region.
// Uses CSI n S (Scroll Up) escape sequence.
//
//-------------------------------------------------------------------------------------------------
/*static*/
void
CxScreen::scrollUp(int lines)
{
    if (lines > 0) {
        char buf[16];
        sprintf(buf, "\033[%dS", lines);
        fputs(buf, stdout);
    }
}


//-------------------------------------------------------------------------------------------------
// CxScreen::scrollDown
//
// Scroll the content within the current scroll region down by N lines.
// New blank lines appear at the top of the region.
// Uses CSI n T (Scroll Down) escape sequence.
//
//-------------------------------------------------------------------------------------------------
/*static*/
void
CxScreen::scrollDown(int lines)
{
    if (lines > 0) {
        char buf[16];
        sprintf(buf, "\033[%dT", lines);
        fputs(buf, stdout);
    }
}


//-------------------------------------------------------------------------------------------------
// CxScreen::insertLines
//
// Insert N blank lines at the current cursor position.
// Existing lines from cursor down are pushed down (within scroll region if set).
// Uses CSI n L (Insert Lines) escape sequence.
//
//-------------------------------------------------------------------------------------------------
/*static*/
void
CxScreen::insertLines(int lines)
{
    if (lines > 0) {
        char buf[16];
        sprintf(buf, "\033[%dL", lines);
        fputs(buf, stdout);
    }
}


//-------------------------------------------------------------------------------------------------
// CxScreen::deleteLines
//
// Delete N lines at the current cursor position.
// Existing lines below are pulled up, blank lines appear at bottom (within scroll region if set).
// Uses CSI n M (Delete Lines) escape sequence.
//
//-------------------------------------------------------------------------------------------------
/*static*/
void
CxScreen::deleteLines(int lines)
{
    if (lines > 0) {
        char buf[16];
        sprintf(buf, "\033[%dM", lines);
        fputs(buf, stdout);
    }
}


//-------------------------------------------------------------------------------------------------
// CxScreen::beginSyncUpdate
//
// Begin synchronized update mode. The terminal buffers all subsequent output
// until endSyncUpdate() is called, then renders everything in a single
// atomic operation. This eliminates flicker and tearing during complex
// screen updates.
//
// Uses DEC private mode 2026 (CSI ? 2026 h).
// Supported by: iTerm2 3.3+, kitty, foot, contour, wezterm, newer xterm.
// Terminals that don't support it will ignore the sequence harmlessly.
//
//-------------------------------------------------------------------------------------------------
// Static buffer for stdout batching. Large enough for a full screen redraw.
// Used by beginSyncUpdate/endSyncUpdate to hold all output and flush once.
static char syncUpdateBuffer[256 * 1024];

/*static*/
void
CxScreen::beginSyncUpdate(void)
{
    // Switch stdout to full buffering so all writes accumulate in our buffer.
    // This eliminates flicker on terminals that don't support mode 2026.
    setvbuf(stdout, syncUpdateBuffer, _IOFBF, sizeof(syncUpdateBuffer));

    // Also send mode 2026 for terminals that support it (double protection)
    fputs("\033[?2026h", stdout);
}


//-------------------------------------------------------------------------------------------------
// CxScreen::endSyncUpdate
//
// End synchronized update mode. The terminal renders all buffered content
// from since beginSyncUpdate() was called in a single atomic frame.
//
// Uses DEC private mode 2026 (CSI ? 2026 l) plus stdout flush for portability.
//
//-------------------------------------------------------------------------------------------------
/*static*/
void
CxScreen::endSyncUpdate(void)
{
    fputs("\033[?2026l", stdout);

    // Flush everything accumulated since beginSyncUpdate, then restore
    // line buffering so interactive output (cursor moves, etc.) works normally.
    fflush(stdout);
    setvbuf(stdout, NULL, _IOLBF, 0);
}


//-------------------------------------------------------------------------------------------------
// CxScreen::setWindowTitle
//
// Set the terminal window/tab title using OSC 0 (Operating System Command).
// Works on iTerm2, xterm, and most Linux terminal emulators.
// Terminals that don't support it will ignore the sequence harmlessly.
//
//-------------------------------------------------------------------------------------------------
/*static*/
void
CxScreen::setWindowTitle(CxString title)
{
    fputs("\033]0;", stdout);
    fputs(title.data(), stdout);
    fputc(7, stdout);
}


//-------------------------------------------------------------------------------------------------
// CxScreen::saveWindowTitle
//
// Push the current window title onto the terminal's internal title stack.
// Uses xterm's CSI 22;0t sequence. Pair with restoreWindowTitle() to
// restore the original title when the application exits.
//
// Supported by: xterm, iTerm2, kitty, foot, wezterm, most Linux terminals.
//
//-------------------------------------------------------------------------------------------------
/*static*/
void
CxScreen::saveWindowTitle(void)
{
    fputs("\033[22;0t", stdout);
}


//-------------------------------------------------------------------------------------------------
// CxScreen::restoreWindowTitle
//
// Pop and restore the previously saved window title from the terminal's
// internal title stack. Uses xterm's CSI 23;0t sequence.
//
//-------------------------------------------------------------------------------------------------
/*static*/
void
CxScreen::restoreWindowTitle(void)
{
    fputs("\033[23;0t", stdout);
}


//-------------------------------------------------------------------------------------------------
// CxScreen::enableMouseTracking
//
// Enable terminal mouse tracking using SGR extended mode. This provides:
// - Button press events
// - Button release events
// - Mouse drag (button-motion) events
// - Scroll wheel events
// - Extended coordinate support (handles terminals wider/taller than 223)
//
// Supported by modern terminals: iTerm2, Terminal.app, xterm, kitty, etc.
//
//-------------------------------------------------------------------------------------------------
/*static*/
void
CxScreen::enableMouseTracking(void)
{
    fputs("\033[?1000h", stdout);  // Enable basic mouse tracking
    fputs("\033[?1002h", stdout);  // Enable button-event tracking (drag)
    fputs("\033[?1006h", stdout);  // Enable SGR extended coordinates
    fflush(stdout);
}


//-------------------------------------------------------------------------------------------------
// CxScreen::disableMouseTracking
//
// Disable terminal mouse tracking. Should be called before exiting the
// application to restore normal terminal behavior.
//
//-------------------------------------------------------------------------------------------------
/*static*/
void
CxScreen::disableMouseTracking(void)
{
    fputs("\033[?1006l", stdout);  // Disable SGR extended
    fputs("\033[?1002l", stdout);  // Disable button-event
    fputs("\033[?1000l", stdout);  // Disable basic tracking
    fflush(stdout);
}
