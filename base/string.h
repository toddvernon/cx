/***************************************************************************
 *
 *  string.h
 *
 *  CxString Class
 *
 ***************************************************************************
 *
 * (c) Copyright 1999,2000,2001 T.Vernon
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



//-------------------------------------------------------------------------
// standard includes
//
//-------------------------------------------------------------------------
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <iostream>
#include <strings.h> // for IRIX

//UBUNTU
#include <cstring>
#include <stdarg.h>

#include <sys/types.h>
#include <pwd.h>
//#include <uuid/uuid.h> UBUNTO

//-------------------------------------------------------------------------
// standard includes
//
//-------------------------------------------------------------------------
#if defined(_LINUX_) || defined(_OSX_)
#include <unistd.h>
#endif




#ifndef _CxString_h_
#define _CxString_h_


#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


//-------------------------------------------------------------------------
// class CxString
//
//-------------------------------------------------------------------------
class CxString
{
  public:

	CxString( void );
	// constructor

    CxString(int i);
    // set string from int

    CxString( long l );
    // set string from long

    CxString(unsigned long ul);
    // set string from unsigned long

    CxString(double d);
    // set string from double

	CxString( const CxString& sr_ );
	// copy constructor

	CxString( const char * cptr_, int len=-1 );
	// construct from a const char string

	CxString( const CxString * sr_ );
	// copy from a pointer

    CxString( const char cc_ );
	// construct from a single char

	~CxString( void );
	// destructor

	CxString& operator=( const CxString& sr_ );
	// assignment from CxString

	CxString operator+( const CxString& sr_ );
	// append contents of a CxString

	CxString& operator+=( const CxString& sr_ );
	// append CxString to self

    int operator==(const CxString& sr_ ) const;
	// compare CxString to self

    int operator!=(const CxString& sr_ ) const;
	// compare CxString to self

    int operator<=(const CxString& sr_) const;
    // compare CxString to self

	void append( const CxString& sr_ );
	// append CxString to self

	void append( const char cc_ );
	// append a char to self

	void insert( const CxString& sr_, int pos );
	// append CxString to self

	void insert( char c, int n);
	// insert a character at n	

	int length( void ) const;
	// return length of self

	int firstChar( const char ) const;
	// return index of first occurance of char, or -1

	int firstChar( const char * delimSet, char *theDelim=NULL) const;
	// return index of first occurance of char, or -1

    int index( CxString s, int startpos_=0 ) const;
	// return index of first occurance of s starting at pos, or -1

	int lastChar( const char ) const;
	// return index of last occurance of char, or -1

    int charAt(int index);
    // return the character at the index

	CxString& stripLeading( const char* charSet_ );
	// strip all leading characters contained in charSet

	CxString& stripTrailing( const char* charSet_ );
	// strip all the trailing characters contained in charSet

	CxString  nextToken( const char* charSet_, char *delim = NULL );
	// return the next token as delimited by charSet, removing from self

	CxString  subString( int start, int len );
	// return a copy of the substring between start and start+len

	CxString& remove( int start, int len );
	// remove a sub part of self and return
    
    int replaceAll(CxString match, CxString replace);
    // replace all occurances of match with replace
    
    int numberOfOccurances( CxString match );
    // find out how many times a string appears in the string
    
    int toInt( void );
	// convert contents of self to integer and return

    long toLong( void );
	// convert contents of self to long and return

    unsigned long toUnsignedLong( void );
	// convert contents of self to unsigned long and return

    float toFloat( void );
	// convert contents of self to float and return

    double toDouble( void );
	// convert contenst of self to double and return

    static CxString netNormalize( CxString text );
	// remove any trailing CR and/or LF from text and return

    static CxString removeCarriageReturns( CxString line );
    // return a string with all CR's removed from line

    static CxString toUpper( CxString s );
	// convert all characters in s to upper case and return

    static CxString toLower( CxString s );
	// convert all characters in s to lower case and return

	char *data( void ) const;
	// return a pointer to a raw c string

	int isNull( void ) const;
	// if self contains nothing return true

	unsigned int hashValue( void ) const;
	// return a integer hash value of self

	static CxString urlDecode( CxString s_ );
	// return a decoded string	

	int isFloat(void);
	// does the string contain only items that appear in a float

	int isInt(void);
	// does the string contain only items that appear in a float

	void setString( CxString s );
	// set string from string

	void setInt(int i);
 	// set string from int

	void setLong( long l );
  	// set string from long

	void setUnsignedLong(unsigned long ul);
  	// set string from unsigned long

 	void setDouble(double d);
  	// set string from double

    int charInString( char ch );
    // return TRUE if ch is in the string, FALSE otherwise

    int compare( CxString right );

    void printf( const char * format, ... );

	int findAndReplaceFirst( CxString findString, CxString replaceString, int startPos = 0);
	// find the first occarance of find string in self, and replace it with replace string
	// returns -1 if no occurance is found

  private:

	friend std::ostream& operator<<(std::ostream& str, const CxString& cxstring_ );
	// outputs a CxString to an ostream

	static int charInSet( char ch, const char* charSet_ );
	// return true if ch is in charSet

	void reAssign( const char *, int len=-1 );
	// internal assignment of self

	char *_data;
	// internal pointer to data
	
};


#endif

