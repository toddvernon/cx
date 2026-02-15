//-------------------------------------------------------------------------------------------------
//
//  prop.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxPropEntry, CxPropertyList Class
//
//-------------------------------------------------------------------------------------------------

#include <cx/base/prop.h>


//-------------------------------------------------------------------------
// CxPropEntry::CxPropEntry
//
//-------------------------------------------------------------------------
CxPropEntry::CxPropEntry()
{
}

//-------------------------------------------------------------------------
// CxPropEntry::CxPropEntry
//
//-------------------------------------------------------------------------
CxPropEntry::CxPropEntry( CxString var_, CxString val_ )
{
    _var = var_;
    _val = val_;
}

//-------------------------------------------------------------------------
// CxPropEntry::CxPropEntry
//
//-------------------------------------------------------------------------
CxPropEntry::CxPropEntry(const CxPropEntry& pe)
{
	if (this != &pe) {
		_var = pe.var();
		_val = pe.val();
	}
}

//-------------------------------------------------------------------------
// CxPropEntry::operator=
//
//-------------------------------------------------------------------------
CxPropEntry&
CxPropEntry::operator=(const CxPropEntry& pe)
{
	if (this != &pe) {
		_var = pe.var();
		_val = pe.val();
	}
	return(*this);
}

//-------------------------------------------------------------------------
// CxPropEntry::operator==
//
//-------------------------------------------------------------------------
int
CxPropEntry::operator==(const CxPropEntry& pe) const
{
    if ((_var == pe.var()) && (_val == pe.val())) return (1);
    return (0);
};

//-------------------------------------------------------------------------
// CxPropEntry::var
//
//-------------------------------------------------------------------------
CxString
CxPropEntry::var(void) const
{
	return(_var);
}

//-------------------------------------------------------------------------
// CxPropEntry::val
//
//-------------------------------------------------------------------------
CxString
CxPropEntry::val(void) const
{
	return(_val);
}



//-------------------------------------------------------------------------
// CxPropertyList::CxPropertyList
//
//-------------------------------------------------------------------------
CxPropertyList::CxPropertyList(void)
{
}

//-------------------------------------------------------------------------
// CxPropertyList::~CxPropertyList
//
//-------------------------------------------------------------------------
CxPropertyList::~CxPropertyList(void)
{
}

//-------------------------------------------------------------------------
// CxPropertyList::CxPropertyList
//
//-------------------------------------------------------------------------
CxPropertyList::CxPropertyList( const CxPropertyList& p )
{
	if (this != &p) {
		_list = p._list;
	}
}

//-------------------------------------------------------------------------
// CxPropertyList::operator=
//
//-------------------------------------------------------------------------
CxPropertyList&
CxPropertyList::operator=( const CxPropertyList& p )
{
	if (this != &p) {
		_list = p._list;
	}
    return (*this);
}

//-------------------------------------------------------------------------
// CxPropertyList::operator+
//
//-------------------------------------------------------------------------
CxPropertyList&
CxPropertyList::operator+( CxPropertyList& p )
{
    if ( this == &p ) return *this;

	_list.append( p._list );

    return(*this);
}


//-------------------------------------------------------------------------
// CxPropertyList::set
//
//-------------------------------------------------------------------------
void
CxPropertyList::set( CxString p, CxString v )
{
	for (unsigned int c=0; c<_list.entries(); c++) {

		CxPropEntry pe = _list.at(c);

		if (pe.var() == p ) {
			CxPropEntry pp( pe.var(), v );
			_list.replaceAt( c, pp );
			return;
		}
	}

	CxPropEntry pp( p, v );
	_list.append( pp );
}


//-------------------------------------------------------------------------
// CxPropertyList::get
//
//-------------------------------------------------------------------------
CxString
CxPropertyList::get( CxString p ) const
{
	for (unsigned int c=0; c<_list.entries(); c++) {

		CxPropEntry pe = _list.at(c);

		if (pe.var() == p ) {
			return( pe.val() );
		}
	}
    return ("");
};


