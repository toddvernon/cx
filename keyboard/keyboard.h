/***************************************************************************
 *
 *  keyboard
 *
 *  CxKeyboard Class
 *
 ***************************************************************************
 *
 * (c) Copyright 2018 T.Vernon
 * ALL RIGHTS RESERVED
 *
 * The software and information contained herein are proprietary to, and
 * comprise valuable trade secrets of, T.Vernon , which
 * intends to preserve as trade secrets such software and information.
 * This software is furnished pursuant to a written license agreement and
 * may be used, copied, transmitted, and stored only in accordance with
 * the terms of such license and with the inclusion of the above copyright
 * notice.  This software and information or any other copies thereof may
 * not be provided or otherwise made available to any other person.
 *
 *
 **************************************************************************/
#include <stdio.h>
#include <termios.h>    //termios, TCSANOW, ECHO, ICANON
#include <unistd.h>     //STDIN_FILENO
#include <iostream>
#include <fcntl.h>
#include <errno.h>

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
    
    static CxString handleControlKeySequence(int c );
    // handle known control key codes
    
    static CxString handleKey( int c );
    // handle normal ascii keys
    
    CxHashmap< CxString, CxString > keyHash;
    
    struct termios _oldt;
    struct termios _newt;
};

#endif
