//-------------------------------------------------------------------------------------------------
//
//  token.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  token.h
//
//-------------------------------------------------------------------------------------------------

#ifndef __token_h__
#define __token_h__

#include <cx/base/string.h>
#include <cx/base/slist.h>

//-------------------------------------------------------------------------------------------------
// token: order of operations					
//-------------------------------------------------------------------------------------------------
//								
//	  ()						:Level6 (paren expression)	
//	  -(type)					:Level5 (unary)			
//								:Level4 (not used exponents)	
//	  *	   /					:Level3				
//	  +	   -					:Level2				
//	  <<  >>					:(shift unused)		
//	  <	  <=   >   >=  ==  !=	:(relational unused)	
//	  &							:(bit and)		
//	  ^							:(bit xor)		
//	  |							:(bit or)		
//	  &&						:(logical and, unused)	
//	  ||						:(logical or, unused)	
//	  ?							:(question, unused) 
//	  =	  +=   -=  *=  /=		:(assignment, unused)	
//	  ,							:(comma)			
//								
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// token: public/defines (token types)				
//-------------------------------------------------------------------------------------------------

/*
#define PLUS_SIGN			 	1
#define MINUS_SIGN				2
#define MULT_SIGN				3
#define DIV_SIGN				4
#define EXP_SIGN				5
#define MOD_SIGN				6

// new 
#define LT_SIGN					7
#define GT_SIGN					8
#define LE_SIGN					9
#define GE_SIGN					10

#define EE_SIGN					11
#define NE_SIGN					12
#define LA_SIGN					13
#define LO_SIGN					14


#define COMMA					18

#define LEFT_PAREN			 	20
#define RIGHT_PAREN			 	21

#define DOUBLE_NUMBER		 	30

#define VARIABLE			 	40
#define FUNCTION				50

#define UNKNOWN_VARIABLE		80
#define UNKNOWN_FUNCTION		81

#define UNKNOWN_SYMBOL			82
#define PREMATURE_EOL		 	83
#define ILLEGAL_NUMBER			84
#define UNBALANCED_PARENS		85

#define END					 	86
*/

class CxExpressionToken
{
  public:
    
    enum expressionTokenType {
        NONE,
        PLUS_SIGN,
        MINUS_SIGN,
        MULT_SIGN,
        DIV_SIGN,
        EXP_SIGN,
        MOD_SIGN,
//        LT_SIGN,
//        GT_SIGN,
//        LE_SIGN,
//        GE_SIGN,
//       EE_SIGN,
//        NE_SIGN,
//        LA_SIGN,
//        LO_SIGN,
        COMMA,
        LEFT_PAREN,
        RIGHT_PAREN,
        DOUBLE_NUMBER,
        VARIABLE,
        FUNCTION,
        UNKNOWN_VARIABLE,
        UNKNOWN_FUNCTION,
        UNKNOWN_SYMBOL,
        PREMATURE_EOL ,
        ILLEGAL_NUMBER ,
        UNBALANCED_PARENS ,
        END
    };
  
    
		CxExpressionToken( void );
		// constructor

		CxExpressionToken(expressionTokenType ttype_ , CxString text_ , double value_ );
		// constructor

		CxExpressionToken( const CxExpressionToken& et_);
		// copy constructor
    
        CxExpressionToken& operator=( const CxExpressionToken& et_);
        // assignment operator

		int operator==( const CxExpressionToken& et_ ) const;
    	// operator equal

		int operator!=( const CxExpressionToken& et_ ) const;
    	// operator not equal


		static int	 isparen(char ch);
		static int	 isleftparen(char ch);
		static int	 isperiod(char ch);
		static int	 not_one_of(char ch, const char *group);
		static int	 isexponent(char ch);
		static int	 is_one_of(char ch, const char *group);
		static int	 is_in(char c,const char *s);
		static int	 iswhite(char c);
		static int	 isvarfunc(char c);
		static int	 isnull(char c);
	
		int	 Dump(void);
		CxString  asString(void);

//	private:
	
		expressionTokenType ttype;
		CxString text;
		double	 value;
		//int		 key;

};


//-------------------------------------------------------------------------------------------------
// token: public/types						
//-------------------------------------------------------------------------------------------------
/*
typedef struct {
	
	unsigned int	type;
	char			*text;
	double			value;
	int		key;

} token_struct;
*/

//-------------------------------------------------------------------------------------------------
// token: public/forwards					
//-------------------------------------------------------------------------------------------------
/*
token_struct **TOKEN_InitList(void);
token_struct **TOKEN_AddToList(token_struct *tlist[], unsigned int type, 
			   char *text, double value, int key);

void		   TOKEN_FreeList(token_struct *tlist[]);


int	 isparen(char ch);
int	 isleftparen(char ch);
int	 isperiod(char ch);
int	 not_one_of(char ch, char *group);
int	 isexponent(char ch);
int	 is_one_of(char ch, char *group);
int	 is_in(char c, const char *s);
int	 iswhite(char c);
int	 isvarfunc(char c);
int	 isnull(char c);
int	 TOKEN_Dump(token_struct *token);
void TOKEN_Strcat(token_struct *token, char *s);
*/

#endif
