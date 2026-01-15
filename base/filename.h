/***************************************************************************
 *
 *  filename.h
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



//-------------------------------------------------------------------------
// standard includes
//
//------------------------------------------------------------------------- 

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
