//-------------------------------------------------------------------------------------------------
//
//  json_utf_array.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxJSONUTFArray Class - UTF-8 aware JSON array
//
//-------------------------------------------------------------------------------------------------

#include <cx/json/json_utf_array.h>


//-------------------------------------------------------------------------
// CxJSONUTFArray::CxJSONUTFArray
//
//-------------------------------------------------------------------------
CxJSONUTFArray::CxJSONUTFArray(void)
{
    _type = CxJSONUTFBase::ARRAY;
}

//-------------------------------------------------------------------------
// CxJSONUTFArray::~CxJSONUTFArray
//
//-------------------------------------------------------------------------
CxJSONUTFArray::~CxJSONUTFArray(void)
{
	clear();
}

void
CxJSONUTFArray::append( CxJSONUTFBase *o)
{
	_objectList.append(o);
}

//-------------------------------------------------------------------------
// CxJSONUTFArray::entries
//
//-------------------------------------------------------------------------
int
CxJSONUTFArray::entries( void ) const
{
    return (_objectList.entries() );
}


//-------------------------------------------------------------------------
// CxJSONUTFArray::at
//
//-------------------------------------------------------------------------
CxJSONUTFBase *
CxJSONUTFArray::at( int i ) const
{
    CxJSONUTFBase *o = _objectList.at( i );
    return(o);
}

//-------------------------------------------------------------------------
// CxJSONUTFArray::clear
//
//-------------------------------------------------------------------------
void
CxJSONUTFArray::clear(void)
{
    for (unsigned int c=0; c<_objectList.entries(); c++) {
        CxJSONUTFBase *o = _objectList.at(c);
		delete o;
	}

    _objectList.clear();
}



/* virtual */
void CxJSONUTFArray::print(std::ostream& str ) const
{
    int first = 1;

    str << "[";
    for (unsigned int c=0; c< _objectList.entries(); c++) {
        CxJSONUTFBase *b =  _objectList.at(c);

		if (!first) str << ",";

        str << *b;

        first = 0;
    }
    str << "]";
}

/* virtual */
CxString CxJSONUTFArray::toJsonString(void) const
{
    CxString result("[");
    int first = 1;

    for (unsigned int c=0; c< _objectList.entries(); c++) {
        CxJSONUTFBase *b =  _objectList.at(c);

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

/* virtual */
CxString CxJSONUTFArray::toPrettyJsonString(int indent) const
{
    CxString result("[\n");
    CxString indentStr;
    CxString nextIndentStr;

    // Build indentation strings
    for (int i = 0; i < indent; i++) {
        indentStr += "  ";
    }
    nextIndentStr = indentStr + "  ";

    int first = 1;

    for (unsigned int c = 0; c < _objectList.entries(); c++) {
        CxJSONUTFBase *b = _objectList.at(c);

        if (!first) result += ",\n";

        result += nextIndentStr;
        if (b != NULL) {
            result += b->toPrettyJsonString(indent + 1);
        } else {
            result += "null";
        }

        first = 0;
    }

    result += "\n";
    result += indentStr;
    result += "]";
    return result;
}


//-------------------------------------------------------------------------
// CxJSONUTFArray::operator<<
//
//-------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& str, const CxJSONUTFArray& a_ )
{
	a_.print(str);
    return(str);
}
