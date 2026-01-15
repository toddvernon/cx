/***************************************************************************
 *
 *  filename.cpp
 *
 *  CxFileName Class
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

#include <cx/base/filename.h>

//-------------------------------------------------------------------------
// <constructor>
//
//-------------------------------------------------------------------------
CxFileName::CxFileName( void )
{
}


//-------------------------------------------------------------------------
// <constructor>
//
//-------------------------------------------------------------------------
CxFileName::CxFileName( CxString name )
{
    // check for zero length name
    if (name.length()==0) {
        return;
    }
    
    _dotFound = FALSE;
    char charString[2];
    charString[0] = ' ';
    charString[1] = (char) NULL;
    
    char *cptr = name.data() + name.length()-1;
    for (int c=name.length()-1; c>=0; c--) {
        
        if ((*cptr == '.') && (!_dotFound)) {
            _dotFound=TRUE;
        } else {
            if (!_dotFound) {
                charString[0] = *cptr;
                _extension = CxString( charString ) + _extension;
            } else {
                charString[0] = *cptr;
                _name = CxString( charString ) + _name;
            }
        }
        
        cptr--;
    }
    
    if (!_dotFound) {
        _name = _extension;
        _extension = "";
    }
}


//-------------------------------------------------------------------------
// <constructor>
//
//-------------------------------------------------------------------------
CxFileName::CxFileName( const CxFileName& f)
{
    if (&f != this) {
        _name      = f._name;
        _extension = f._extension;
        _dotFound  = f._dotFound;
    }
}


//-------------------------------------------------------------------------
// <destructor>
//
//-------------------------------------------------------------------------
CxFileName::~CxFileName( void )
{
}


//-------------------------------------------------------------------------
// CxFileName::operator=
//
//-------------------------------------------------------------------------
CxFileName&
CxFileName::operator=( const CxFileName& f )
{
    if (&f != this) {
        _name      = f._name;
        _extension = f._extension;
        _dotFound  = f._dotFound;
    }
    return( *this );
}


//-------------------------------------------------------------------------
// CxFileName::operator<<
//
//-------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& str, const CxFileName& cxfn )
{
    if (cxfn._dotFound) {
        str << cxfn._name << CxString(".") << cxfn._extension;
    } else {
        str << cxfn._name;
    }
    
    return(str);
}


//-------------------------------------------------------------------------
// CxFileName::setName
//
//-------------------------------------------------------------------------
void
CxFileName::setName( CxString n)
{
    _name = n;
}


//-------------------------------------------------------------------------
// CxFileName::name
//
//-------------------------------------------------------------------------
CxString
CxFileName::name(void)
{
    return( _name );
}


//-------------------------------------------------------------------------
// CxFileName::setExtension
//
//-------------------------------------------------------------------------
void
CxFileName::setExtension( CxString e )
{
    _extension = e;
}


//-------------------------------------------------------------------------
// CxFileName::extension
//
//-------------------------------------------------------------------------
CxString
CxFileName::extension(void)
{
    return( _extension );
}


//-------------------------------------------------------------------------
// CxFileName::fullName
//
//-------------------------------------------------------------------------
CxString
CxFileName::fullName(void)
{
    if (_dotFound) {
        return( _name + CxString(".") + _extension );
    } else {
        return( _name );
    }
}
