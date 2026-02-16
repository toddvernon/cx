//-------------------------------------------------------------------------------------------------
//
//  json_array.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxJSONArray Class
//
//-------------------------------------------------------------------------------------------------

#include <cx/json/json_array.h>


//-------------------------------------------------------------------------
// CxJSONArray::CxJSONArray
//
//-------------------------------------------------------------------------
CxJSONArray::CxJSONArray(void)
{
    _type = CxJSONBase::ARRAY;
}

//-------------------------------------------------------------------------
// CxJSONArray::~CxJSONArray
//
//-------------------------------------------------------------------------
CxJSONArray::~CxJSONArray(void)
{
	clear();
}

void
CxJSONArray::append( CxJSONBase *o)
{
	_objectList.append(o);
}

//-------------------------------------------------------------------------
// CxJSONArray::entries
//
//-------------------------------------------------------------------------
int
CxJSONArray::entries( void ) const
{
    return (_objectList.entries() );
}


//-------------------------------------------------------------------------
// CxJSONArray::at
//
//-------------------------------------------------------------------------
CxJSONBase *
CxJSONArray::at( int i ) const
{
    CxJSONBase *o = _objectList.at( i );
    return(o);
}

//-------------------------------------------------------------------------
// CxJSONArray::clear
//
//-------------------------------------------------------------------------
void
CxJSONArray::clear(void)
{
    for (unsigned int c=0; c<_objectList.entries(); c++) {
        CxJSONBase *o = _objectList.at(c);
		delete o;
	}

    _objectList.clear();
}



/* virtual */
void CxJSONArray::print(std::ostream& str ) const
{
    int first = 1;

    str << "[";
    for (unsigned int c=0; c< _objectList.entries(); c++) {
        CxJSONBase *b =  _objectList.at(c);

		if (!first) str << ",";

        str << *b;

        first = 0;
    }
    str << "]";
}

/* virtual */
CxString CxJSONArray::toJsonString(void) const
{
    CxString result("[");
    int first = 1;

    for (unsigned int c=0; c< _objectList.entries(); c++) {
        CxJSONBase *b =  _objectList.at(c);

        if (!first) result += ",";

        if (b != NULL) {
            result += b->toJsonString();
        } else {
            result += "null";
        }

        first = 0;
    }
    result += "]";
    return result;
}


//-------------------------------------------------------------------------
// CxString::operator<<
//
//-------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& str, const CxJSONArray& a_ )
{
	a_.print(str);
    return(str);
}
 
