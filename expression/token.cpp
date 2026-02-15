//-------------------------------------------------------------------------------------------------
//
//  token.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  token.cpp
//
//-------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//-------------------------------------------------------------------------------------------------
// token: local/includes					
//-------------------------------------------------------------------------------------------------
#include "token.h"
#include "expression.h"
#include "scanDouble.h"


//-------------------------------------------------------------------------------------------------
// CxExpressionToken::CxExpressionToken (constructor) 
//
//
//-------------------------------------------------------------------------------------------------
CxExpressionToken::CxExpressionToken(void)
{
	ttype = NONE;
	text  = "";
	value = 0.0;
	//key   = 0;	
}


//-------------------------------------------------------------------------------------------------
// CxExpressionToken::CxExpressionToken (constructor) 
//
//
//-------------------------------------------------------------------------------------------------
CxExpressionToken::CxExpressionToken(expressionTokenType ttype_, CxString text_ , double value_)
: ttype( ttype_), text (text_), value(value_)
{
}


//-------------------------------------------------------------------------------------------------
// CxExpressionToken::CxExpressionToken (copy constructor) 
//
//
//-------------------------------------------------------------------------------------------------
CxExpressionToken::CxExpressionToken( const CxExpressionToken& et_)
{
	if ( &et_ != this ) {
		ttype = et_.ttype;
		text  = et_.text;
		value = et_.value;
	}
}

//-------------------------------------------------------------------------------------------------
// CxExpressionToken::CxExpressionToken (assignment constructor)
//
//
//-------------------------------------------------------------------------------------------------
CxExpressionToken&
CxExpressionToken::operator=( const CxExpressionToken& et_)
{
    if ( &et_ != this ) {
        ttype = et_.ttype;
        text  = et_.text;
        value = et_.value;
    }
    return(*this);
}

//-------------------------------------------------------------------------------------------------
// CxExpressionToken::CxExpressionToken (operator ==) 
//
//
//-------------------------------------------------------------------------------------------------
int
CxExpressionToken::operator==( const CxExpressionToken& et_ ) const
{
	if ((et_.ttype == ttype) &&
		(et_.text  == text ) &&
		(et_.value == value) ) {
			return( TRUE );
    }
    
	return( FALSE );
}


//-------------------------------------------------------------------------------------------------
// CxExpressionToken::CxExpressionToken (operator !=) 
//
//
//-------------------------------------------------------------------------------------------------
int
CxExpressionToken::operator!=( const CxExpressionToken& et_ ) const
{
	if ((et_.ttype == ttype) &&
		(et_.text  == text ) &&
		(et_.value == value)) {
			return( FALSE  );
    }
	return( TRUE );
}


//-------------------------------------------------------------------------------------------------
// isparen:							
// Returns true if character is left or right paren.	
//								
//-------------------------------------------------------------------------------------------------
/*static*/
int CxExpressionToken::isparen(char ch)
{
	if ((ch=='(') || (ch == ')')) return(1);
	
	return(0);
}


//-------------------------------------------------------------------------------------------------
// isleftparen:						
// Returns true if character is left paren			
//								
//-------------------------------------------------------------------------------------------------
/*static*/
int CxExpressionToken::isleftparen(char ch)
{
	if (ch=='(') return(1);

	return(0);
}


//-------------------------------------------------------------------------------------------------
// isperiod:							
// Returns true if character is a period.			
//								
//-------------------------------------------------------------------------------------------------
/*static*/
int CxExpressionToken::isperiod(char ch)
{
	if (ch=='.') return(1);

	return(0);
}


//-------------------------------------------------------------------------------------------------
// not_one_of:							
// Returns true if the character is NOT one the characters 
// contained within the string.				
//								
//-------------------------------------------------------------------------------------------------
/*static*/
int CxExpressionToken::not_one_of(char ch, const char *group) 
{
	while (*group != 0) {
        if (*group == ch) return(0);
        group++;
	}

	return(1);
}


//-------------------------------------------------------------------------------------------------
// isexponent:							
// Returns true if character is the letter e or E		
//								
//-------------------------------------------------------------------------------------------------
/*static*/
int CxExpressionToken::isexponent(char ch)
{
	if ((ch=='e') || (ch=='E')) return(1);

	return(0);
}


//-------------------------------------------------------------------------------------------------
// is_one_of:							
// Returns true if character is one of the characters in	
// the string.						
//								
//-------------------------------------------------------------------------------------------------
/*static*/
int CxExpressionToken::is_one_of(char ch, const char *group)
{
	while (*group != 0) {

		if (*group == ch) return(1);
	
		group++;
	}		

	return(0);
}


//-------------------------------------------------------------------------------------------------
// is_in:							
// Returns true if character is in the string.		
//								
//-------------------------------------------------------------------------------------------------
/*static*/
int CxExpressionToken::is_in(char c, const char *s)
{
	while (*s) {
		if (*s++==c) return(1);
	}
	return(0);
}


//-------------------------------------------------------------------------------------------------
// iswhite:							
// Returns true if character is white space		
//								
//-------------------------------------------------------------------------------------------------
/*static*/
int CxExpressionToken::iswhite(char c)
{
	if (c==' '|| c==9) return(1);
	
	return(0);
}


