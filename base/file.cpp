//-------------------------------------------------------------------------------------------------
//
//  file.cpp
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
#include <cx/base/file.h>



//-------------------------------------------------------------------------
// CxFile::CxFile
//
//-------------------------------------------------------------------------
CxFile::CxFile( void )
: _impl( new CxFileImpl )
{
}


//-------------------------------------------------------------------------
// CxFile::CxFile
//
//-------------------------------------------------------------------------
CxFile::CxFile( CxFileImpl *impl_ )
: _impl( impl_ )
{
}

        
//-------------------------------------------------------------------------
// CxFile::~CxFile
//
//-------------------------------------------------------------------------
CxFile::~CxFile( void )  
{ 
}


//-------------------------------------------------------------------------
// CxFile::CxFile
//
//-------------------------------------------------------------------------
CxFile::CxFile( const CxFile& file_ )
{
    if (this != &file_) {
        _impl = file_._impl;
    }
}


//-------------------------------------------------------------------------
// CxFile::operator=
//
//-------------------------------------------------------------------------
CxFile&
CxFile::operator=(const CxFile& file_)
{
    if (this != &file_) {
        _impl = file_._impl;
    }
    return(*this);
}

//-------------------------------------------------------------------------
// CxFile::tempName
//
//-------------------------------------------------------------------------
/*static*/
CxString
CxFile::tempName(const char* prefix)
{
	char buffer[1000];

#ifdef _SUNOS_
	tmpnam(buffer);
#else
	// mkstemp requires a template with XXXXXX pattern
	if (prefix != NULL) {
		sprintf(buffer, "/tmp/%s", prefix);
	} else {
		strcpy(buffer, "/tmp/cxfile_XXXXXX");
	}
	int fd = mkstemp(buffer);
	if (fd != -1) {
		::close(fd);  // mkstemp opens the file, close it
	}
#endif

	return(buffer);
}


//-------------------------------------------------------------------------
// CxFile::tildaExpansion
//
//-------------------------------------------------------------------------
/*static*/
CxString
CxFile::tildaExpansion(CxString path)
{
	CxString originalPath = path;

	path.stripLeading(" \t");
	path.stripTrailing(" \t\n");

	if (path.length()) {

		if (path.firstChar('~')==0) 
		{
			const char *homeDir = getenv("HOME");
			path.stripLeading("~");

		   	if (!homeDir) {
    		   	struct passwd* pwd = getpwuid(getuid());
       			if (pwd) {
          			homeDir = pwd->pw_dir;
   				}
			}

			if (homeDir!=NULL) {
				CxString homeDirString(homeDir);
				path = homeDirString + path;
				return(path);
			} else {
				return( originalPath );
			}
		} else {
			return(originalPath);
		}
	}

	return( originalPath );
}


//-------------------------------------------------------------------------
// CxFile::fd
//
//-------------------------------------------------------------------------
FILE *
CxFile::fd(void)
{
    return( _impl->fd() );
}


//-------------------------------------------------------------------------
// CxFile::getStat
//
//-------------------------------------------------------------------------
struct stat
CxFile::getStat()
{
	return( _impl->getStat() );
}


//-------------------------------------------------------------------------
// CxFile::setFlushMode
//
//-------------------------------------------------------------------------
void
CxFile::setFlushMode( flushMode f )
{
	if (f == CxFile::DONT_FLUSH) {
		_impl->setFlushMode( CxFileImpl::DONT_FLUSH );
	} else 

	if (f == CxFile::ALWAYS_FLUSH) {
		_impl->setFlushMode( CxFileImpl::ALWAYS_FLUSH );
	}
}


//-------------------------------------------------------------------------
// CxFile::path
//
//-------------------------------------------------------------------------
CxString
CxFile::path(void)
{
	return( _impl->path( ) );
}


//-------------------------------------------------------------------------
// CxFile::seek
//
//-------------------------------------------------------------------------
void
CxFile::seek( long offset, int origin)
{    
	_impl->seek( offset, origin );
}


//-------------------------------------------------------------------------
// CxFile::tell
//
//-------------------------------------------------------------------------
long
CxFile::tell( void )
{
	return( _impl->tell( ) );	
}


//-------------------------------------------------------------------------
// CxFile::open
//
//-------------------------------------------------------------------------
int
CxFile::open( CxString path, CxString mode )
{
	return( _impl->open( path, mode ) );
}


//-------------------------------------------------------------------------
// CxFile::attach
//
//-------------------------------------------------------------------------
int
CxFile::attach( FILE *fd_, CxString mode )
{
	return( _impl->attach( fd_, mode ) );
}

//-------------------------------------------------------------------------
// CxFile::fread 
//
//-------------------------------------------------------------------------
size_t
CxFile::fread( void *ptr_, size_t s_, size_t num_ )
{
	return( _impl->fread( ptr_, s_, num_ ) );
}

CxString
CxFile::fread( size_t s_)
{
    return( _impl->fread( s_ ) );
}

//-------------------------------------------------------------------------
// CxFile::fwrite
//
//-------------------------------------------------------------------------
size_t
CxFile::fwrite( void *ptr_, size_t s_, size_t num_ )
{
	return( _impl->fwrite( ptr_, s_, num_ ) );
}


//-------------------------------------------------------------------------
// CxFile::getch
//
//-------------------------------------------------------------------------
CxString
CxFile::getch( fType failType )
{
	if (failType==CxFile::ALLOW_EOF) {
		return( _impl->getch( CxFileImpl::ALLOW_EOF) );
	} else

//	if (failType==CxFile::NO_EOF) {
		return( _impl->getch( CxFileImpl::NO_EOF ) );
//	}
}


//-------------------------------------------------------------------------
// CxFile::getUntil
//
//-------------------------------------------------------------------------
CxString
CxFile::getUntil( char c )
{
	return( _impl->getUntil( c ) );
}


//-------------------------------------------------------------------------
// CxFile::eof
//
//-------------------------------------------------------------------------
int
CxFile::eof(void)
{
	return( _impl->eof() );
}


//-------------------------------------------------------------------------
// CxFile::close
//
//-------------------------------------------------------------------------
void
CxFile::close( void )
{
	_impl->close();
}


//-------------------------------------------------------------------------
// CxFile::isOpen
//
//-------------------------------------------------------------------------
int
CxFile::isOpen( void )
{
	return(_impl->isOpen());
}


//-------------------------------------------------------------------------
// CxFile::printf
//
//-------------------------------------------------------------------------
void
CxFile::printf( const char *format, ... )
{
	va_list marker;
	va_start (marker, format );
		
	_impl->printf( format, marker );
        
	va_end(marker);
}
