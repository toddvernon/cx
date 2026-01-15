/***************************************************************************
 *
 *  json_array.cpp
 *
 *  CxJSONArray Class
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
   

//-------------------------------------------------------------------------
// CxString::operator<<
//
//-------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& str, const CxJSONArray& a_ )    
{
	a_.print(str);
    return(str);
}
 

