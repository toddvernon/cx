//-------------------------------------------------------------------------------------------------
//
//  keyboard.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxKeyboard Class
//
//-------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <termios.h>    //termios, TCSANOW, ECHO, ICANON
#include <unistd.h>     //STDIN_FILENO
#include <iostream>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>   // timeval for select() timeout
#if !defined(_SUNOS_)
#include <sys/select.h> // select() - not needed on SunOS 4.1.x (in sys/types.h)
#endif

#include <cx/base/string.h>
#include <cx/base/hashmap.h>
#include <cx/base/slist.h>
#include <cx/functor/defercall.h>
#include <cx/keyboard/keyaction.h>


#ifndef _CXKEYBOARD_
#define _CXKEYBOARD_


//-------------------------------------------------------------------------
// CxKeyboard
//
//-------------------------------------------------------------------------

class CxKeyboard
{
  public:
    
    enum BLOCKING {
        WAIT,
        NO_WAIT
    };
    
    CxKeyboard(void);
    // constructor
    
    ~CxKeyboard( void );
    // destructor
    
    CxKeyAction getAction( void );
    // get the next keyboard action
    
    static char readKey( CxKeyboard::BLOCKING blocking);
        // read a key

    static void addIdleCallback( CxFunctor *callback );
        // add a callback to be called during keyboard idle (every ~100ms when waiting for input)

    static CxSList< CxFunctor * > idleCallbackQueue;
        // queue of callbacks to call during idle

    static void setDoubleClickThreshold( int milliseconds );
        // set the double-click detection threshold (default 400ms)

    static int getDoubleClickThreshold( void );
        // get the current double-click threshold in milliseconds

  private:
    
    void setupTerminalFeatures(void);
    // setup terminal to no echo, char-by-char input
    
    void teardownTerminalFeatures(void);
    // restore terminal to orginal condition
    
    
    
    static CxString handleOptionSequence(int c);
    // handle known sequences that represet option key combinations
    
    static CxString handleOptionSequence2(int c);
    // handle 2 character option key sequences
    
    static CxString handleOptionSequence3(int c);
    // handle 3 character option key sequences
    
    static CxString handleEscapeSequence(int c);
    // handle known escape sequences

    static CxString handleSGRMouseSequence(void);
    // handle SGR extended mouse sequence (ESC [ < ...)

    static CxString handleControlKeySequence(int c );
    // handle known control key codes

    static CxString handleKey( int c );
    // handle normal ascii keys
    
    CxHashmap< CxString, CxString > keyHash;

    struct termios _oldt;
    struct termios _newt;

    // Double-click detection state (static so it persists across getAction calls)
    static struct timeval _lastClickTime;
    static int _lastClickRow;
    static int _lastClickCol;
    static int _lastClickButton;
    static int _lastClickValid;  // 1 if last click data is valid
    static int _doubleClickThresholdMs;  // configurable threshold (default 400)

    static int isDoubleClick(int button, int row, int col);
    // Check if current click qualifies as double-click (same position, within threshold)

    static void recordClick(int button, int row, int col);
    // Record click for future double-click detection
};

#endif
