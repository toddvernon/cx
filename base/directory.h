/***************************************************************************
 *
 *  directory.h
 *
 *  CxDirectory Class
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

#include <math.h>

#include <cx/base/string.h>
#include <cx/base/slist.h>
#include <cx/base/rule.h>
#include <cx/base/exception.h>
#include <cx/base/file.h>
#include <cx/base/star.h>
#include <cx/base/filename.h>


#pragma GCC diagnostic ignored "-Wwrite-strings"

#ifndef _CXDIRECTORY_
#define _CXDIRECTORY_


//-------------------------------------------------------------------------
// class CxDirectory
//
//------------------------------------------------------------------------- 
class CxDirectory
{
  public:
    
    enum fileType { CxFILE, CxDIRECTORY, CxUNKNOWN, CxINVALID };
    // type of target
    
    enum pathType { CxABSOLUTE, CxRELATIVE };
    // type of path
    
    CxDirectory( void );
    // constructor
    
    CxDirectory( CxString path, CxMatchTemplate temp="*");
    // constructor
    
    CxDirectory( const CxDirectory& d);
    // copy constructor
    
    CxDirectory& operator=( const CxDirectory& d );
    // assignment operator
    
    CxString directoryPath( void );
    // return the path absent the filename
    
    CxSList< CxFileName > fileNameList( void );
    // return the list of filename objects (no path)
    
    CxSList< CxString > filePathList( void );
    // return the list of files with complete path
    
    int pathIsAbsolute(void);
    // returns true if absolute path
    
    int pathIsRelative(void);
    // returns true if relative path
    
    int pathIsFile(void);
    // returns true if entity is a file
    
    int pathIsDirectory(void);
    // returns true of entity is a directory
    
    int pathIsInvalid(void);
    // returns true if the path doesn't point to a valid fs object
    
    int filterBy( CxMatchTemplate t );
    // filter the file list by a wildcard template
    
    static fileType getFileType(CxString path);
    // return of the path is a file or directory
    
  private:
    
    fileType            _fileType;
    pathType            _pathType;
    CxSList<CxString>   _segmentList;
    CxSList<CxFileName> _fileList;
};


#endif