//-------------------------------------------------------------------------------------------------
// isvarfunc:							
// Returns true if character is allowable in a variable	
// or function name.					
//								
//-------------------------------------------------------------------------------------------------
/*static*/
int CxExpressionToken::isvarfunc(char c)
{
//	if (isalnum((int) c) || (c=='_') || (c==':') || (c=='.')) return(1);


	if (isalnum((int) c) || (c=='_') || (c==':') || (c=='$')) return(1);
	return(0);
}


//-------------------------------------------------------------------------------------------------
// isnull:							
// Returns true if character is NULL			
//								
//-------------------------------------------------------------------------------------------------
/*static*/
int CxExpressionToken::isnull(char c)
{
	if (c==0) return(1);
	return(0);
}


//-------------------------------------------------------------------------------------------------
// TOKEN_Dump:							
// Print a debug dump of the token in readable form	
//								
//-------------------------------------------------------------------------------------------------
int CxExpressionToken::Dump(void)
{
	switch(ttype) {
			
		case PLUS_SIGN	   :
			printf("TOKEN(PLUS_SIGN) ");
			break;
			
		case MINUS_SIGN	   :
			printf("TOKEN(MINUS_SIGN) ");
			break;
			
		case MULT_SIGN	   :
			printf("TOKEN(MULT_SIGN) ");
			break;
			
		case DIV_SIGN	  :
			printf("TOKEN(DIV_SIGN) ");
			break;
			
		case EXP_SIGN	  :
			printf("TOKEN(EXP_SIGN) ");
			break;
			
		case MOD_SIGN	  :
			printf("TOKEN(MOD_SIGN) ");
			break;
			
		case LEFT_PAREN	   :
			printf("TOKEN(LEFT_PAREN) ");
			break;
			
		case RIGHT_PAREN   :
			printf("TOKEN(RIGHT_PAREN) ");
			break;
			
		case DOUBLE_NUMBER :
			printf("TOKEN(DOUBLE=%f) ", value);
			break;
			
		case VARIABLE  :
			printf("TOKEN(VAR=%s) ", text.data() );
			break;
			
		case FUNCTION  :
			printf("TOKEN(FUNC=%s) ",text.data() );
			break;
			
		case END :
			printf("TOKEN(END) ");
			break;
			
		case UNKNOWN_FUNCTION :
			printf("TOKEN(UNKNOWN_FUNCTION=%s) ",text.data() );
			break;
			
		case UNKNOWN_VARIABLE :
			printf("TOKEN(UNKNOWN_VARIABLE=%s) ",text.data() );
			break;
			
		case ILLEGAL_NUMBER :
			printf("TOKEN(ILLEGAL_NUMBER) ");
			break;
			
		case UNBALANCED_PARENS :
			printf("TOKEN(UNBALANCED_PARENS) ");
			break;
			
		case UNKNOWN_SYMBOL :
			printf("TOKEN(UNKNOWN_SYMBOL=%s) ", text.data() );
			break;
			
		case COMMA :
			printf("TOKEN(COMMA) ");
			break;
			
		default		 :
			printf("TOKEN(UNKNOWN) ");
	}
	return (0); // guess at return value - GMCC 
}


//-------------------------------------------------------------------------------------------------
// TOKEN_Strcat:						
// Concat a string representation of the token onto the	
// end of the string.					
//								
//-------------------------------------------------------------------------------------------------
CxString CxExpressionToken::asString( void )
{
	char buffer[200];

	switch(ttype) {
			
		case PLUS_SIGN	   :
			return( CxString("+") );
			break;
			
		case MINUS_SIGN	   :
			return( CxString("-") );
			break;
			
		case MULT_SIGN	  :
			return( CxString("*") );
			break;
			
		case DIV_SIGN	   :
			return( CxString("/") );
			break;
			
		case EXP_SIGN	   :
			return( CxString("^") );
			break;
			
		case MOD_SIGN	   :
			return( CxString("%") );
			break;
			
		case LEFT_PAREN	   :
			return( CxString("(") );
			break;
			
		case RIGHT_PAREN  :
			return( CxString(")") );
			break;
			
		case DOUBLE_NUMBER :
			sprintf(buffer, "%f", value);
			return( CxString( buffer ) );
			break;
			
		case VARIABLE	  :
			return( text );
			break;
			
		case FUNCTION  :
			return( text );
			break;
			
		case UNKNOWN_FUNCTION :
			sprintf(buffer, "%s", text.data() );
			return( CxString( buffer ));
			break;
			
		case UNKNOWN_VARIABLE :
			sprintf(buffer, "%s", text.data() );
			return( CxString( buffer ) );
			break;
			
		case ILLEGAL_NUMBER :
			return( CxString("<ILLEGAL_NUMBER>") );
			break;
			
		case UNBALANCED_PARENS :
			return( CxString("<UNBALANCED_PARENS>") );
			break;
			
		case UNKNOWN_SYMBOL :
			sprintf(buffer,"%s", text.data() );
			return( CxString(buffer) );
			break;
			
		case COMMA :
			return( CxString(",") );
			break;
			
		case END :
			return( CxString("") );
			break;

		default:
			return( CxString ("<UNDEF>") );
			break;
	}
		
}
