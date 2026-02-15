//-------------------------------------------------------------------------------------------------
//
//  tokenizer.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  tokenizer.cpp
//
//-------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <iostream>
#include <fcntl.h>

#include <cx/base/string.h>
#include <cx/base/slist.h>
#include <cx/base/tokenizer.h>


//-------------------------------------------------------------------------------------------------
// CxTokenizer::<constructor>
//-------------------------------------------------------------------------------------------------
CxTokenizer::CxTokenizer( void )
{
}


//-------------------------------------------------------------------------------------------------
// CxTokenizer::isCharCommonSymbol
//
// returns true if the char is a common symbol on the keyboard
//
//-------------------------------------------------------------------------------------------------
/*static*/
int CxTokenizer::isCharCommonSymbol( char ch )
{
    CxString symbol = "~!@#$%^&*()_-+={}[]|\"':;<,>.?/";
    return( symbol.charInString( ch ));
}


//-------------------------------------------------------------------------------------------------
// CxTokenizer::isCharAlphaLower
//
// returns true if the character is a lower case alpha characters
//
//-------------------------------------------------------------------------------------------------
/*static*/
int CxTokenizer::isCharAlphaLower( char ch )
{
	CxString alphaLower = "abcdefghijklmnopqrstuvwxyz";
	return ( alphaLower.charInString( ch ));
}


//-------------------------------------------------------------------------------------------------
// CxTokenizer::isCharAlphaUpper
//
// returns true if the character is a upper case alpha characters
//
//-------------------------------------------------------------------------------------------------
/*static*/
int CxTokenizer::isCharAlphaUpper( char ch )
{
	CxString alphaUpper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	return( alphaUpper.charInString( ch ));
}


//-------------------------------------------------------------------------------------------------
// CxTokenizer::isCharAlpha
//
// returns true if the character is either an upper or lower case alpha character
//
//-------------------------------------------------------------------------------------------------
/*static*/
int CxTokenizer::isCharAlpha( char ch )
{
	if (isCharAlphaUpper( ch )) {
		return( TRUE );
	}
	if (isCharAlphaLower( ch )) {
		return( TRUE );
	}
	return(FALSE);
}


//-------------------------------------------------------------------------------------------------
// CxTokenizer::isCharDigit
//
// returns true if the character is a digit
//
//-------------------------------------------------------------------------------------------------
/*static*/
int CxTokenizer::isCharDigit( char ch )
{
	CxString digitString = "0123456789";
	return( digitString.charInString( ch ) );
}


//-------------------------------------------------------------------------------------------------
// CxTokenizer::isCharOctalDigit
//
// returns true if the character is an octal digit
//
//-------------------------------------------------------------------------------------------------
/*static*/
int CxTokenizer::isCharOctalDigit( char ch )
{
	CxString octalString      = "01234567";
	return( octalString.charInString( ch ) );
}


//-------------------------------------------------------------------------------------------------
// CxTokenizer::isCharHexDigit
//
// returns true if the character is a hex digit
//
//-------------------------------------------------------------------------------------------------
/*static*/
int CxTokenizer::isCharHexDigit( char ch )
{
	CxString hexString = "0123456789ABCDEFabcdef";
	return( hexString.charInString( ch ) );
}


//-------------------------------------------------------------------------------------------------
// CxTokenizer::isCharKnownEscape
//
// returns true if the character is a valid escape characters in C
//
//-------------------------------------------------------------------------------------------------
/*static*/
int CxTokenizer::isCharKnownEscape( char ch )
{
    if (ch == 'n') {
        return( '\n' );
    }

    if (ch == 'r') {
        return( '\r' );
    }

    return( 0 );
}


//-------------------------------------------------------------------------------------------------
// CxTokenizer::octal_string_to_unsigned_int
//
// returns the value of an encoded octal string
//
//-------------------------------------------------------------------------------------------------
/*static*/
unsigned int
CxTokenizer::octal_string_to_unsigned_int( CxString octString )
{
    unsigned int value;
    CxString formatString = "%o";
    sscanf((const char *) octString.data(), (const char *) formatString.data(), &value);
    return(value);
}


