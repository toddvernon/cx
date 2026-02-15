//-------------------------------------------------------------------------------------------------
//
//  string.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxString Class
//
//-------------------------------------------------------------------------------------------------

#include <cx/base/string.h>


//-------------------------------------------------------------------------
// CxString::CxString
//
//-------------------------------------------------------------------------
CxString::CxString( void )
: _data( NULL )
{
	reAssign( (char *) NULL );
}


//-------------------------------------------------------------------------
// CxString::CxString
//
//-------------------------------------------------------------------------
CxString::CxString(int i)
:_data( NULL )
{
    reAssign( (char *) NULL );
    setInt(i);
}

//-------------------------------------------------------------------------
// CxString::CxString
//
//-------------------------------------------------------------------------
CxString::CxString( long l )
: _data( NULL )
{
    reAssign( (char *) NULL );
    setLong(l);
}

//-------------------------------------------------------------------------
// CxString::CxString
//
//-------------------------------------------------------------------------
CxString::CxString(unsigned long ul)
: _data( NULL )
{
    reAssign( (char *) NULL );
    setUnsignedLong(ul);
}

//-------------------------------------------------------------------------
// CxString::CxString
//
//-------------------------------------------------------------------------
CxString::CxString(double d)
: _data( NULL )
{
    reAssign( (char *) NULL );
    setDouble(d);
}

//-------------------------------------------------------------------------
// CxString::CxString
//
//-------------------------------------------------------------------------
CxString::CxString( const char* cptr_, int len )
: _data(NULL)
{
	reAssign( cptr_, len );
}


//-------------------------------------------------------------------------
// CxString::
//
//-------------------------------------------------------------------------
CxString::CxString( const CxString& sr_ )
: _data( NULL)
{
	if ( &sr_ != this ) {
		//_data = (char*) NULL;
		reAssign( sr_.data() );
	}
}


//-------------------------------------------------------------------------
// CxString::CxString
//
//-------------------------------------------------------------------------
CxString::CxString( const CxString* sr_ )
: _data( NULL )
{
    if ( sr_ != NULL ) {
        reAssign( sr_->data() );
    } else {
        reAssign( (char *) NULL );
    }
}


//-------------------------------------------------------------------------
// CxString::
//
//-------------------------------------------------------------------------
CxString::CxString( const char c_ )
:_data(NULL)
{
    char d[2];
    d[0] = c_;
    d[1] = (char) NULL;
    reAssign( d );
}


//-------------------------------------------------------------------------
// CxString::~CxString
//
//-------------------------------------------------------------------------
CxString::~CxString( void )
{
	if (_data) {
		delete[] _data;
		_data = NULL;
	}	
}


//-------------------------------------------------------------------------
// CxString::operator=
//
//-------------------------------------------------------------------------
CxString&
CxString::operator=( const CxString& sr_ )
{
	if ( &sr_ != this ) {
		reAssign( sr_.data() );
	}
	return( *this );
}


//-------------------------------------------------------------------------
// CxString::operator+
//
//-------------------------------------------------------------------------
CxString
CxString::operator+( const CxString& sr_ )
{
	int newLen = sr_.length() + strlen( _data ) + 1;
	char *cptr = new char[ sr_.length() + strlen( _data ) + 1 ];
	memset( cptr, 0, newLen );

	int len = strlen( _data );

	memcpy( cptr, &(_data[0]), len );
	memcpy( &(cptr[len]), sr_.data(), sr_.length() );

	CxString newString = cptr;
	delete [] cptr;

	return( newString );
}



//-------------------------------------------------------------------------
// CxString::insert
//
//-------------------------------------------------------------------------
void
CxString::insert( const CxString& sr_, int n )
{
	if ( sr_.length()  == 0 ) return;
	
	if ( strlen(_data) == 0 ) {
		reAssign( sr_.data() );
		return;
	}

	if (n==length()) {
		append(sr_);
		return;
	}

	if ( n > strlen(_data) -1 ) n = strlen(_data)-1;
	if ( n < 0 ) n = 0;

	int newLen = sr_.length() + strlen(_data) + 1;
	char *cptr = new char[ sr_.length() + strlen( _data ) + 1 ];
	memset( cptr, 0, newLen );

	// copy first n
	memcpy( cptr, &(_data[0]), n );
	memcpy( cptr + n, sr_.data(), sr_.length() );
//	memcpy( cptr + n + sr_.length(), &(_data[n]), newLen - sr_.length() );

	memcpy( cptr + n + sr_.length(), &(_data[n]), strlen( _data )-n);
	
	reAssign( cptr );

	delete [] cptr;
}


