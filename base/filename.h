//-------------------------------------------------------------------------------------------------
//
//  filename.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxFileName Class
//
//-------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <iostream>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include <cx/base/string.h>
#include <cx/base/slist.h>
#include <cx/base/rule.h>
#include <cx/base/exception.h>
#include <cx/base/file.h>
#include <cx/base/star.h>


#ifndef _CXFILENAME_
#define _CXFILENAME_

//-------------------------------------------------------------------------
// class CxFileName
//
//------------------------------------------------------------------------- 
class CxFileName
{
  public:
    
    CxFileName( void );
    // constructor
    
    CxFileName( CxString name );
    // constructor
    
    CxFileName( const CxFileName& f);
    // copy constructor
    
    ~CxFileName(void);
    // destructor
    
    CxFileName& operator=( const CxFileName& f);
    // assignment operator
    
    CxString name(void);
    // get the name part of the filename
    
    void setName( CxString n);
    // set the name part of the filename
    
    CxString extension(void);
    // get the extension part of the filename
    
    void setExtension( CxString e);
    // set the extension part of the filename
    
    CxString fullName(void);
    // get the full name plus extension
    
  private:
    
    friend std::ostream& operator<<(std::ostream& str, const CxFileName& cxfn );
    
    CxString _name;
    CxString _extension;
    int      _dotFound;
};

#endif
