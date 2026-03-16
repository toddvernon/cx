//-------------------------------------------------------------------------------------------------
//
//  keyaction.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxKeyAction Class
//
//-------------------------------------------------------------------------------------------------

#include <cx/keyboard/keyaction.h>
    
//-------------------------------------------------------------------------
// CxKeyAction:: (constructor)
//
//-------------------------------------------------------------------------
CxKeyAction::CxKeyAction( CxString definition )
: _mouseButton(0)
, _mouseRow(0)
, _mouseCol(0)
, _mouseModifiers(0)
{
	_actionType = CxKeyAction::UNKNOWN;
	_definition = definition;
        
 	//printf("definition=[%s]\n", definition.data());
        
	CxString left  = definition.nextToken(":");
	_tag   = definition;
	_tag.remove(0,1);
    
    //printf("left=%s; tag=%s\n", left.data(), _tag.data() );
        
    // printf("left=[%s]\n", left.data());
        
  	if (left=="BACKSPACE") {
   		_actionType = CxKeyAction::BACKSPACE;
 	}
        
 	if (left=="NEWLINE") {
    	_actionType = CxKeyAction::NEWLINE;
   	}
        
  	if (left=="TAB") {
    	_actionType = CxKeyAction::TAB;
    }
        
	if (left=="CONTROL") {
    	_actionType = CxKeyAction::CONTROL;
	}
        
 	if (left=="SYMBOL") {
    	_actionType = CxKeyAction::SYMBOL;
 	}
        
 	if (left=="NUMBER") {
    	_actionType = CxKeyAction::NUMBER;
  	}
        
 	if (left=="UPPERCASE-ALPHA") {
    	_actionType = CxKeyAction::UPPERCASE_ALPHA;
  	}
        
  	if (left=="LOWERCASE-ALPHA") {
    	_actionType = CxKeyAction::LOWERCASE_ALPHA;
   	}
        
	if (left=="OPTION") {
    	_actionType = CxKeyAction::OPTION;
 	}
        
	if (left=="SHIFT-OPTION") {
    	_actionType = CxKeyAction::SHIFT_OPTION;
   	}
        
 	if (left=="CURSOR"){
   		_actionType = CxKeyAction::CURSOR;
	}
        
 	if (left=="SHIFT-CURSOR") {
    	_actionType = CxKeyAction::SHIFT_CURSOR;
   	}

 	if (left=="CTRL-CURSOR") {
    	_actionType = CxKeyAction::CTRL_CURSOR;
   	}
    
    if (left=="COMMAND") {
        _actionType = CxKeyAction::COMMAND;
    }
}


//-------------------------------------------------------------------------
// CxKeyAction:: (mouse constructor)
//
// Create a mouse action directly with parsed data.
//-------------------------------------------------------------------------
CxKeyAction::CxKeyAction( aType type, int button, int row, int col, int modifiers )
: _actionType(type)
, _mouseButton(button)
, _mouseRow(row)
, _mouseCol(col)
, _mouseModifiers(modifiers)
{
}


//-------------------------------------------------------------------------
// CxKeyAction:: (copy constructor)
//
//-------------------------------------------------------------------------
CxKeyAction::CxKeyAction( const CxKeyAction& a_)
: _mouseButton(0)
, _mouseRow(0)
, _mouseCol(0)
, _mouseModifiers(0)
{
	if (&a_ != this ) {
    	_actionType     = a_._actionType;
       	_tag            = a_._tag;
        _definition     = a_._definition;
        _mouseButton    = a_._mouseButton;
        _mouseRow       = a_._mouseRow;
        _mouseCol       = a_._mouseCol;
        _mouseModifiers = a_._mouseModifiers;
    }
}


//-------------------------------------------------------------------------
// CxKeyAction:: (assignment operator)
//
//-------------------------------------------------------------------------
CxKeyAction&
CxKeyAction::operator=( const CxKeyAction& a_ )
{
	if ( &a_ != this ) {
    	_actionType     = a_._actionType;
     	_tag            = a_._tag;
     	_definition     = a_._definition;
        _mouseButton    = a_._mouseButton;
        _mouseRow       = a_._mouseRow;
        _mouseCol       = a_._mouseCol;
        _mouseModifiers = a_._mouseModifiers;
  	}
    return( *this );
}


//-------------------------------------------------------------------------
// CxKeyAction::actionType
//
//-------------------------------------------------------------------------
int
CxKeyAction::actionType(void)
{
	return( _actionType );
}


//-------------------------------------------------------------------------
// CxKeyAction::tag
//
//-------------------------------------------------------------------------
CxString
CxKeyAction::tag(void)
{
	return(_tag);
}

//-------------------------------------------------------------------------
// CxKeyAction::definition
//
//-------------------------------------------------------------------------
CxString
CxKeyAction::definition(void)
{
	return(_definition);
}


//-------------------------------------------------------------------------
// CxKeyAction::mouseButton
//
// Returns the mouse button: 1=left, 2=middle, 3=right, 4=wheel-up, 5=wheel-down
//-------------------------------------------------------------------------
int
CxKeyAction::mouseButton(void)
{
    return(_mouseButton);
}


//-------------------------------------------------------------------------
// CxKeyAction::mouseRow
//
// Returns the terminal row (0-indexed)
//-------------------------------------------------------------------------
int
CxKeyAction::mouseRow(void)
{
    return(_mouseRow);
}


//-------------------------------------------------------------------------
// CxKeyAction::mouseCol
//
// Returns the terminal column (0-indexed)
//-------------------------------------------------------------------------
int
CxKeyAction::mouseCol(void)
{
    return(_mouseCol);
}


//-------------------------------------------------------------------------
// CxKeyAction::mouseModifiers
//
// Returns modifier bit flags: 4=shift, 8=meta, 16=ctrl
//-------------------------------------------------------------------------
int
CxKeyAction::mouseModifiers(void)
{
    return(_mouseModifiers);
}


//-------------------------------------------------------------------------
// CxKeyAction::mouseShift
//
// Returns 1 if shift held, 0 otherwise
//-------------------------------------------------------------------------
int
CxKeyAction::mouseShift(void)
{
    return((_mouseModifiers & 4) ? 1 : 0);
}


//-------------------------------------------------------------------------
// CxKeyAction::mouseCtrl
//
// Returns 1 if ctrl held, 0 otherwise
//-------------------------------------------------------------------------
int
CxKeyAction::mouseCtrl(void)
{
    return((_mouseModifiers & 16) ? 1 : 0);
}