void
CxString::insert( char c, int n)
{
	CxString i(c);
	insert( i, n);
}





//-------------------------------------------------------------------------
// CxString::operator+=
//
//-------------------------------------------------------------------------
CxString&
CxString::operator+=( const CxString& sr_ )
{
	append( sr_ );
	return( *this );
}

//-------------------------------------------------------------------------
// CxString::operator+=
//
//-------------------------------------------------------------------------
CxString&
CxString::operator+=( const char cc_ )
{
	append( cc_ );
	return( *this );
}


//-------------------------------------------------------------------------
// CxString::operator==
//
//-------------------------------------------------------------------------
int
CxString::operator==( const CxString& sr_ ) const
{
    if ( strcmp(sr_.data(), _data) == 0 )  return( TRUE );
    return( FALSE );
}

//-------------------------------------------------------------------------
// CxString::operator!=
//
//-------------------------------------------------------------------------
int
CxString::operator!=( const CxString& sr_ ) const
{
    if ( strcmp(sr_.data(), _data) != 0 )  return( TRUE );
    return( FALSE );
}

//-------------------------------------------------------------------------
// CxString::operator<=
//
//-------------------------------------------------------------------------
int
CxString::operator<=( const CxString& rhs_ ) const
{
    return( strcmp( data(), rhs_.data() ) <= 0 );
}


//-------------------------------------------------------------------------
// CxString::append
//
//-------------------------------------------------------------------------
void
CxString::append( const CxString& sr_ )
{
	int newLen = sr_.length() + strlen( _data ) + 1;
	char *cptr = new char[ sr_.length() + strlen( _data ) + 1 ];
	memset( cptr, 0, newLen );

	int len = strlen( _data );

	memcpy( cptr, &(_data[0]), len );
	memcpy( &(cptr[len]), sr_.data(), sr_.length() );

	reAssign( cptr );

	delete [] cptr;	
}

void
CxString::append( char cc_ )
{
	char s[2];
	s[1] = (char) NULL;
	s[0] = cc_;

	append( CxString( s ));
}



//-----------------------------------------------------------------------------------------------
// CxString::reAssign
//
//------------------------------------------------------------------------------------------------
void
CxString::reAssign( const char *cptr, int len )
{
	//---------------------------------------------------------------------------------------------
	//  if data is a valid pointer, the delete the block and set the pointer to NULL
	//---------------------------------------------------------------------------------------------

    if (_data) {
        delete[] _data;
        _data = NULL;
    }

	// check if the source pointer is an empty string

	if (cptr != NULL) {

		if (len == -1) {

			// the length parameter is default, just make the new memory block 
			// the size of the source string

			_data = new char[ strlen( cptr ) + 1 ];
			if (_data) {
				strcpy( _data, cptr );
			}

		} else {

			// the length parameter was passed in so make the block the the size passed
			// in

			_data = new char[ len + 1 ];
			if (_data) {
				strncpy( _data, cptr, len );
				_data[len]=(char)NULL;
			}
		}
	
	} else {

		// the source pointer is and empty string just create a empty string
		_data = new char[1];
		_data[0]=(char) NULL;
	}
}



//-------------------------------------------------------------------------
// CxString::data
//
//-------------------------------------------------------------------------
char *
CxString::data( void ) const
{
	return( _data );
}


//-------------------------------------------------------------------------
// CxString::firstChar
//
//-------------------------------------------------------------------------
int
CxString::firstChar( const char ch ) const
{
	if ( isNull() )  return( -1 );
		
	int c;
	int i = strlen( _data );

	for (c=0; c<i; c++ ) {
		if ( _data[c] == ch ) return( c );
	}

	return( -1 );
}