//-------------------------------------------------------------------------
// CxPropertyList::has
//
//-------------------------------------------------------------------------
int
CxPropertyList::has( CxString p ) const
{
	for (unsigned int c=0; c<_list.entries(); c++) {

		CxPropEntry pe = _list.at(c);
		if (pe.var() == p ) {
			return( TRUE );
		}
	}
    return ( FALSE );
}


//-------------------------------------------------------------------------
// CxPropertyList::remove
//
//-------------------------------------------------------------------------
int
CxPropertyList::remove( CxString p )
{
	for (unsigned int c=0; c<_list.entries(); c++) {

		CxPropEntry pe = _list.at(c);
		if (pe.var() == p ) {
			_list.removeAt( c );
			return( TRUE );
		}
	}
    return ( FALSE );
}


//-------------------------------------------------------------------------
// CxPropertyList::entries
//
//-------------------------------------------------------------------------
int
CxPropertyList::entries( void ) const
{
    return (_list.entries());
}


//-------------------------------------------------------------------------
// CxPropertyList::at
//
//-------------------------------------------------------------------------
void
CxPropertyList::at( int i, CxString *var, CxString *val) const
{
    CxPropEntry pe = _list.at( i );
    *var = pe.var();
    *val = pe.val();
}


//-------------------------------------------------------------------------
// CxPropertyList::set
//
//-------------------------------------------------------------------------
void
CxPropertyList::set( CxString varval )
{
	// remove leading and trailing garbage
	varval.stripLeading(" \r\t");
	varval = CxString::netNormalize( varval );
	
	CxString var = varval.nextToken("=");
	var.stripTrailing(" \t=");

	CxString val = varval;
	val.stripLeading(" =\t");

	set( var, val );
}

