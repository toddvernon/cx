//-------------------------------------------------------------------------------------------------
//
//  fileaccess.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxFile Class
//
//-------------------------------------------------------------------------------------------------

#include <cx/base/string.h>
#include <cx/base/fileaccess.h>

//-------------------------------------------------------------------------
// CxFileAccess::CxFileAccess
//
//-------------------------------------------------------------------------
CxFileAccess::CxFileAccess( void )
{
}


//-------------------------------------------------------------------------
// CxFile::checkStatus
//
//-------------------------------------------------------------------------
/*static*/
CxFileAccess::status
CxFileAccess::checkStatus( CxString path)
{
    int value = access( path.data(), F_OK );
    
    // check if theres a file there
    if (value == 0) {
        
        struct stat path_stat;
        stat(path.data(), &path_stat);

        if (!S_ISREG(path_stat.st_mode) ) {
            return( NOT_A_REGULAR_FILE );
        }
        
        value = access( path.data(), R_OK | W_OK );
        
        //-----------------------------------------------------------------------------------------
        // the file found
        //
        //-----------------------------------------------------------------------------------------
        if (value == 0) {
            return( FOUND_RW );
        }
        
        value = access( path.data(), R_OK );
        if (value == 0) {
            return( FOUND_R );
        }
        
        value = access( path.data(), W_OK );
        if (value == 0) {
            return( FOUND_W );
        }
        
    } else {
        
        CxFile testFile;
        
        int status = testFile.open(path, "w");
        testFile.close();
        
        if (status == 1) {
            remove( path.data() );
            return( NOT_FOUND_W );
        }
        
        return( NOT_FOUND );
    }
    
    return( NOT_FOUND );
    
}

 