int
CxString::charAt(int index)
{
    return( _data[index] );
}

//-------------------------------------------------------------------------
// CxString::lastChar
//
//-------------------------------------------------------------------------
int
CxString::lastChar( const char ch ) const
{
	if ( isNull() )  return( -1 );
		
	int c;
	int i = strlen( _data );

	if (i==0) return -1;

	for (c=i-1; c>=0; c-- ) {
		if ( _data[c] == ch ) return( c );
	}

	return( -1 );
}


//-------------------------------------------------------------------------
// CxString::firstChar
//
//-------------------------------------------------------------------------
int
CxString::firstChar( const char *delimSet, char *theDelim) const
{
	if ( isNull() )  return( -1 );
		
	int c;
    int j;
    int delimSetLength = strlen( delimSet );
	int i = strlen( _data );

	for (c=0; c<i; c++ ) {
        for (j=0; j<delimSetLength; j++) {
		    if ( _data[c] == delimSet[j] ) {
				if (theDelim != NULL) *theDelim = delimSet[j];
				return( c );
			}
	    }
    }

	return( -1 );
}


//-------------------------------------------------------------------------
// CxString::index
//
//-------------------------------------------------------------------------
int
CxString::index( CxString s_, int startpos_ ) const
{
	if ( isNull() )  return( -1 );

	int c;
	int i = strlen( _data );

	if ( startpos_  > i-1) return( -1 );

	for (c=startpos_; c<i; c++ ) {
        
        if (!bcmp( (void *) &_data[c], (void *) s_.data(), s_.length() )) {
        
        // if (!strncmp( (const char *) &_data[c], (const char *) s_.data(), s_.length())) {
            return(c);
        }
    }
	return( -1 );
}	


//-------------------------------------------------------------------------
// CxString::stripLeading
//
//-------------------------------------------------------------------------
CxString&
CxString::stripLeading( const char *charSet_ )
{
	if ( isNull() ) return( *this );

	while ( 1 ) {

		if (_data[0] == (char) NULL) {
			reAssign( (char*) NULL );	
			return( *this );
		}

		if (CxString::charInSet(_data[0], charSet_)) {
			memmove( &(_data[0]), &(_data[1]), strlen( _data ) );
		} else {
			return( *this );
		}
	}
}


//-------------------------------------------------------------------------
// CxString::stripTrailing
//
//-------------------------------------------------------------------------
CxString&
CxString::stripTrailing( const char *charSet_ )
{
	if ( isNull() ) return( *this );

	int cursor = strlen( _data ) -1;

	while ( 1 ) {

		if (_data[cursor] != (char) NULL) {
			if (CxString::charInSet(_data[ cursor ], charSet_)) {
				_data[ cursor ] = (char) NULL;
			} else {
				return( *this );
			}
		}

		if (cursor == 0) {
			return( *this );
		}

		cursor--;
	}
}


//-------------------------------------------------------------------------
// CxString::nextToken
//
//-------------------------------------------------------------------------
CxString
CxString::nextToken( const char *charSet_, char *delim)
{
	stripLeading( charSet_ );

	// the the position of the first character in the charset
	int pos = firstChar( charSet_, delim);

	// if position is -1 that means we hit the end of the string
	// so none of the characters where found
	if (pos == -1) {

		pos = length();
//		if (delim != NULL) *delim = (char) NULL;
	
	} else {

//		CxString delimString = subString( pos, 1);
//		if (delim != NULL) *delim = delimString.data()[0];
	}

	CxString s = subString( 0, pos );
	
	remove( 0, pos );

	return( s );
}


//-------------------------------------------------------------------------
// CxString::substring
//
//-------------------------------------------------------------------------
CxString
CxString::subString( int start, int len )
{
	CxString s;

	if (start < 0) return( *this );
    if (len  < 0) return( *this );

    if (len==0) return s;  // return empty string
	if (start > (int) strlen( _data )) return( s ); // return empty string

	while ( start+len > (int) strlen(_data) ) len--;

	char *cptr = new char[ len+1 ];
	memset( cptr, 0, len+1);
	memcpy( cptr, &(_data[start]), len );

	s = cptr;

	delete[] cptr;

	return( s );
}