//-------------------------------------------------------------------------------------------------
// CxTokenizer::tokenClassify
//
// returns an enum type value for the token classification
//
//-------------------------------------------------------------------------------------------------
/*static*/
CxTokenizer::TokenType
CxTokenizer::tokenClassify( char ch )
{
	if (ch == ' ')           return(TOKEN_SPACE);
	if (ch == '\t')          return(TOKEN_TAB);
	if (ch == '\n')          return(TOKEN_NEWLINE);
	if (ch == '\r')          return(TOKEN_CR);
	if (ch == '\\')          return(TOKEN_SLASH);
	if (ch == '"')           return(TOKEN_QUOTE);
	if (isCharDigit( ch ))   return(TOKEN_DIGIT);
	if (isCharAlpha( ch ))   return(TOKEN_ALPHA);
    if (isCharCommonSymbol( ch )) return( TOKEN_COMMON_SYMBOL );
	return( TOKEN_UNKNOWN );
}


//-------------------------------------------------------------------------------------------------
// CxTokenizer::tokenClassifyString
//
// returns a string that represents the token type
//
//-------------------------------------------------------------------------------------------------
/*static*/
CxString
CxTokenizer::tokenClassifyString( TokenType tt )
{
	switch( tt ) {

		case TOKEN_UNKNOWN : return("TOKEN_UNKNOWN");
		case TOKEN_SPACE   : return("TOKEN_SPACE");
		case TOKEN_TAB     : return("TOKEN_TAB");
		case TOKEN_NEWLINE : return("TOKEN_NEWLINE");
		case TOKEN_CR      : return("TOKEN_CR");
		case TOKEN_SLASH   : return("TOKEN_SLASH");
		case TOKEN_QUOTE   : return("TOKEN_QUOTE");
		case TOKEN_DIGIT   : return("TOKEN_DIGIT");
		case TOKEN_ALPHA   : return("TOKEN_ALPHA");
        case TOKEN_COMMON_SYMBOL: return("TOKEN_COMMON_SYMBOL");
	}
	return("");
}


//-------------------------------------------------------------------------------------------------
// MACRO's to make State Machine code mor readable
//
//-------------------------------------------------------------------------------------------------

#define CHANGE_STATE(x) state = x
#define ERROR_OUT goto ERROR_DONE

//-------------------------------------------------------------------------------------------------
// CxTokenizer::getTokenList
//
//
//-------------------------------------------------------------------------------------------------
CxSList<CxString>
CxTokenizer::getTokenList( void )
{
    return( tokenList );
}


