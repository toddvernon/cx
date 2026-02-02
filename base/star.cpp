/***************************************************************************
 *
 *  star.cpp
 *
 *  CxMatchTemplate, CxStringMatch classes
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

#include <cx/base/star.h>


//-------------------------------------------------------------------------
// CxStringMatch::CxStringMatch
//
//-------------------------------------------------------------------------
CxStringMatch::CxStringMatch( void )
{
	_buffer    = "";
	_lastIndex = 0;

}

//-------------------------------------------------------------------------
// CxStringMatch::CxStringMatch
//
//-------------------------------------------------------------------------
CxStringMatch::CxStringMatch( CxString s_ )
{
	_buffer    = s_;
	_lastIndex = 0;
}


//-------------------------------------------------------------------------
// CxStringMatch::isNext
//
//-------------------------------------------------------------------------
int
CxStringMatch::isNext( CxString sub_ )
{
	int i = _buffer.index( sub_, _lastIndex );

	if (i != -1) {
		_lastIndex = i+1;
		return( TRUE );
	} else {
		return( FALSE );
	}
}


//-------------------------------------------------------------------------
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
// CxMatchTemplate::CxMatchTemplate
//
//-------------------------------------------------------------------------
CxMatchTemplate::CxMatchTemplate( void )
{
	_tString       = "";
	_leadingStar   = 0;
	_trailingStar  = 0;
	_matchAll      = FALSE;
	_matchNone     = TRUE;   // Empty pattern matches nothing

	_parts.clear();
}

//-------------------------------------------------------------------------
// CxMatchTemplate::CxMatchTemplate
//
//-------------------------------------------------------------------------
CxMatchTemplate::CxMatchTemplate( CxString t )
{
	_tString       = t;
	_leadingStar   = 0;
	_trailingStar  = 0;
	_matchAll      = FALSE;
	_matchNone     = FALSE;
	_parts.clear();
	parse( t );
}

//-------------------------------------------------------------------------
// CxMatchTemplate::CxMatchTemplate
//
//-------------------------------------------------------------------------
CxMatchTemplate::CxMatchTemplate( const char *t )
{
	CxString temp(t);

	_tString       = t;
	_leadingStar   = 0;
	_trailingStar  = 0;
	_matchAll      = FALSE;
	_matchNone     = FALSE;
	_parts.clear();
	parse( t );
}

//-------------------------------------------------------------------------
// CxMatchTemplate::CxMatchTemplate
//
//-------------------------------------------------------------------------
CxMatchTemplate::CxMatchTemplate( const CxMatchTemplate& ts )
{
	if ( this != &ts ) {
		_tString      = ts._tString;
		_leadingStar  = ts._leadingStar;
		_trailingStar = ts._trailingStar;
		_matchAll     = ts._matchAll;
		_matchNone    = ts._matchNone;
		_parts.clear();
		parse( _tString );
	}
}

//-------------------------------------------------------------------------
// CxMatchTemplate::operator=
//
//-------------------------------------------------------------------------
CxMatchTemplate&
CxMatchTemplate::operator=( const CxMatchTemplate& ts )
{
	if ( this != &ts ) {
		_tString = ts._tString;
		_leadingStar  = ts._leadingStar;
		_trailingStar = ts._trailingStar;
		_matchAll     = ts._matchAll;
		_matchNone    = ts._matchNone;
		_parts.clear();
		parse( _tString );
	}
	return(*this);
}

//-------------------------------------------------------------------------
// CxMatchTemplate::parse
//
//-------------------------------------------------------------------------
void
CxMatchTemplate::parse( CxString t )
{
	_matchAll     = FALSE;
	_matchNone    = FALSE;
	_leadingStar  = FALSE;
	_trailingStar = FALSE;

	t = t.stripLeading(" ");

	if ( t.length()) {

		if (t.data()[t.length()-1]=='*') _trailingStar = TRUE;
		if (t.data()[0]=='*')            _leadingStar  = TRUE;
		if ( (t.length()==1) && (_leadingStar) )  _matchAll     = TRUE;

	} else {

		_matchNone = TRUE;
		return;
	}


	CxString p = t.nextToken("*");
	while (p.length()) {
		_parts.append( p );
		p = t.nextToken("*");
	}
}


//-------------------------------------------------------------------------
// CxMatchTemplate::test
//
//-------------------------------------------------------------------------
int
CxMatchTemplate::test( CxString candidate )
{
	// check the candidate for zero length
	candidate = candidate.stripLeading(" ");
	candidate = candidate.stripTrailing(" ");

	if (!candidate.length()) return(FALSE);

	// check the easy outs
	if (_matchNone) return( FALSE );
	if (_matchAll)  return( TRUE  );

	// Special case: no wildcards at all means exact match required
	// (like shell glob behavior where "test" only matches "test")
	if (!_leadingStar && !_trailingStar && _parts.entries() == 1) {
		return( candidate == _parts.at(0) );
	}

	// build a string match object
	CxStringMatch sm( candidate );

	// walk through the list of parts and see
	// if the happen in sequence in the candidate
	for (unsigned int c=0; c<_parts.entries(); c++) {
		if ( !sm.isNext( _parts.at(c) )) return( FALSE );
	}

	// if the template did not start with a star then
	// we have to match the first object at the beginning of
	// of the candidate
	if (!_leadingStar) {
		if ( candidate.index( _parts.at(0) ) ) {
			return( FALSE );
		}
	}

	// if the template did not end with a star then
	// we have to match the last object at the end of the
	// candidate string.
	if (!_trailingStar) {

		CxString p = _parts.at( _parts.entries()-1 );
		int offset = candidate.length() - p.length();
		if ( offset >= 0 ) {
			if ( p != candidate.subString( offset, p.length() )) {
				return(FALSE);
			}
		}
	}
	return( TRUE );
}