//-------------------------------------------------------------------------
// CxString::remove
//
//-------------------------------------------------------------------------
CxString&
CxString::remove( int start, int len )
{
    //printf("CxString::remove():start position = %d\n", start);
    //printf("CxString::remove():len to removed = %d\n", len);
    //printf("CxString::remove():strlen to removed = %d\n", strlen( _data ));


	if (start < 0) return( *this );
	if (start > (int) strlen( _data )-1) return( *this );
	if (len < 0) return( *this );

    /*
    char *srcPtr  = &_data[start+1];
    char *destPtr = &_data[start];

    while ( *srcPtr != (char) NULL) {
        *destPtr = *srcPtr;
        destPtr++;
        srcPtr++;
    }

    *destPtr = (char) NULL;
    */
    
	while (start+len > (int) strlen(_data)) len--;

	int sLen = strlen( _data );
	memmove( &(_data[start]), &(_data[start+len]), sLen-(start+len)+1);


    return(*this);
}

/*
int
CxString::findAndReplaceFirst( CxString findString, CxString replaceString )
{
	// first get the index of the first occurance of the find string
    int i = index(findString);
	if (i!=-1) {

		// compute the new length
        int totalNewLength = length() - findString.length() + replaceString.length() + 1;

		// allocate a new buffer for the resulting new string
        char *newBuffer = new char[totalNewLength];

		// scr pointer is the existing contents of the string
		// dest pointer is the newly allocated buffer
		char *srcPtr  = _data;
		char *destPtr = newBuffer;

		// copy first part of the original string up until the 
		// the occurance of the find string
		int pos = 0;
		while (pos++ < i) {
			*destPtr++ = *srcPtr++;
		}

		// advance the the source pointer beyond the find string in
		// in the src buffer
		srcPtr += findString.length();

		// get the length of the replace string and a pointer
		// to the data
		int replaceStringLength = replaceString.length();
		char *replaceStringPtr = replaceString.data();

		// copy the replace string into the destination buffer
		pos = 0;
		while (pos++ < replaceStringLength) {
			*destPtr++ = *replaceStringPtr++;
		}

		// copy the remainder of the original src string
		// into new buffer
		while (*srcPtr != '\000') {
			*destPtr++ = *srcPtr++;
		}
		*destPtr = '\000';

		reAssign(newBuffer);

		delete [] newBuffer;
	
	}

	return(i);
}
 */


int
CxString::findAndReplaceFirst( CxString findString, CxString replaceString, int startPos )
{
    // first get the index of the first occurance of the find string
    int i = index(findString, startPos);
    if (i!=-1) {
        
        // compute the new length
        int totalNewLength = length() - findString.length() + replaceString.length() + 1;

        // allocate a new buffer for the resulting new string
        char *newBuffer = new char[totalNewLength];

        // scr pointer is the existing contents of the string
        // dest pointer is the newly allocated buffer
        char *srcPtr  = _data;
        char *destPtr = newBuffer;
 
        // copy first part of the original string we skipped plus the part
        // we didn't skip that did not match the find string up until the
        // the occurance of the find string
        int pos = 0;
        while (pos++ < i) {
            *destPtr++ = *srcPtr++;
        }
            
        // advance the the source pointer beyond the find string in
        // in the src buffer
        srcPtr += findString.length();

        // get the length of the replace string and a pointer
        // to the data
        int replaceStringLength = replaceString.length();
        char *replaceStringPtr = replaceString.data();

        // copy the replace string into the destination buffer
        pos = 0;
        while (pos++ < replaceStringLength) {
            *destPtr++ = *replaceStringPtr++;
        }

        // copy the remainder of the original src string
        // into new buffer
        while (*srcPtr != '\000') {
            *destPtr++ = *srcPtr++;
        }
        *destPtr = '\000';

        reAssign(newBuffer);

        delete [] newBuffer;
    }

    return(i);
}


