/***************************************************************************
 *
 *  CxCommandLine.cpp
 *
 *  CxCommandLine Class
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
#include <termios.h>
#include <unistd.h>

#include <iostream>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include <cx/base/string.h>
#include <cx/base/slist.h>
#include <cx/base/rule.h>
#include <cx/base/exception.h>
#include <cx/base/file.h>
#include <cx/base/star.h>
#include <cx/base/filename.h>
#include <cx/base/directory.h>
#include <cx/functor/functor.h>
#include <cx/functor/defercall.h>
#include <cx/functor/reference.h>

#include <cx/keyboard/keyaction.h>
#include <cx/keyboard/keyboard.h>

#include <cx/commandline/command.h>
#include <cx/commandline/commandline.h>


#define ESC_KEY          27

//-------------------------------------------------------------------------
// CxCommandLine::CxCommandLine
//
//-------------------------------------------------------------------------
CxCommandLine::CxCommandLine( void )
{
	_first  = TRUE;
	_prompt = "";
	_cursorColumn = 1;
    
    setToEditMode();
}


//-------------------------------------------------------------------------
// CxCommandLine::~CxCommandLine
//
//-------------------------------------------------------------------------
CxCommandLine::~CxCommandLine( void )
{
}


//-------------------------------------------------------------------------
// CxCommandLine::registerCommandCallback
//
//-------------------------------------------------------------------------
void
CxCommandLine::registerCommandCallback( CxCommand command )
{
    _commandCallbackList.append(command);
}


//-------------------------------------------------------------------------
// CxCommandLine::registerReplacementCallback
//
//-------------------------------------------------------------------------
void
CxCommandLine::registerReplacementCallback( CxCommand command)
{
    _replacementCallbackList.append(command);
}


//-------------------------------------------------------------------------
// CxCommandLine::setPrompt
//
//-------------------------------------------------------------------------
void
CxCommandLine::setPrompt(CxString p)
{
    _prompt = p;
    _cursorColumn = _prompt.length()+2;
    CxScreen::moveCursorToColumn( _cursorColumn );
    
}


//-------------------------------------------------------------------------
// CxCommandLine::setPromptForegroundColor
//
//-------------------------------------------------------------------------
/*
void
CxCommandLine::setPromptForegroundColor( CxColor c)
{
    _promptForegroundColor = c;
}

void
CxCommandLine::setPromptBackgroundColor( CxColor c)
{
    _promptBackgroundColor = c;
}
*/

//-------------------------------------------------------------------------
// CxCommandLine::setCommandHistory
//
//-------------------------------------------------------------------------
void
CxCommandLine::setCommandHistory( CxSList< CxString > ch)
{
    _commandLineHistory = ch;
}

void
CxCommandLine::appendCommandHistory( CxString cmd)
{
    _commandLineHistory.append( cmd );
}


//-------------------------------------------------------------------------
// CxCommandLine::setCommandFuture
//
//-------------------------------------------------------------------------
void
CxCommandLine::setCommandFutures( CxSList< CxString > f)
{
    _commandLineFutures = f;
}

void
CxCommandLine::insertCommandFutures( CxSList< CxString > f)
{
    try {
        for (int c=0; c<f.entries(); c++) {
            CxString cmd = f.at(c);
            _commandLineFutures.insertAtHead( cmd );
        }
    }
    
    catch( CxException e) {
        printf("exception=%s\n", e.why().data());
    }
}

//-------------------------------------------------------------------------
// CxCommandLine::setDone
//
//-------------------------------------------------------------------------
void
CxCommandLine::setDone(void)
{
	_doneWithLine = TRUE;
}

//-------------------------------------------------------------------------
// CxCommandLine::previousKeyWasTab
//
//-------------------------------------------------------------------------
int
CxCommandLine::previousKeyWasTab(void)
{
    return( _lastKeyWasTab );
}


