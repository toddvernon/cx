/***************************************************************************
 *
 *  file.cpp
 *
 *  CxFile Class
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

 


