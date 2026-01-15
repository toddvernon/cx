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
