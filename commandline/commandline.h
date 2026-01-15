/***************************************************************************
 *
 *  CxCommandLine.h
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
#include <cx/base/file.h>
#include <cx/base/star.h>
#include <cx/base/filename.h>
#include <cx/base/directory.h>
#include <cx/functor/functor.h>
#include <cx/functor/defercall.h>
#include <cx/functor/reference.h>

#include <cx/screen/screen.h>

#include <cx/keyboard/keyaction.h>
#include <cx/keyboard/keyboard.h>

#include <cx/commandline/command.h>



#ifndef _CxCOMMANDLINE_
#define _CxCOMMANDLINE_


//-------------------------------------------------------------------------
// CxCommandLine::
//
//-------------------------------------------------------------------------
class CxCommandLine
{
  public:

	CxCommandLine( void );
	// constructor
    
	~CxCommandLine( void );
	// destructor

    void registerCommandCallback( CxCommand command );
    // register a command callback
    
    void registerReplacementCallback( CxCommand command );
    // register a tab callback
    
    void setCommandHistory( CxSList< CxString > ch);
    // preload the command history
    
    void appendCommandHistory( CxString cmd );
    // add a command to the command history
    
    void setCommandFutures( CxSList< CxString > f);
    // set commands to run automatically
    
    void insertCommandFutures( CxSList< CxString > f);
    
	void setPrompt( CxString p );
	// set the current prompt

	CxString commandLine(void);
	// get the current commandline
    
    CxString lastCommandLine(void);
    // get the last command line entered

	void getInput(void);
	// collect an input

	void setDone(void);
	// terminate input collection
    
    void setToEditMode(void);
	// exit history scroll, go back to edit mode	
    
    int isEditMode(void);
	// return if command line is in edit mode

    //void setPromptForegroundColor( CxColor c);
	// set the rgb value from the prompt
	// on terminals that support it
    
    //void setPromptBackgroundColor( CxColor c);
    // set the color for the prompt background color
    
    CxString completionString(void);
    // used to get the word currently being edited
    // when the tab key was pushed and the tab callbacks
    // are called
    
    void setReplacementString( CxString rs);
    // used to set the replacement string when the tab
    // callback is called.
    
    int previousKeyWasTab(void);
    // used by a callback to test if this tab key press
    // is the second tab key press in a row
    
    void
    doCannedCommand(CxString cl);
    // do a command string that came from someplace else
    
    int
    isExecutingScript(void);
    // returns true if the commandline is currently executing
    // a script
    
    void
    setCommandLine(CxString text);
    // prepoulate the command line with text
    
    void printCommandLine(void);
    // print the current input field
    
    void processCharacter(CxKeyAction keyAction);
    // process a normal character
    
  private:


    void processArrowKeys(CxKeyAction keyAction);
    // move cursor and insertion point
    
    void processTabKey(void);
    // hand the tab key

	void processDeleteKey(void);
	// handle a delete key

	void processReturnKey(void);
	// handle return key
    
    void processUpArrow(void);
    // process pressing the up arrow

    void processDownArrow(void);
    // process pressing the down arrow
    
    void processRightArrow(void);
    // process pressing the right arrow
    
    void processLeftArrow(void);
    // process pressing the left arrow
    
    void callReplacementCallbacks(CxString commandName);
    // call any registered tab callbacks
    
    void callReturnCallbacks(CxString commandName);
    // call any registered tab callbacks
    
    CxSList< CxString > findMatchingCommandNames( CxString partialCommand );
    // get a list of commands that match
    
    CxString _commandLine;
    // holds the current command line being edited
    
	CxString _prompt;
    // the prompt
    
    //CxColor _promptForegroundColor;
    // the prompt color
    
    //CxColor _promptBackgroundColor;
    // reset the prompt color
    
    CxKeyboard _keyboard;
    // the keyboard object to read characters
    
    CxSList<CxString>  _commandLineFutures;
    // commands to run rather than get from input
    
	CxSList<CxString>  _commandLineHistory;
    // list of prior command lines
    
    CxSList<CxCommand> _commandCallbackList;
    // list of finished command callbacks
    
    CxSList<CxCommand> _replacementCallbackList;
    // list of callbacks to call on tab replacement
    
	int _cursorColumn;
    // current position of the cursor on the screen
    
    int _lastKeyWasTab;
    // is true when tab key is pressed the second time
    
	int _first;
    // first time flag for edit loop
    
    int _doneWithLine;
    // set when a commandline is finished being edited
    
	int _historyCursor;
    // the cursor into the history list navigated but up and
    // down arrows
    
    CxString _completionString;
    // set when replacment callback is called and contains
    // the word being edited
    
    CxString _replacementString;
    // set by a callback to replace the string in completionString
    
};


#endif

