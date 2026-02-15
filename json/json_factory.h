//-------------------------------------------------------------------------------------------------
//
//  json_factory.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxPropEntry, CxPropertyList Class
//
//-------------------------------------------------------------------------------------------------

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