//-------------------------------------------------------------------------
// CxCommandLine::
//
//-------------------------------------------------------------------------
void
CxCommandLine::getInput(void)
{
	if (_first) {

        _first=FALSE;
		
        printCommandLine();

        _cursorColumn = _prompt.length()+1;
        CxScreen::moveCursorToColumn( _cursorColumn );
	}
    
    if (_commandLineFutures.entries() ) {
        CxString nextCommandLine = _commandLineFutures.at(0);
        _commandLineFutures.removeAt(0);
        doCannedCommand( nextCommandLine );
        return;
    }

	_doneWithLine  = FALSE;
    _lastKeyWasTab = FALSE;
    
    while( !_doneWithLine) {
        
        CxKeyAction keyAction = _keyboard.getAction();
    
        switch (keyAction.actionType() ) {
            
            case CxKeyAction::LOWERCASE_ALPHA:
            case CxKeyAction::UPPERCASE_ALPHA:
            case CxKeyAction::NUMBER:
            case CxKeyAction::SYMBOL:
                _lastKeyWasTab = FALSE;
                processCharacter(keyAction);
                break;
            
            case CxKeyAction::NEWLINE:
                _lastKeyWasTab = FALSE;
                processReturnKey();
                break;
            
            case CxKeyAction::BACKSPACE:
                _lastKeyWasTab = FALSE;
                processDeleteKey();
                break;
            
            case CxKeyAction::TAB:
                processTabKey();
                _lastKeyWasTab = TRUE;
                break;
            
            case CxKeyAction::CURSOR:
                _lastKeyWasTab = FALSE;
                processArrowKeys(keyAction);
                break;
                
            case CxKeyAction::COMMAND:
                _commandLine = "";
                processReturnKey();
                break;
                
            default:
                _lastKeyWasTab=FALSE;
                break;
        }
    }
}

//-------------------------------------------------------------------------
// CxCommandLine::
//
//-------------------------------------------------------------------------
void
CxCommandLine::processArrowKeys(CxKeyAction keyAction)
{
    if (keyAction.tag() == "<arrow-left>") {
        processLeftArrow();
    }
    
    if (keyAction.tag() == "<arrow-right>") {
        processRightArrow();
    }
    
    if (keyAction.tag() == "<arrow-up>") {
        processUpArrow();
    }
    
    if (keyAction.tag() == "<arrow-down>") {
        processDownArrow();
    }
}


//-------------------------------------------------------------------------
// CxCommandLine::
//
//-------------------------------------------------------------------------
void
CxCommandLine::setToEditMode()
{
    _historyCursor = -1;
}


//-------------------------------------------------------------------------
// CxCommandLine::
//
//-------------------------------------------------------------------------
int
CxCommandLine::isEditMode(void)
{
    if (_historyCursor==-1) return(TRUE);
    return(FALSE);
}


//-------------------------------------------------------------------------
// CxCommandLine::
//
//-------------------------------------------------------------------------
void
CxCommandLine::processCharacter(CxKeyAction keyAction)
{
    // set to edit mode if not already set
    setToEditMode();
    
    // insert the character at the cursor position
    _commandLine.insert( keyAction.tag(), _cursorColumn-1-_prompt.length());
    
    // reprint the commandline
    printCommandLine( );
    
    // move cursor to new character over
    CxScreen::moveCursorToColumn( ++_cursorColumn );
 
}


