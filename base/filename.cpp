//-------------------------------------------------------------------------------------------------
//
//  filename.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxFileName Class
//
//-------------------------------------------------------------------------------------------------

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
