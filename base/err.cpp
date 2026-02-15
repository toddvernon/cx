//-------------------------------------------------------------------------------------------------
//
//  err.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxError Class
//
//-------------------------------------------------------------------------------------------------

#include <cx/base/err.h>


//-------------------------------------------------------------------------
// CxError::buildOSErrorString
//
//-------------------------------------------------------------------------
CxString
CxError::buildOSErrorString( CxString funcName )
{
    CxString s = funcName +
        CxString(":(OS_ERROR=") +
        CxString( strerror( errno ) ) +
        CxString(")");

    return( s );
}   
