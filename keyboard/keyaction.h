//-------------------------------------------------------------------------------------------------
//
//  keyaction.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxKeyAction Class
//
//-------------------------------------------------------------------------------------------------

#include <cx/base/string.h>


#ifndef _KEYACTION_
#define _KEYACTION_ 


//-------------------------------------------------------------------------
// CxKeyAction
//
//-------------------------------------------------------------------------
class CxKeyAction
{
public:
    
    enum aType {
        NOTHING,                // no key returned
        UNKNOWN,                // unmapped key sequence
        CONTROL,                // key action is a control action
        OPTION,                 // key action is +option key modified
        SHIFT_OPTION,           // key action is shift+option modified
        UPPERCASE_ALPHA,        // jey action is a upper case alpha character
        LOWERCASE_ALPHA,        // key action is a lower case alpha character
        NUMBER,                 // key action is a number
        SYMBOL,                 // key action is a symbol
        CURSOR,                 // key action is an arrow key
        SHIFT_CURSOR,           // key action is an arrow modified with shift
        CTRL_CURSOR,            // key action is an arrow modified with ctrl
        FUNCTION,               // key action is an unmapped function key
        BACKSPACE,              // key action is backspace
        NEWLINE,                // key action is newline (CR)
        TAB,                    // key action is the tab key
        COMMAND,
        MOUSE_PRESS,            // mouse button pressed
        MOUSE_RELEASE,          // mouse button released
        MOUSE_DRAG,             // mouse moved while button held
        MOUSE_WHEEL,            // scroll wheel event
        MOUSE_DOUBLE_CLICK      // mouse double-click detected
    };
    
    CxKeyAction( CxString definition );
	// constructor

    CxKeyAction( aType type, int button, int row, int col, int modifiers );
	// mouse-specific constructor

    CxKeyAction( const CxKeyAction& a_);
	// copy constructor

    CxKeyAction& operator=( const CxKeyAction& a_ );
	// assignment operator

    int actionType(void);
	// classification of action

    CxString tag(void);
	// the tag

    CxString definition(void);
	// the key definition

    // mouse accessors
    int mouseButton(void);
	// mouse button: 1=left, 2=middle, 3=right, 4=wheel-up, 5=wheel-down

    int mouseRow(void);
	// terminal row (0-indexed)

    int mouseCol(void);
	// terminal column (0-indexed)

    int mouseModifiers(void);
	// modifier bit flags: 4=shift, 8=meta, 16=ctrl

    int mouseShift(void);
	// returns 1 if shift held, 0 otherwise

    int mouseCtrl(void);
	// returns 1 if ctrl held, 0 otherwise

  private:

    aType _actionType;
    CxString _tag;
    CxString _definition;

    // mouse event data
    int _mouseButton;       // 1=left, 2=middle, 3=right, 4=wheel-up, 5=wheel-down
    int _mouseRow;          // terminal row (0-indexed)
    int _mouseCol;          // terminal column (0-indexed)
    int _mouseModifiers;    // bit flags: 4=shift, 8=meta, 16=ctrl
};


#endif
