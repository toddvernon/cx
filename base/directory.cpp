/***************************************************************************
 *
 *  directory.cpp
 *
 *  CxDirectory Class
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


#include <cx/base/directory.h>


//-------------------------------------------------------------------------
// <constructor>
//
//-------------------------------------------------------------------------
CxDirectory::CxDirectory(void)
: _fileType( CxUNKNOWN ), _pathType(CxRELATIVE)
{
}


//-------------------------------------------------------------------------
// <constructor>
//
//-------------------------------------------------------------------------
CxDirectory::CxDirectory( const CxDirectory& d)
{
    if (&d != this) {
        _fileType     = d._fileType;
        _pathType     = d._pathType;
        _segmentList  = d._segmentList;
        _fileList     = d._fileList;
    }
}


//-------------------------------------------------------------------------
// pathValid
//
//-------------------------------------------------------------------------
/*static*/
CxDirectory::fileType
CxDirectory::getFileType(CxString path)
{
    // stat the file to see if its a CxDirectoryectory or a file
    struct stat s;
    if( stat(path.data(), &s) != 0 )
    {
        return(CxINVALID);
    }
    
    if( s.st_mode & S_IFDIR ) {
        return(CxDIRECTORY);
    }
    
    if( s.st_mode & S_IFREG ) {
        return(CxFILE);
        
    }
    
    return(CxUNKNOWN);
}

//-------------------------------------------------------------------------
// <constructor>
//
//-------------------------------------------------------------------------
CxDirectory::CxDirectory( CxString path, CxMatchTemplate temp )
{
    
    _fileType = CxDirectory::getFileType( path );
    
    path.stripLeading(" \t");
    path.stripTrailing(" \n\r\t");
    
    if (path.firstChar('/')==0) {
        _pathType  = CxABSOLUTE;
        
    } else {
        _pathType = CxRELATIVE;
    }
    path.stripLeading("/");
    
    
    CxString seg = path.nextToken("/");
    while (seg.length()) {
        _segmentList.append( seg );
        seg = path.nextToken("/");
    }
    
    // if the path is a file, strip the last segment from the
    // segment list and append to the file list
    
    if ((_fileType == CxFILE) || (_fileType==CxINVALID)) {
        
        if (_segmentList.entries()!=0) {
            CxString fileName = _segmentList.at( _segmentList.entries()-1 );
            _segmentList.removeAt( _segmentList.entries()-1 );
            _fileList.append( CxFileName(fileName) );
        }
        
    } else if (_fileType == CxDIRECTORY) {
        
        DIR *dptr = opendir( directoryPath().data() );
        if (dptr == NULL) {
            std::cout << "ERROR OPENING DIRECTORY 1" << std::endl;
            return;
        }
        
        int done = FALSE;
        struct dirent *dir_entry = NULL;
        while (!done) {
            
            // read the delared path
            dir_entry = readdir( dptr );
            if (dir_entry != NULL) {
                
                if (temp.test( dir_entry->d_name ) ) {
                    _fileList.append( CxFileName( dir_entry->d_name) );
                }
                
            } else {
                done = TRUE;
            }
        }
        
        closedir(dptr);
    }
}

//-------------------------------------------------------------------------
// CxDirectory::operator=
//
//-------------------------------------------------------------------------
CxDirectory&
CxDirectory::operator=( const CxDirectory& d )
{
    if (&d != this) {
        _fileType     = d._fileType;
        _pathType     = d._pathType;
        _segmentList  = d._segmentList;
        _fileList     = d._fileList;
    }
    return( *this );
}


//-------------------------------------------------------------------------
// CxDirectory::filterBy
//
//-------------------------------------------------------------------------
int
CxDirectory::filterBy( CxMatchTemplate t )
{
    CxSList<CxFileName> keeperList;
    
    for (int c=0; c<_fileList.entries(); c++) {
        if ( t.test( _fileList.at(c).fullName() ) ) {
            keeperList.append(_fileList.at(c).fullName() );
        }
    }
    
    _fileList.clear();
    _fileList = keeperList;
    
    return( _fileList.entries() );
}


//-------------------------------------------------------------------------
// CxDirectory::directoryPath
//
//-------------------------------------------------------------------------
CxString
CxDirectory::directoryPath( void )
{
    CxString path;
    
    if (_pathType == CxABSOLUTE) {
        path = "/";
    }
    
    for (int c=0; c<_segmentList.entries(); c++) {
        CxString segment = _segmentList.at(c);
        path = path + segment + "/";
    }
    
    return(path);
}

//-------------------------------------------------------------------------
// CxDirectory::pathIsInvalid
//
//-------------------------------------------------------------------------
int
CxDirectory::pathIsInvalid(void)
{
    if (_fileType == CxINVALID) return(TRUE);
    return(FALSE);
}


//-------------------------------------------------------------------------
// CxDirectory::pathIsFile
//
//-------------------------------------------------------------------------
int
CxDirectory::pathIsFile(void)
{
    if (_fileType == CxFILE) return(TRUE);
    return(FALSE);
}


//-------------------------------------------------------------------------
// CxDirectory::pathIsDirectory
//
//-------------------------------------------------------------------------
int
CxDirectory::pathIsDirectory(void)
{
    if (_fileType == CxDIRECTORY) return(TRUE);
    return( FALSE );
}


//-------------------------------------------------------------------------
// CxDirectory::pathIsAbsolute
//
//-------------------------------------------------------------------------
int
CxDirectory::pathIsAbsolute(void)
{
    if (_pathType == CxABSOLUTE) return( TRUE );
    return( FALSE );
}


//-------------------------------------------------------------------------
// CxDirectory::pathIsRelative
//
//-------------------------------------------------------------------------
int
CxDirectory::pathIsRelative(void)
{
    if (_pathType == CxRELATIVE) return( TRUE );
    return( FALSE );
}


//-------------------------------------------------------------------------
// CxDirectory::filePathList
//
//-------------------------------------------------------------------------
CxSList< CxString >
CxDirectory::filePathList( void )
{
    CxSList< CxString > list;
    
    for (int c=0; c<_fileList.entries(); c++) {
        
        CxFileName fn = _fileList.at(c);
        CxString filePath = directoryPath() + fn.fullName();
        list.append( filePath );
    }
    
    return( list );
}


//-------------------------------------------------------------------------
// CxDirectory::fileNameList
//
//-------------------------------------------------------------------------
CxSList< CxFileName >
CxDirectory::fileNameList( void )
{
    return( _fileList );
}

