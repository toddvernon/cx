//-------------------------------------------------------------------------------------------------
//
//  tokenizer.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  tokenizer.h
//
//-------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <iostream>
#include <fcntl.h>

#include <cx/base/string.h>
#include <cx/base/slist.h>

#ifndef _CxTokenizer_h_
#define _CxTokenizer_h_

class CxTokenizer
{
  public:

    enum TokenState {
        
        IN_INITIAL,
        
        IN_DELIMITOR_UNQUOTED,
        IN_UNQUOTED_TOKEN,
        IN_ESCAPE_UNQUOTED,
        IN_ESCAPE_UNQUOTED_FIRST_CHAR,
        IN_ESCAPE_UNQUOTED_SECOND_CHAR,
        
        IN_DELIMITOR_QUOTED,
        IN_QUOTED_TOKEN,
        IN_ESCAPE_QUOTED,
        IN_ESCAPE_QUOTED_FIRST_CHAR,
        IN_ESCAPE_QUOTED_SECOND_CHAR,
    };

    enum TokenType {
        TOKEN_UNKNOWN,
        TOKEN_SPACE,
        TOKEN_TAB,
        TOKEN_NEWLINE,
        TOKEN_CR,
        TOKEN_ALPHA,
        TOKEN_DIGIT,
        TOKEN_SLASH,
        TOKEN_QUOTE,
        TOKEN_COMMON_SYMBOL
    };
    
    enum Result {
        OK,
        SYNTAX_ERROR
    };

    CxTokenizer( void );
    // constructor

    Result tokenize( CxString s );
    // process tokens
    
    CxSList<CxString> getTokenList( void );
    // return token list
    
  private:
    
    static int isCharCommonSymbol( char ch );

    static int isCharAlphaLower( char ch );
    
    static int isCharAlphaUpper( char ch );
    
    static int isCharAlpha( char ch );

    static int isCharDigit( char ch );
    
    static int isCharOctalDigit( char ch );

    static int isCharHexDigit( char ch );

    static int isCharKnownEscape( char ch );
    
    static unsigned int octal_string_to_unsigned_int( CxString octString );

    static TokenType tokenClassify( char ch );

    static CxString tokenClassifyString( TokenType tt );

    CxSList< CxString > tokenList;
    
};


#endif
