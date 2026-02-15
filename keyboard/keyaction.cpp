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
    
    if (left=="COMMAND") {
        _actionType = CxKeyAction::COMMAND;
    }
}


//-------------------------------------------------------------------------
// CxKeyAction:: (copy constructor)
//
//-------------------------------------------------------------------------
CxKeyAction::CxKeyAction( const CxKeyAction& a_)
{
	if (&a_ != this ) {
    	_actionType = a_._actionType;
       	_tag        = a_._tag;
        _definition = a_._definition;
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
    	_actionType = a_._actionType;
     	_tag        = a_._tag;
     	_definition = a_._definition;
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
//-------------------------------------------------------------------------/
CxString 
CxKeyAction::definition(void)
{
	return(_definition);
}
