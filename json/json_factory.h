/***************************************************************************
 *
 *  prop.h
 *
 *  CxPropEntry, CxPropertyList Class
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

#include <iostream>

#include <cx/base/string.h>
#include <cx/base/slist.h>

#include <cx/json/nxjson.h>
#include <cx/json/json_base.h>
#include <cx/json/json_null.h>
#include <cx/json/json_boolean.h>
#include <cx/json/json_string.h>
#include <cx/json/json_number.h>
#include <cx/json/json_object.h>
#include <cx/json/json_array.h>


#ifndef _CXJSON_FACTORY_
#define _CXJSON_FACTORY_



//-------------------------------------------------------------------------
// CxJSONBoolean
//
//-------------------------------------------------------------------------
class CxJSONFactory
{
  public:

	CxJSONFactory( void );

	~CxJSONFactory( void );

    static 
	CxJSONBase *parse( CxString text );

    static 
	void walktree(const nx_json* json, CxJSONBase *cxjObject );

};


#endif