/*static*/
CxPropertyList
CxPropertyList::parseArgs(CxString commandline, CxPropertyList argSet)
{
    CxPropertyList returnArgSet;
    
    //printf("PROCESSING=[%s]\n", commandline.data() );

    //---------------------------------------------------------------------
    // while there are still tokens (that look like flags and args
    //
    //---------------------------------------------------------------------

    while (1) {
        
        CxString token = commandline.nextToken(" \t\n");
        token.stripLeading(" \t\n");
        token.stripTrailing(" \t\n");

        //-------------------------------------------------------------------------
        // if token has no length we are at the end of the tokens, return
        //
        //-------------------------------------------------------------------------
        if (token.length()==0) {
            return( returnArgSet );
        }

        //-------------------------------------------------------------------------
        // if next argument does not start with (-) sign
        // than we are done with flags
        //
        //-------------------------------------------------------------------------
        if (token.index("-")==-1) {
            commandline = token + CxString(" ") + commandline;
            returnArgSet.set("*", commandline );
            return( returnArgSet );
        }
        
        //-------------------------------------------------------------------------
        // check each token on the commandline and compare against the definitions
        // that are passed in.  We use this to know what flags are valid as well
        // as how many args each flag expects
        //
        //-------------------------------------------------------------------------
        for (int c=0; c<argSet.entries(); c++) {
            
            CxString key, value;
            argSet.at(c, &key, &value);

            //---------------------------------------------------------------------
            // if token is one of the known flags
            //
            //---------------------------------------------------------------------
            if (token.index(key)==0) {
                
                //-----------------------------------------------------------------
                // the token matches a known flag value but is longer.  We assume
                // that this means that the value is attached to the flag with
                // no space between then (ie -xmin32 == -xmin 32).  We do validate
                // that an argument is expected however later.
                //
                //-----------------------------------------------------------------
                if (token.length()>key.length()) {
                    token.remove(0,key.length());
                } else {
                    
                    //-------------------------------------------------------------
                    // get the defined value for the known flag.  this contains
                    // the number of args that are expected, regardless of their
                    // attachement (above) or not to the flag text physically
                    //
                    //-------------------------------------------------------------

                    if (value.length()) {
                        
                        //---------------------------------------------------------
                        // if flag definition is zero arguments
                        // then just set the value to true as the flag simply
                        // represents a on or off (binary) value
                        //
                        //---------------------------------------------------------
                        if (value == "0") {
                            returnArgSet.set(key, "TRUE");
                            goto NEXT;
                        }
                         
                        //---------------------------------------------------------
                        // if the flag definition is one argument then get the next
                        // argument from the command line
                        //
                        //---------------------------------------------------------
                        if (value == "1") {
                            
                            CxString priorToken = token;
                            token = commandline.nextToken(" \t\n");
                            token.stripLeading(" \t\n");
                            token.stripTrailing(" \t\n");
                            
                            //-----------------------------------------------------
                            // get the next token that should be the value
                            //
                            //-----------------------------------------------------
                            if (token.length()==0) {
                                CxString e; e.printf("Error: missing argument for flag [%s]", priorToken.data() );
                                returnArgSet.set("$ERROR",e);
                                return( returnArgSet );
                            }
                            
                            //-----------------------------------------------------
                            // if the next token starts with a "-" its probably
                            // a syntax error and the value was not entered
                            //
                            //-----------------------------------------------------

                            if (token.index("-")!=-1) {
                                if (argSet.has(token)) {
                                    CxString e;
                                    e.printf("Error: missing argument for flag [%s]", priorToken.data());
                                    returnArgSet.set("$ERROR",e);
                                    return( returnArgSet );
                                }
                            }
                            //-----------------------------------------------------
                            // fall through to set the value for the flag
                            //
                            //-----------------------------------------------------

                        }
                        
                    //-------------------------------------------------------------
                    // there was no value for the flag definition so it was not
                    // an expected flag
                    //
                    //-------------------------------------------------------------
                    } else {
                        CxString e; e.printf("Error: undefined flag [%s]", token.data());
                        returnArgSet.set("$ERROR",e);
                        return( returnArgSet );
                    }
                }
                //-----------------------------------------------------------------
                // set the return value for the expected flag
                //
                //-----------------------------------------------------------------
                returnArgSet.set(key, token);
                goto NEXT;
            }
        }
        
        //-------------------------------------------------------------------------
        // we found a token that was not found as a flag, check to see if it
        // looks like a flag and if so we probably have a parse error
        //
        //-------------------------------------------------------------------------
        if (token.index("-")==0) {
            CxString e; e.printf("Error: undefined flag [%s]", token.data());
            returnArgSet.set("$ERROR",e);
            return( returnArgSet );
        }

        //-------------------------------------------------------------------------
        // copy the remaining stuff as more command line that is not flags and
        // return;
        //
        //-------------------------------------------------------------------------
        commandline = token + CxString(" ") + commandline;
        returnArgSet.set("*", commandline );
        return( returnArgSet );
        
        //-------------------------------------------------------------------------
        // just keep processing tokens
        //
        //-------------------------------------------------------------------------

        NEXT:
            continue;
    }
}


//-------------------------------------------------------------------------
// CxPropertyList::import
//
//-------------------------------------------------------------------------
void
CxPropertyList::import( CxString text_, CxString delims_ )
{
	CxString varval = text_.nextToken( delims_.data() );

	while (varval.length()) {
		set( varval );
		varval = text_.nextToken( delims_.data() );
	}	
}


//-------------------------------------------------------------------------
// CxPropertyList::clear
//
//-------------------------------------------------------------------------
void
CxPropertyList::clear(void)
{
    _list.clear();
}


//-------------------------------------------------------------------------
// CxPropertyList::dump
//
//-------------------------------------------------------------------------
void
CxPropertyList::dump( void )
{
	std::cout << "ENTRIES=" << _list.entries() << std::endl;

	for (unsigned int c=0; c<_list.entries(); c++) {

		CxPropEntry pe = _list.at(c);
		std::cout << "[" << pe.var() << "] = [" << pe.val() << "]" << std::endl;

	}
}
