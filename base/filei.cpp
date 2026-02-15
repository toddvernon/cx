//-------------------------------------------------------------------------------------------------
//
//  filei.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxFileImpl Class
//
//-------------------------------------------------------------------------------------------------

#include <cx/base/filei.h>


//-------------------------------------------------------------------------
// CxFileImpl::CxFileImpl
//
//-------------------------------------------------------------------------
CxFileImpl::CxFileImpl(void)
: _flushFlag( ALWAYS_FLUSH ), _fd( NULL ), _open(FALSE)
{

}


//-------------------------------------------------------------------------
// CxFileImpl::~CxFileImplImpl
//
//-------------------------------------------------------------------------
CxFileImpl::~CxFileImpl(void)
{
    if (_open) close();
}


//-------------------------------------------------------------------------
// CxFileImpl::fd
//
//-------------------------------------------------------------------------
FILE *
CxFileImpl::fd(void)
{
    return(_fd);
}


//-------------------------------------------------------------------------
// CxFileImpl::getStat
//
//-------------------------------------------------------------------------
struct stat
CxFileImpl::getStat()
{
    if (!_open) throw( CxFileException("CxFile::getStat():file not open" ) );         
    struct stat myst;
    stat( _path.data() , &myst );
    return myst;
}


//-------------------------------------------------------------------------
// CxFileImpl::setFlushMode
//
//-------------------------------------------------------------------------
void
CxFileImpl::setFlushMode( flushMode f)
{
    _flushFlag = f;
}


//-------------------------------------------------------------------------
// CxFileImpl::path
//
//-------------------------------------------------------------------------
CxString
CxFileImpl::path(void)
{
    return(_path);
}


//-------------------------------------------------------------------------
// CxFileImpl::seek
//
//-------------------------------------------------------------------------
void
CxFileImpl::seek( long offset, int origin)
{    
    if (!_open) throw( CxFileException("CxFile::seek():file not open" ) );
    if (_fd) fseek( _fd, offset, origin );
}


//-------------------------------------------------------------------------
// CxFileImpl::tell
//
//-------------------------------------------------------------------------
long
CxFileImpl::tell( void )
{
    if (!_open) throw( CxFileException("CxFile::tell():file not open" ) );
    if (_fd) return(ftell( _fd ));
    return(0);
}


//-------------------------------------------------------------------------
// CxFileImpl::open
//
//-------------------------------------------------------------------------
int
CxFileImpl::open( CxString path, CxString mode )
{
    if (_open) throw( CxFileException("CxFile::open():file already open" ) );

   
    _fd = fopen( path.data(), mode.data() );
   
    
    if (_fd) {

        _open = TRUE;
        _path = path;
        _mode = mode;
	
        return( 1 );
    } 

    _open = TRUE;
    _path = path;
    _mode = mode;
  
    return(0);
}


//-------------------------------------------------------------------------
// CxFileImpl::attach
//
//-------------------------------------------------------------------------
int
CxFileImpl::attach( FILE *fd_, CxString mode )
{
    if (_open) throw( CxFileException("CxFile::open():file already open" ) );
    _open = TRUE;
    _path = "";
    _mode = mode;
    _fd   = fd_;
    if (_fd) return(1);
    return(0);
}

//-------------------------------------------------------------------------
// CxFileImpl::fread 
//
//-------------------------------------------------------------------------
size_t
CxFileImpl::fread( void *ptr_, size_t s_, size_t num_ )
{
	return( ::fread( ptr_, s_, num_, _fd ) );
}

CxString
CxFileImpl::fread( size_t s_ )
{
    if (eof()) {
        printf("EOF\n");
		CxString s;
        return(s);
    }
    char buffer[s_+1];
    buffer[s_] = (char) 0;
    int bytesRead = ::fread( buffer, 1, s_, _fd );
    buffer[bytesRead] = (char) NULL;
    return(buffer);
}

//-------------------------------------------------------------------------
// CxFileImpl::fwrite
//
//-------------------------------------------------------------------------
size_t
CxFileImpl::fwrite( void *ptr_, size_t s_, size_t num_ )
{
	return( ::fwrite( ptr_, s_, num_, _fd ) );
}


//-------------------------------------------------------------------------
// CxFileImpl::getch
//
//-------------------------------------------------------------------------
CxString
CxFileImpl::getch( fType failType )
{
    char c[2];

    if (!_open) throw( CxFileException("CxFile::getc():file not open" ) );

    // Get a character
    c[0] = fgetc(_fd);
    c[1] = (char) NULL;

    if (feof(_fd)) {
        if (failType == CxFileImpl::NO_EOF) clearerr(_fd);
        return CxString("");
    }

    return(CxString(c));
}


//-------------------------------------------------------------------------
// CxFileImpl::getUntil
//
//-------------------------------------------------------------------------
CxString
CxFileImpl::getUntil( char c )
{
    CxString line;
    CxString character;

    if (!_open) throw( CxFileException("CxFile::getUntil():file not open" ) );

    do {
        character =  CxFileImpl::getch( CxFileImpl::ALLOW_EOF );

        if ( character.length() ) {

            if (character == CxString( c ) ) {
                return(line);
            } else {
               line += character;
            }
        }

    } while ( ! CxFileImpl::eof() );

    return(line);
}


//-------------------------------------------------------------------------
// CxFileImpl::eof
//
//-------------------------------------------------------------------------
int
CxFileImpl::eof(void)
{
    if (!_open) throw( CxFileException("CxFile::eof():file not open" ) );
    return(feof(_fd));
}


//-------------------------------------------------------------------------
// CxFileImpl::close
//
//-------------------------------------------------------------------------
void
CxFileImpl::close( void )
{
    if (!_open) throw( CxFileException("CxFile::close():file not open" ) );
    if (_fd) fclose(_fd);
    _fd   = NULL;
    _open = FALSE;

}


//-------------------------------------------------------------------------
// CxFileImpl::printf
//
//-------------------------------------------------------------------------
void
CxFileImpl::printf( const char *format, va_list& marker )
{
    if (!_open) throw( CxFileException("CxFile::printf():file not open" ) );

    if (_fd) {

        vfprintf( _fd, format, marker );

        if (_flushFlag == CxFileImpl::ALWAYS_FLUSH) fflush(_fd);
    }
}


//-------------------------------------------------------------------------
// CxFileImpl::printf
//
//-------------------------------------------------------------------------
void
CxFileImpl::printf( const char *format, ... )
{
    va_list marker;
    va_start (marker, format );
     
    vfprintf( _fd, format, marker );

    va_end(marker);
}

//-------------------------------------------------------------------------
// CxFileImpl::isOpen
//
//-------------------------------------------------------------------------
int
CxFileImpl::isOpen(void)
{
    return( _open );
}