//-------------------------------------------------------------------------------------------------
// CxTokenizer::tokenize
//
// return a list of strings based on space delimiting
//
//
//-------------------------------------------------------------------------------------------------
CxTokenizer::Result
CxTokenizer::tokenize( CxString s )
{
	TokenState state = IN_INITIAL;

	CxString currentToken;
    CxString firstOctalDigit  = "";
    CxString secondOctalDigit = "";
    CxString thirdOctalDigit  = "";

	for (int pos=0; pos<s.length(); pos++) {

		char ch = s.charAt(pos);

		TokenType tt = tokenClassify( ch );

//		printf("char[%c] : %s\n", ch, tokenClassifyString( tt ).data() );

        //-----------------------------------------------------------------------------------------
        // STATE MACHINE
        //
        //-----------------------------------------------------------------------------------------
		switch( state ) {

            //-------------------------------------------------------------------------------------
            // STATE: INITIAL
            //
            //-------------------------------------------------------------------------------------
			case IN_INITIAL:
			{
				switch( tt ) {

 					case TOKEN_UNKNOWN:
					{
                        printf("ERROR");
						ERROR_OUT;
					}
					break;

					case TOKEN_SPACE  :
					{
                        CHANGE_STATE( IN_INITIAL );
					}
					break;

					case TOKEN_TAB    :
					{
                        CHANGE_STATE( IN_INITIAL );
					}
					break;

					case TOKEN_NEWLINE:
					{
                        CHANGE_STATE( IN_INITIAL );
					}
					break;

					case TOKEN_CR     :
					{
						CHANGE_STATE( IN_INITIAL );
					}
					break;

					case TOKEN_SLASH  :
					{
                        CHANGE_STATE( IN_UNQUOTED_TOKEN );
					}
					break;

					case TOKEN_QUOTE  :
					{
                        CHANGE_STATE( IN_QUOTED_TOKEN );
					}
					break;

					case TOKEN_DIGIT  :
					{
                        currentToken.append( ch );
                        CHANGE_STATE( IN_UNQUOTED_TOKEN );
					}
					break;

					case TOKEN_ALPHA  :
					{
                        currentToken.append( ch );
                        CHANGE_STATE( IN_UNQUOTED_TOKEN );
					}
					break;

                    case TOKEN_COMMON_SYMBOL:
                    {
                        currentToken.append( ch );
                        CHANGE_STATE( IN_UNQUOTED_TOKEN );
                    }
                    break;

				}
			}
			break;

            //-------------------------------------------------------------------------------------
            // STATE: IN DELIMITER UNQUOTED
            //
            //-------------------------------------------------------------------------------------
            case IN_DELIMITOR_UNQUOTED:
            {
                switch( tt ) {

                    case TOKEN_UNKNOWN:
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_SPACE  :
                    {
                        CHANGE_STATE( IN_DELIMITOR_UNQUOTED );
                    }
                    break;

                    case TOKEN_TAB    :
                    {
                        CHANGE_STATE( IN_DELIMITOR_UNQUOTED );
                    }
                    break;

                    case TOKEN_NEWLINE:
                    {
                        CHANGE_STATE( IN_DELIMITOR_UNQUOTED );
                    }
                    break;

                    case TOKEN_CR     :
                    {
                        CHANGE_STATE( IN_DELIMITOR_UNQUOTED );
                    }
                    break;

                    case TOKEN_SLASH  :
                    {
                        CHANGE_STATE( IN_ESCAPE_UNQUOTED );
                    }
                    break;

                    case TOKEN_QUOTE  :
                    {
                        CHANGE_STATE( IN_QUOTED_TOKEN );
                    }
                    break;

                    case TOKEN_DIGIT  :
                    {
                        currentToken.append( ch );
                        CHANGE_STATE( IN_UNQUOTED_TOKEN );
                    }
                    break;

                    case TOKEN_ALPHA  :
                    {
                        currentToken.append( ch );
                        CHANGE_STATE( IN_UNQUOTED_TOKEN );
                    }
                    break;

                    case TOKEN_COMMON_SYMBOL:
                    {
                        currentToken.append( ch );
                        CHANGE_STATE( IN_UNQUOTED_TOKEN );
                    }
                    break;
                }
            }
            break;


            //-------------------------------------------------------------------------------------
            // STATE: IN UNQUOTED TOKEN
            //
            //-------------------------------------------------------------------------------------
			case IN_UNQUOTED_TOKEN:
			{
				switch( tt ) {

					case TOKEN_UNKNOWN:
					{
                        printf("ERROR");
                        ERROR_OUT;
					}
					break;

					case TOKEN_SPACE  :
					{
                        // end of token
                        tokenList.append( currentToken );
                        currentToken = "";
                        CHANGE_STATE( IN_DELIMITOR_UNQUOTED );
					}
					break;

					case TOKEN_TAB    :
					{
                        tokenList.append( currentToken );
                        currentToken = "";
                        CHANGE_STATE( IN_DELIMITOR_UNQUOTED );
					}
					break;

					case TOKEN_NEWLINE:
					{
                        tokenList.append( currentToken );
                        currentToken = "";
                        CHANGE_STATE( IN_DELIMITOR_UNQUOTED );
					}
					break;

					case TOKEN_CR     :
					{
                        tokenList.append( currentToken );
                        currentToken = "";
                        CHANGE_STATE( IN_DELIMITOR_UNQUOTED );
					}
					break;

					case TOKEN_SLASH  :
					{
                        CHANGE_STATE( IN_ESCAPE_UNQUOTED );
					}
					break;

					case TOKEN_QUOTE  :
					{
                        printf("ERROR");
						ERROR_OUT;
					}
					break;

					case TOKEN_DIGIT  :
					{
                        currentToken.append( ch );
                        CHANGE_STATE( IN_UNQUOTED_TOKEN );
					}
					break;

					case TOKEN_ALPHA  :
					{
                        currentToken.append( ch );
                        CHANGE_STATE( IN_UNQUOTED_TOKEN );
					}
					break;

                    case TOKEN_COMMON_SYMBOL:
                    {
                        currentToken.append( ch );
                        CHANGE_STATE( IN_UNQUOTED_TOKEN );
                    }
                    break;
				}
			}
			break;


            //-------------------------------------------------------------------------------------
            // STATE: IN ESCAPE UNQUOTED SEQUENCE
            //
            //-------------------------------------------------------------------------------------
            case IN_ESCAPE_UNQUOTED:
            {
                switch( tt ) {

                    case TOKEN_UNKNOWN:
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_SPACE  :
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_TAB    :
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_NEWLINE:
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_CR     :
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_SLASH  :
                    {
                        currentToken.append( ch );
                        CHANGE_STATE( IN_UNQUOTED_TOKEN );
                    }
                    break;

                    case TOKEN_QUOTE  :
                    {
                        currentToken.append( ch );
                        CHANGE_STATE ( IN_QUOTED_TOKEN );
                    }
                    break;

                    case TOKEN_DIGIT  :
                    {
                        if (isCharOctalDigit(ch)) {
                            firstOctalDigit = CxString(ch);
                            CHANGE_STATE( IN_ESCAPE_QUOTED_FIRST_CHAR );
                        } else {
                            printf("ERROR");
							ERROR_OUT;
                        }
                    }
                    break;

                    case TOKEN_ALPHA  :
                    {
                        char cc = isCharKnownEscape( ch );
                        if (cc == (char) NULL) {
                            currentToken.append(cc);
                            CHANGE_STATE( IN_QUOTED_TOKEN );
                        } else {
                            printf("ERROR");
							ERROR_OUT;
                        }
                    }
                    break;

                    case TOKEN_COMMON_SYMBOL:
                    {
                        currentToken.append( ch );
                        CHANGE_STATE( IN_QUOTED_TOKEN );
                    }
                    break;
                }
            }
            break;


            //-------------------------------------------------------------------------------------
            // STATE: IN ESCAPE UNQUOTED FIRST CHAR
            //
            //-------------------------------------------------------------------------------------
            case IN_ESCAPE_UNQUOTED_FIRST_CHAR:
            {
                switch( tt ) {

                    case TOKEN_UNKNOWN:
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_SPACE  :
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_TAB    :
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_NEWLINE:
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_CR :
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_SLASH  :
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_QUOTE  :
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_DIGIT  :
                    {
                        if (isCharOctalDigit(ch)) {
                            secondOctalDigit = CxString( ch );
                            CHANGE_STATE( IN_ESCAPE_UNQUOTED_SECOND_CHAR );
                        } else {
                            printf("ERROR");
							ERROR_OUT;
                        }
                    }
                    break;

                    case TOKEN_ALPHA  :
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_COMMON_SYMBOL:
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;
                }
            }
            break;


            //-------------------------------------------------------------------------------------
            // STATE: IN ESCAPE UNQUOTED_SECOND CHAR
            //
            //-------------------------------------------------------------------------------------
            case IN_ESCAPE_UNQUOTED_SECOND_CHAR:
            {
                switch( tt ) {

                    case TOKEN_UNKNOWN:
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_SPACE  :
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_TAB    :
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_NEWLINE:
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_CR     :
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_SLASH  :
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_QUOTE  :
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_DIGIT  :
                    {
                        if (isCharOctalDigit(ch)) {

                            thirdOctalDigit = CxString( ch );
                            CxString octString = firstOctalDigit + secondOctalDigit + thirdOctalDigit;
                            unsigned int value = octal_string_to_unsigned_int( octString );
                            char cc = (char) value;
                            currentToken.append( cc );
                            CHANGE_STATE( IN_UNQUOTED_TOKEN );

                        } else {
                            printf("ERROR");
							ERROR_OUT;
                        }
                    }
                    break;

                    case TOKEN_ALPHA  :
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_COMMON_SYMBOL:
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;
                }
            }
            break;


            //-------------------------------------------------------------------------------------
            // STATE: IN DELIMITER QUOTED
            //
            //-------------------------------------------------------------------------------------
            case IN_DELIMITOR_QUOTED:
            {
                switch( tt ) {

                    case TOKEN_UNKNOWN:
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_SPACE  :
                    {
                        currentToken.append( ch );
                        CHANGE_STATE( IN_DELIMITOR_QUOTED );
                    }
                    break;

                    case TOKEN_TAB    :
                    {
                        currentToken.append( ch );
                        CHANGE_STATE( IN_DELIMITOR_QUOTED );
                    }
                    break;

                    case TOKEN_NEWLINE:
                    {
                        currentToken.append( ch );
                        CHANGE_STATE( IN_DELIMITOR_QUOTED );
                    }
                    break;

                    case TOKEN_CR     :
                    {
                        currentToken.append( ch );
                        CHANGE_STATE( IN_DELIMITOR_QUOTED );
                    }
                    break;

                    case TOKEN_SLASH  :
                    {
                        CHANGE_STATE( IN_ESCAPE_QUOTED );
                    }
                    break;

                    case TOKEN_QUOTE  :
                    {
                        tokenList.append( currentToken );
                        currentToken = "";
                        CHANGE_STATE( IN_DELIMITOR_UNQUOTED );
                    }
                    break;

                    case TOKEN_DIGIT  :
                    {
                        currentToken.append( ch );
                        CHANGE_STATE( IN_QUOTED_TOKEN );
                    }
                    break;

                    case TOKEN_ALPHA  :
                    {
                        currentToken.append( ch );
                        CHANGE_STATE( IN_QUOTED_TOKEN );
                    }
                    break;

                    case TOKEN_COMMON_SYMBOL:
                    {
                        currentToken.append( ch );
                        CHANGE_STATE( IN_QUOTED_TOKEN );
                    }
                    break;
                }
            }

            //-------------------------------------------------------------------------------------
            // STATE: QUOTED TOKEN
            //
            //-------------------------------------------------------------------------------------
			case IN_QUOTED_TOKEN:
			{
				switch( tt ) {

                    case TOKEN_UNKNOWN:
                    {
                        printf("ERROR TOKEN UNKNOWN");
						ERROR_OUT;
                    }
                    break;

					case TOKEN_SPACE  :
					{
                        currentToken.append( ch );
                        CHANGE_STATE( IN_QUOTED_TOKEN );
					}
					break;

					case TOKEN_TAB    :
					{
                        currentToken.append( ch );
                        CHANGE_STATE( IN_QUOTED_TOKEN );
					}
					break;

					case TOKEN_NEWLINE:
					{
                        currentToken.append( ch );
                        CHANGE_STATE( IN_QUOTED_TOKEN );
					}
					break;

					case TOKEN_CR     :
					{
                        currentToken.append( ch );
                        CHANGE_STATE( IN_QUOTED_TOKEN ;)
					}
					break;

					case TOKEN_SLASH  :
					{
                        CHANGE_STATE( IN_ESCAPE_QUOTED );
					}
					break;

					case TOKEN_QUOTE  :
					{
                        tokenList.append( currentToken );
                        currentToken = "";
                        CHANGE_STATE( IN_DELIMITOR_UNQUOTED );
					}
					break;

					case TOKEN_DIGIT  :
					{
                        currentToken.append( ch );
                        CHANGE_STATE( IN_QUOTED_TOKEN );
					}
					break;

					case TOKEN_ALPHA  :
					{
                        currentToken.append( ch );
                        CHANGE_STATE( IN_QUOTED_TOKEN );
					}
					break;

                    case TOKEN_COMMON_SYMBOL:
                    {
                        currentToken.append( ch );
                        CHANGE_STATE( IN_QUOTED_TOKEN );
                    }
                    break;

				}
			}
			break;


            //-------------------------------------------------------------------------------------
            // STATE: IN ESCAPE QUOTED SEQUENCE
            //
            //-------------------------------------------------------------------------------------
            case IN_ESCAPE_QUOTED:
            {
                switch( tt ) {

                    case TOKEN_UNKNOWN:
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_SPACE  :
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_TAB    :
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_NEWLINE:
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_CR     :
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_SLASH  :
                    {
                        currentToken.append( ch );
                        CHANGE_STATE( IN_QUOTED_TOKEN );
                    }
                    break;

                    case TOKEN_QUOTE  :
                    {
                        currentToken.append( ch );
                        CHANGE_STATE ( IN_QUOTED_TOKEN );
                    }
                    break;

                    case TOKEN_DIGIT  :
                    {
                        if (isCharOctalDigit(ch)) {
                            firstOctalDigit = CxString(ch);
                            CHANGE_STATE( IN_ESCAPE_QUOTED_FIRST_CHAR );
                        } else {
                            printf("ERROR");
							ERROR_OUT;
                        }
                    }
                    break;

                    case TOKEN_ALPHA  :
                    {
                        char cc = isCharKnownEscape( ch );
                        if (cc == (char) NULL) {
                            currentToken.append(cc);
                            CHANGE_STATE( IN_QUOTED_TOKEN );
                        } else {
                            printf("ERROR");
							ERROR_OUT;
                        }
                    }
                    break;

                    case TOKEN_COMMON_SYMBOL:
                    {
                        currentToken.append( ch );
                        CHANGE_STATE( IN_QUOTED_TOKEN );
                    }
                    break;

                }
            }
            break;


            //-------------------------------------------------------------------------------------
            // STATE: IN ESCAPE UNQUOTED FIRST CHAR
            //
            //-------------------------------------------------------------------------------------
            case IN_ESCAPE_QUOTED_FIRST_CHAR:
            {
                switch( tt ) {

                    case TOKEN_UNKNOWN:
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_SPACE  :
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_TAB    :
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_NEWLINE:
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_CR :
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_SLASH  :
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_QUOTE  :
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_DIGIT  :
                    {
                        if (isCharOctalDigit(ch)) {
                            secondOctalDigit = CxString( ch );
                            CHANGE_STATE( IN_ESCAPE_QUOTED_SECOND_CHAR );
                        } else {
                            printf("ERROR");
							ERROR_OUT;
                        }
                    }
                    break;

                    case TOKEN_ALPHA  :
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_COMMON_SYMBOL:
                    {
                        currentToken.append( ch );
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                }
            }
            break;


            //-------------------------------------------------------------------------------------
            // STATE: IN ESCAPE UNQUOTED_SECOND CHAR
            //
            //-------------------------------------------------------------------------------------
            case IN_ESCAPE_QUOTED_SECOND_CHAR:
            {
                switch( tt ) {

                    case TOKEN_UNKNOWN:
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_SPACE  :
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_TAB    :
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_NEWLINE:
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_CR     :
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_SLASH  :
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_QUOTE  :
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_DIGIT  :
                    {
                        if (isCharOctalDigit(ch)) {

                            thirdOctalDigit = CxString( ch );
                            CxString octString = firstOctalDigit + secondOctalDigit + thirdOctalDigit;
                            unsigned int value = octal_string_to_unsigned_int( octString );
                            char cc = (char) value;
                            currentToken.append( cc );

                            CHANGE_STATE( IN_QUOTED_TOKEN );
                        } else {
                            printf("ERROR");
							ERROR_OUT;
                        }
                    }
                    break;

                    case TOKEN_ALPHA  :
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                    case TOKEN_COMMON_SYMBOL:
                    {
                        printf("ERROR");
						ERROR_OUT;
                    }
                    break;

                }
            }
            break;
		}
	}


    if (currentToken.length() > 0 ) {
        tokenList.append( currentToken );
    }

    switch( state ) {

        case IN_INITIAL:
            return( OK);

        case IN_DELIMITOR_UNQUOTED:
            return( OK);

        case IN_UNQUOTED_TOKEN:
            return( OK);

        // it's an error to end in an incomplete escape seqence
        case IN_ESCAPE_UNQUOTED:
            return( SYNTAX_ERROR);

        case IN_ESCAPE_UNQUOTED_FIRST_CHAR:
            return( SYNTAX_ERROR);

        case IN_ESCAPE_UNQUOTED_SECOND_CHAR:
            return( SYNTAX_ERROR);

        // its an error to end in a quoted token without an end quote
        case IN_DELIMITOR_QUOTED:
            return( SYNTAX_ERROR);

        case IN_QUOTED_TOKEN:
            return( SYNTAX_ERROR);

        // it's an error to end in an incomplete escape seqence
        case IN_ESCAPE_QUOTED:
            return( SYNTAX_ERROR);

        case IN_ESCAPE_QUOTED_FIRST_CHAR:
            return( SYNTAX_ERROR);

        case IN_ESCAPE_QUOTED_SECOND_CHAR:
            return( SYNTAX_ERROR);
    }

    // should not get here
    return( OK );

ERROR_DONE:

    if (currentToken.length() >0 ) {
        tokenList.append( currentToken );
    }

    for (int c=0; c<tokenList.entries(); c++) {
        printf("token=[%s]\n", tokenList.at(c).data());
    }

    return( SYNTAX_ERROR );

}