//-------------------------------------------------------------------------
// CxCommandLine::
//
//-------------------------------------------------------------------------
void
CxCommandLine::processTabKey(void)
{
    CxString word;
    
    setToEditMode();
    
    int index =(_cursorColumn-2-_prompt.length());
 
    if (index==-1) {
        //printf("no text to search\n");
        return;
    }
    
    char *firstCharPtr = _commandLine.data();
    char *lastCharPtr  = _commandLine.data() + index;
    char *cptr         = lastCharPtr;
    
    CxString validNumericSet("0123456789");
    CxString validAlphaSet("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
    CxString validSymbolSet("_:./~-+");
    CxString validSet = validAlphaSet + validNumericSet + validSymbolSet;
    
    while (validSet.charInString(*cptr) && (cptr >= firstCharPtr)) {
        //printf("char=[%c]\n", *cptr);
        cptr--;
    }
    
    cptr++;
    
    int replacementStartPoint = cptr - firstCharPtr;
    int deleteLength          = lastCharPtr - cptr +1;

    _completionString  = "";
    _replacementString = "";

    // build up the completion string
    while (cptr <= lastCharPtr) {
        _completionString.append(*cptr);
        cptr++;
    }

    // if we are in position zero, then this is a command
    // replacement attempt
    
    if ((replacementStartPoint==0) && (deleteLength)) {
        
        // printf("looking for command replacement for [%s]\n", word.data());

        CxSList<CxString> candidateCommands = findMatchingCommandNames( _completionString );
        if (candidateCommands.entries()) {
            _replacementString = candidateCommands.at(0);
        }
        
        // if the replacement string is something
        if (_replacementString.length()) {
            
            // remove the original string, and insert the new string
            _commandLine.remove(replacementStartPoint, deleteLength);
            _commandLine.insert(_replacementString, replacementStartPoint);
            
            // print the commandline
            printCommandLine();
            
            // put the cursor back into the right spot
            _cursorColumn = _prompt.length()+replacementStartPoint+_replacementString.length()+1;
            CxScreen::moveCursorToColumn( _cursorColumn );
            
        } else {
            
            // nothing in the replacement string, so just redraw
            printCommandLine();
            CxScreen::moveCursorToColumn( _cursorColumn );
        }
        
        return;
    
        
    // if the completion string did not start at position zero then
    // we are doing a replacement specific to a command argument, so
    // call the replacement callback for that command
        
    } else {
        
        // printf("looking for other replacement for [%s]\n", word.data());

        CxString cl = _commandLine;
        CxString commandName = cl.nextToken(" \t");
        
        callReplacementCallbacks( commandName );
        
        if (_replacementString.length()) {
            
            // remove the old string, and replace with new string
            _commandLine.remove(replacementStartPoint, deleteLength);
            _commandLine.insert(_replacementString, replacementStartPoint);
            
            // print the commandline
            printCommandLine();
            
            // calculate the new cursor position, and place on screen
            _cursorColumn = _prompt.length()+replacementStartPoint+_replacementString.length()+1;
            CxScreen::moveCursorToColumn( _cursorColumn );
        
        } else {
            
            // no replacement string, just reprint everything
            printCommandLine();
            CxScreen::moveCursorToColumn( _cursorColumn );

        }
    }
}

// set command to _commandLine
// print commandline
// processReturnKey
//


//-------------------------------------------------------------------------
// CxCommandLine::
//
//-------------------------------------------------------------------------
void
CxCommandLine::doCannedCommand(CxString cl)
{
    _commandLine = cl;
    _lastKeyWasTab=FALSE;
    printCommandLine();
    processReturnKey();
}

int
CxCommandLine::isExecutingScript(void)
{
    if (_commandLineFutures.entries()) return TRUE;
    return(FALSE);
}

void
CxCommandLine::setCommandLine(CxString text)
{
    setToEditMode();
    printCommandLine();
      
    _cursorColumn = _prompt.length()+1;
    CxScreen::moveCursorToColumn( _cursorColumn );

    _commandLine = text;
    _cursorColumn = _commandLine.length()+_prompt.length()+1;

    printCommandLine();
    
    CxScreen::moveCursorToColumn( _cursorColumn );
    CxScreen::clearScreenFromCursorToEndOfLine();
    
    _first = false;
    
    
}



//-------------------------------------------------------------------------
// CxCommandLine::
//
//-------------------------------------------------------------------------
void
CxCommandLine::processReturnKey(void)
{
    setToEditMode();
    
    CxString cl = _commandLine;
    CxString commandName = cl.nextToken(" \t");

    // if we have a complete command on the command line
    // call any callbacks registered for it
    callReturnCallbacks( commandName );
		
    // set commandline to empty
    _commandLine = "";

    // print the commandline
    //printCommandLine();
    
    // calculate the new cursor position, and set
    _cursorColumn = _commandLine.length()+1+_prompt.length();
    CxScreen::moveCursorToColumn( _cursorColumn );

    // erase anything still on the screen past that point
    ///CxScreen::clearScreenFromCursorToEndOfLine();

    // done editing the commandline
    _doneWithLine = TRUE;
}


//-------------------------------------------------------------------------
// CxCommandLine::
//
//-------------------------------------------------------------------------
CxSList< CxString >
CxCommandLine::findMatchingCommandNames( CxString partialCommand )
{
    CxSList< CxString > keeperList;
    CxMatchTemplate temp(partialCommand+CxString("*"));
    
    for( int i=0; i<_commandCallbackList.entries(); i++) {
        CxCommand command = _commandCallbackList.at(i);
        if (temp.test(command.name())) {
            keeperList.append(command.name());
        }
    }

    return(keeperList);
}


//-------------------------------------------------------------------------
// CxCommandLine::
//
//-------------------------------------------------------------------------
void
CxCommandLine::callReturnCallbacks(CxString commandName)
{
    int anyCalled = FALSE;
    
    // call any callback registered with the command name
    
    for( int i=0; i<_commandCallbackList.entries(); i++) {
        CxCommand command = _commandCallbackList.at(i);
        if (command.name() == commandName ) {
            command.call();
            anyCalled = TRUE;
        }
    }
    
    // if no callback was registered for the command then call
    // the unknown callback
    
    if (!anyCalled) {
        for( int i=0; i<_commandCallbackList.entries(); i++) {
            CxCommand command = _commandCallbackList.at(i);
            if (command.name() == "<unknown>" ) {
                command.call();
            }
        }
    }
}


//-------------------------------------------------------------------------
// CxCommandLine::
//
//-------------------------------------------------------------------------
void
CxCommandLine::callReplacementCallbacks(CxString commandName)
{
    // if we have a registered callback for the current
    // command name, call the callback for parameter completion
    
    for( int i=0; i<_replacementCallbackList.entries(); i++) {
        CxCommand command = _replacementCallbackList.at(i);
        if (command.name() == commandName ) {
            command.call();
        }
    }
}

//-------------------------------------------------------------------------
// CxCommandLine::
//
//-------------------------------------------------------------------------
CxString
CxCommandLine::completionString(void)
{
    return(_completionString);
}


//-------------------------------------------------------------------------
// CxCommandLine::
//
//-------------------------------------------------------------------------
void
CxCommandLine::setReplacementString(CxString rs)
{
    _replacementString = rs;
}


//-------------------------------------------------------------------------
// CxCommandLine::
//
//-------------------------------------------------------------------------
void
CxCommandLine::processDeleteKey(void)
{
	if (_cursorColumn>_prompt.length()+1) {

		// save the starting cursor column
		int startingCursorColumn = _cursorColumn;

		// remove the character from the array
		_commandLine.remove( _cursorColumn-2-_prompt.length(), 1);
		
		// print the commandline (leaving cursor at beginning of line
		printCommandLine();
	
		// move back to end of text, remove rest on screen
        _cursorColumn = _commandLine.length()+1+_prompt.length();
        CxScreen::moveCursorToColumn( _cursorColumn );
        CxScreen::clearScreenFromCursorToEndOfLine();

		// reset the cursor position to where we started, minus 1
		_cursorColumn = startingCursorColumn-1;

        // place the cursor on the screen
        CxScreen::moveCursorToColumn( _cursorColumn );

	}
}

//-------------------------------------------------------------------------
// CxCommandLine::
//
//-------------------------------------------------------------------------
void
CxCommandLine::processRightArrow(void)
{
    int len = _commandLine.length();
    if (_cursorColumn<=len+_prompt.length()) {
        CxScreen::moveCursorRight(1);
        _cursorColumn++;
    }
}

//-------------------------------------------------------------------------
// CxCommandLine::
//
//-------------------------------------------------------------------------
void
CxCommandLine::processLeftArrow(void)
{
    if (_cursorColumn>1+_prompt.length() ) {
        CxScreen::moveCursorLeft(1);
        _cursorColumn--;
    }
}


//-------------------------------------------------------------------------
// CxCommandLine::
//
//-------------------------------------------------------------------------
void
CxCommandLine::processUpArrow(void)
{
    CxString lastCommandLine;
    
    if (isEditMode()) {
        
        if (_commandLine.length()) {
            _commandLineHistory.append(_commandLine);
        }
        
        // editing the commandline
        _historyCursor  = _commandLineHistory.entries()-1;
        lastCommandLine = _commandLineHistory.at( _historyCursor );
        
    } else {
    
        _historyCursor--;
        if (_historyCursor<0) {
            _historyCursor = 0;
            return;
        } else {
            lastCommandLine = _commandLineHistory.at( _historyCursor );
        }
    }
    
    _commandLine = lastCommandLine;
    
    printCommandLine();
    
    _cursorColumn = _commandLine.length()+1+_prompt.length();
    CxScreen::moveCursorToColumn( _cursorColumn );
    CxScreen::clearScreenFromCursorToEndOfLine();

}



//-------------------------------------------------------------------------
// CxCommandLine::
//
//-------------------------------------------------------------------------
void
CxCommandLine::processDownArrow(void)
{
    if (isEditMode()) {

        return;
 
    } else {
        
        _historyCursor++;
        
        if (_historyCursor==_commandLineHistory.entries()) {

            setToEditMode();
            _commandLine = "";
            printCommandLine();
            
            _cursorColumn = _commandLine.length()+1+_prompt.length();
            
            CxScreen::moveCursorToColumn( _cursorColumn );
            CxScreen::clearScreenFromCursorToEndOfLine();


        } else {
            
            _commandLine = _commandLineHistory.at( _historyCursor );
            printCommandLine();

            _cursorColumn = _commandLine.length()+1+_prompt.length();
            
            CxScreen::moveCursorToColumn( _cursorColumn );
            CxScreen::clearScreenFromCursorToEndOfLine();
        }
    }
}


//-------------------------------------------------------------------------
// CxCommandLine::
//
//-------------------------------------------------------------------------
void
CxCommandLine::printCommandLine(void)
{
    //CxScreen::setForegroundColor( _promptForegroundColor );
    
    printf("\r%s", _prompt.data());
    //CxScreen::resetForegroundColor();
    printf("%s", _commandLine.data() );
    
    fflush(stdout);
}


//-------------------------------------------------------------------------
// CxCommandLine::
//
//-------------------------------------------------------------------------
CxString
CxCommandLine::commandLine(void)
{
	return(_commandLine);
}


//-------------------------------------------------------------------------
// CxCommandLine::
//
//-------------------------------------------------------------------------
CxString
CxCommandLine::lastCommandLine(void)
{
    if (_commandLineHistory.entries()) {
        return( _commandLineHistory.at( _commandLineHistory.entries()-1));
    }
    
    return("");
}