//-------------------------------------------------------------------------
// CxString::replaceAll
//
//-------------------------------------------------------------------------
int
CxString::replaceAll(CxString match, CxString replace)
{
    int num = 0;
    int foundPosition = -1;
    int startPosition =  0;
    int done = false;

    while (!done) {

        foundPosition = index( match, startPosition);

        if (foundPosition != -1) {

            num++;

            remove( foundPosition, match.length() );
            insert( replace, foundPosition);

            startPosition = foundPosition + replace.length();

        } else {
            
            done = true;
        
        }
    }

    return( num );
}

int
CxString::numberOfOccurances( CxString match )
{
    int num = 0;
    int foundPosition = -1;
    int startPosition =  0;
    int done = false;

    while (!done) {

        foundPosition = index( match, startPosition);

        if (foundPosition != -1) {
            num++;
            startPosition = foundPosition + match.length();
        } else {
            done = true;
        }
    }

    return( num );
}


//-------------------------------------------------------------------------
// CxString::length
//
//-------------------------------------------------------------------------
int
CxString::length( void ) const
{
	return( strlen( _data ) );
}


//-------------------------------------------------------------------------
// CxString::toInt
//
//-------------------------------------------------------------------------
int
CxString::toInt( void )
{
    int i;
    if (!isNull()) {
        if (sscanf(_data, "%d", &i )==1) {
            return( i );
        }
    }

    return(0);
}


//-------------------------------------------------------------------------
// CxString::toLong
//
//-------------------------------------------------------------------------
long
CxString::toLong( void )
{
    long l;
    if (!isNull()) {
        if (sscanf(_data, "%ld", &l )==1) {
            return( l );
        }
    }

    return((long) 0);
}


//-------------------------------------------------------------------------
// CxString::toUnsignedLong
//
//-------------------------------------------------------------------------
unsigned long
CxString::toUnsignedLong( void )
{
    unsigned long ul;
    if (!isNull()) {
        if (sscanf(_data, "%lu", &ul )==1) {
            return( ul );
        }
    }

    return((unsigned long) 0);
}


//-------------------------------------------------------------------------
// CxString::toFloat
//
//-------------------------------------------------------------------------
float
CxString::toFloat( void )
{
    float f;
    if (!isNull()) {
        if (sscanf(_data, "%f", &f )==1) {
            return( f );
        }
    }

    return((float) 0.0);
}


//-------------------------------------------------------------------------
// CxString::toDouble
//
//-------------------------------------------------------------------------
double
CxString::toDouble( void )
{
    double d;
    if (!isNull()) {
        if (sscanf(_data, "%lf", &d )==1) {
            return( d );
        }
    }

    return((double) 0.0);
}

//-------------------------------------------------------------------------
// CxString::
//
//-------------------------------------------------------------------------
int
CxString::charInString( char ch )
{
    int i = firstChar(ch);
    if (i==-1) return(FALSE);
    return( TRUE );
}

//-------------------------------------------------------------------------
// CxString::charInSet
//
//-------------------------------------------------------------------------
int
CxString::charInSet( char ch, const char *charSet_ )
{
	int c;
	int len = strlen( charSet_ );

	for (c=0; c<len; c++) {
		if (ch==charSet_[c]) return(1);
	}
	return(0);
}


//-------------------------------------------------------------------------
// CxString::isNull
//
//-------------------------------------------------------------------------
int
CxString::isNull(void) const
{
	if (!_data) return( TRUE );
	if (_data[0] == (char) NULL) return( TRUE );
	return( FALSE );
}


//-------------------------------------------------------------------------
// CxString::operator<<
//
//-------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& str, const CxString& cxstring_ )
{
    str << cxstring_.data();
    return(str);
}


//-------------------------------------------------------------------------
// CxString::netNormalize
//
//-------------------------------------------------------------------------
CxString
CxString::netNormalize( CxString line )
{
    int nlIndex;
    int crIndex;

    do {
        nlIndex = line.firstChar('\012');
        if (nlIndex != -1) line = line.remove(nlIndex,1);
    } while (nlIndex != -1);

    do {
        crIndex = line.firstChar('\015');
        if (crIndex != -1) line = line.remove(crIndex,1);
    } while (crIndex != -1);

    return(line);
}

