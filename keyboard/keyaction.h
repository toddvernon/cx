/***************************************************************************
 *
 *  CxKeyAction
 *
 *  CxKeyAction Class
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
        FUNCTION,               // key action is an unmapped function key
        BACKSPACE,              // key action is backspace
        NEWLINE,                // key action is newline (CR)
        TAB,                    // key action is the tab key
        COMMAND
    };
    
    CxKeyAction( CxString definition );
	// constructor  
  
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

  private:
    
    aType _actionType;
    CxString _tag;
    CxString _definition;
};


#endif