//-------------------------------------------------------------------------
// CxString::netNormalize
//
//-------------------------------------------------------------------------
CxString
CxString::removeCarriageReturns( CxString line )
{
    int crIndex;

    do {
        crIndex = line.firstChar('\015');
        if (crIndex != -1) line = line.remove(crIndex,1);
    } while (crIndex != -1);

    return(line);
}




//-------------------------------------------------------------------------
// CxString::toUpper
//
//-------------------------------------------------------------------------
/* static */
CxString
CxString::toUpper( CxString s )
{
    char *cptr = s.data();
    for (int c=0; c<s.length(); c++) {
        *cptr = toupper( *cptr );
        cptr++;
    }
    return( s );
}


int
CxString::compare( CxString s )
{
    return( strcmp( data(), s.data() ));
}


void
CxString::printf( const char * format, ... )
{
  char buffer[1000];
  va_list args;
  va_start (args, format);
  vsprintf (buffer, format, args);
    reAssign( buffer );
  va_end (args);
}


//-------------------------------------------------------------------------
// CxString::toLower
//
//-------------------------------------------------------------------------
/* static */
CxString
CxString::toLower( CxString s )
{
    char *cptr = s.data();
    for (int c=0; c<s.length(); c++) {
        *cptr = tolower( *cptr );
        cptr++;
    }
    return( s );
}


void
CxString::setString( CxString s )
{
	reAssign( s.data() );	
}

void
CxString::setInt(int i)
{
	char buffer[25];
	sprintf(buffer, "%d", i);
	reAssign( buffer );
}

void
CxString::setLong(long l)
{
	char buffer[25];
	sprintf(buffer, "%ld", l);
	reAssign( buffer );
}

void
CxString::setUnsignedLong(unsigned long ul)
{
	char buffer[25];
	sprintf(buffer, "%lu", ul);
	reAssign( buffer );
}

void
CxString::setDouble(double d)
{
	char buffer[25];
	sprintf(buffer, "%lf", d);
	reAssign( buffer );
}


//-------------------------------------------------------------------------
// CxString::hashValue
//
//-------------------------------------------------------------------------
unsigned int
CxString::hashValue( void ) const
{
	unsigned int i = 0;

	char *ptr = &_data[0];

	while (*ptr != (char) NULL) {

		i += (unsigned int) *ptr;

		ptr++;
	}

	return( i );
}


//-------------------------------------------------------------------------
// CxString::isFloat
//
//-------------------------------------------------------------------------
int
CxString::isFloat( void )
{
	char *ptr = &_data[0];

	while (*ptr != (char) NULL) {

		switch( *ptr ) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '.':
			case '-':
			case '+':
				break;
			default:
				return(FALSE);
		}
		ptr++;
	}

	return( TRUE );

}


//-------------------------------------------------------------------------
// CxString::isInt
//
//-------------------------------------------------------------------------
int
CxString::isInt( void )
{
	char *ptr = &_data[0];

	while (*ptr != (char) NULL) {

		switch( *ptr ) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '-':
			case '+':
				break;
			default:
				return(FALSE);
		}
		ptr++;
	}

	return( TRUE );

}


//-------------------------------------------------------------------------
// CxString::urlDecode
//
//-------------------------------------------------------------------------
static int hexCharToInt( char c )
{
	if (c >= '0' && c <= '9') return c - '0';
	if (c >= 'A' && c <= 'F') return c - 'A' + 10;
	if (c >= 'a' && c <= 'f') return c - 'a' + 10;
	return -1;
}

CxString
CxString::urlDecode( CxString s_ )
{
	CxString result;
	char *ptr = s_.data();

	while (*ptr != (char) NULL) {
		if (*ptr == '+') {
			result.append(' ');
		} else if (*ptr == '%' && ptr[1] != (char) NULL && ptr[2] != (char) NULL) {
			int hi = hexCharToInt(ptr[1]);
			int lo = hexCharToInt(ptr[2]);
			if (hi >= 0 && lo >= 0) {
				char decoded = (char)((hi << 4) | lo);
				result.append(decoded);
				ptr += 2;
			} else {
				result.append(*ptr);
			}
		} else {
			result.append(*ptr);
		}
		ptr++;
	}

	return( result );
}
