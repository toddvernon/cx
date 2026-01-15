/***************************************************************************
 *
 *  logfile.cpp
 *
 *  CxLogFile Class
 *
 ***************************************************************************
 *
 * (c) Copyright 1999,2023 T.Vernon
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

#if defined(_LINUX_) || defined(_SOLARIS6_) || defined(_IRIX6_)
#include <sys/timeb.h>
#include <pthread.h>
#endif

#include <sys/stat.h>
#include <cx/base/string.h>
#include <cx/base/file.h>
#include <cx/log/logfile.h>



CxLogFile * CxLogFile::_globalLog  = NULL;



//-------------------------------------------------------------------------
// CxLogFile::CxLogFile
//
//-------------------------------------------------------------------------
CxLogFile::CxLogFile( void )
: _impl( new CxFileImpl )
{
	CxPropertyList propList;

    // if no one's set the global log file yet, set it to this
    if (_globalLog == NULL) _globalLog = this;

	// set default properties
	propList = setDefaults();

	// init
    initialize( propList );
}



//-------------------------------------------------------------------------
// CxLogFile::
//
//-------------------------------------------------------------------------
CxLogFile::CxLogFile( CxPropertyList initProps )
: _impl( new CxFileImpl )
{
	CxPropertyList propList;

    // if noone's set the global log file yet, set it to this
    if (_globalLog == NULL) _globalLog = this;

	// set the defaults
	propList = setDefaults( );

	// init with standard stuff
	initialize( propList );

	// init the way the caller wants
    initialize( initProps );
}


//-------------------------------------------------------------------------
// CxLogFile::CxLogFile
//
//-------------------------------------------------------------------------
CxLogFile::CxLogFile( CxFileImpl *impl_ )
: _impl( impl_ )
{
    // if no one's set the global log file yet, set it to this
    if (_globalLog == NULL) _globalLog = this;

	// set default properties
	setDefaults();
}

        
//-------------------------------------------------------------------------
// CxLogFile::~CxLogFile
//
//-------------------------------------------------------------------------
CxLogFile::~CxLogFile( void )  
{ 
}


//-------------------------------------------------------------------------
// CxLogFile::CxLogFile
//
//-------------------------------------------------------------------------
CxLogFile::CxLogFile( const CxLogFile& file_ )
{
    if (this != &file_) {

        _impl = file_._impl;

		logFlags[CX_INFO]   = file_.logFlags[CX_INFO];
    	logFlags[CX_ERROR]  = file_.logFlags[CX_ERROR];
		logFlags[CX_DATA]   = file_.logFlags[CX_DATA];
    	logFlags[CX_DETAIL] = file_.logFlags[CX_DETAIL];
	
		_maxFileSize   = file_._maxFileSize;
    	_threadFlag    = file_._threadFlag;
    	_pidFlag 	   = file_._pidFlag;
    	_timeStampFlag = file_._timeStampFlag;
		_lineNoFlag    = file_._lineNoFlag;
    }
}


//-------------------------------------------------------------------------
// CxLogFile::operator=
//
//-------------------------------------------------------------------------
CxLogFile&
CxLogFile::operator=(const CxLogFile& file_)
{
    if (this != &file_) {

        _impl = file_._impl;

		logFlags[CX_INFO]   = file_.logFlags[CX_INFO];
    	logFlags[CX_ERROR]  = file_.logFlags[CX_ERROR];
		logFlags[CX_DATA]   = file_.logFlags[CX_DATA];
    	logFlags[CX_DETAIL] = file_.logFlags[CX_DETAIL];
	
		_maxFileSize   = file_._maxFileSize;
    	_threadFlag    = file_._threadFlag;
    	_pidFlag 	   = file_._pidFlag;
    	_timeStampFlag = file_._timeStampFlag;
		_lineNoFlag    = file_._lineNoFlag;
    }

    return(*this);
}



//-------------------------------------------------------------------------
// CxLogFile::
//
//-------------------------------------------------------------------------
CxPropertyList
CxLogFile::setDefaults( void )
{
	CxPropertyList defaultPropList;

	defaultPropList.set("LOG_INFO=1");
	defaultPropList.set("LOG_DATA=1");
	defaultPropList.set("LOG_DETAIL=1");
	defaultPropList.set("LOG_ERROR=1");
	defaultPropList.set("LOG_PID=1");
	defaultPropList.set("LOG_TIMESTAMP=1");
	defaultPropList.set("LOG_LINENUMBERS=0");
	defaultPropList.set("LOG_ALWAYSFLUSH=1");
	defaultPropList.set("FILESIZE=1000000");

#if defined(_SUNOS_)
	defaultPropList.set("LOG_THREADID=0");
#else
	defaultPropList.set("LOG_THREADID=1");
#endif

	logFlags[CX_INFO]   = 1;
    logFlags[CX_ERROR]  = 1;
	logFlags[CX_DATA]   = 0;
    logFlags[CX_DETAIL] = 0;

    return( defaultPropList );
}


//-------------------------------------------------------------------------
// CxLogFile::
//
//-------------------------------------------------------------------------
void 
CxLogFile::initialize( CxPropertyList logProps )
{
    if ( logProps.has("LOG_INFO") ) {
        if ( logProps.get("LOG_INFO") == "1" ) {
            logFlags[CX_INFO] = 1;
        } else {
            logFlags[CX_INFO] = 0;
        }
    }
    if ( logProps.has("LOG_ERROR") ) {
        if ( logProps.get("LOG_ERROR") == "1" ) {
            logFlags[CX_ERROR] = 1;
        } else {
            logFlags[CX_ERROR] = 0;
        }
    }
    if ( logProps.has("LOG_DATA") ) {
        if ( logProps.get("LOG_DATA") == "1" ) {
            logFlags[CX_DATA] = 1;
        } else {
            logFlags[CX_DATA] = 0;
        }
    }
    if ( logProps.has("LOG_DETAIL") ) {
        if ( logProps.get("LOG_DETAIL") == "1" ) {
            logFlags[CX_DETAIL] = 1;
        } else {
            logFlags[CX_DETAIL] = 0;
        }
    }

    if ( logProps.has("FILESIZE") ) {
        _maxFileSize = atol( logProps.get("FILESIZE").data() );
    }


    if ( logProps.has("LOG_THREADID") ) {
        if ( logProps.get("LOG_THREADID") == "1" ) {

#if defined(_SUNOS_)
            _threadFlag = INCLUDE_THREAD_ID;
#else
            _threadFlag = NO_THREAD_ID;
#endif

        } else {
            _threadFlag = NO_THREAD_ID;
        }
    }


    if ( logProps.has("LOG_PID") ) {
        if ( logProps.get("LOG_PID") == "1" ) {
            _pidFlag = INCLUDE_PID;
        } else {
            _pidFlag = NO_PID;
        }
    }

    if ( logProps.has("LOG_TIMESTAMP") ) {
        if ( logProps.get("LOG_TIMESTAMP") == "1" ) {
            _timeStampFlag = INCLUDE_TIMESTAMP;
        }else {
            _timeStampFlag = NO_TIMESTAMP;
        }
    }

    if ( logProps.has("LOG_LINENUMBERS") ) {
        if ( logProps.get("LOG_LINENUMBERS") == "1" ) {
            _lineNoFlag = INCLUDE_LINE_NUMBERS;
        }else {
            _lineNoFlag = NO_LINE_NUMBERS;
        }
    }

    if ( logProps.has("LOG_ALWAYSFLUSH") ) {
        if ( logProps.get("LOG_ALWAYSFLUSH") == "1" ) {
            _impl->setFlushMode( CxFileImpl::ALWAYS_FLUSH );
        } else {
            _impl->setFlushMode( CxFileImpl::DONT_FLUSH );
        }
    }
}


//-------------------------------------------------------------------------
// CxLogFile::
//
//-------------------------------------------------------------------------
void
CxLogFile::setThreadMode( threadMode tm)
{
#if !defined(_SUNOS_)
    _threadFlag = tm;
#endif
}

//-------------------------------------------------------------------------
// CxLogFile::
//
//-------------------------------------------------------------------------
void
CxLogFile::setPIDMode( pidMode pm )
{
	_pidFlag = pm;
}


//-------------------------------------------------------------------------
// CxLogFile::
//
//-------------------------------------------------------------------------
void
CxLogFile::setTimeStampMode( timeMode ts)
{
    _timeStampFlag = ts;
}


//-------------------------------------------------------------------------
// CxLogFile::
//
//-------------------------------------------------------------------------
void
CxLogFile::setLineNoMode( lineNoMode li )
{
    _lineNoFlag = li;
}


//-------------------------------------------------------------------------
// CxLogFile::
//
//-------------------------------------------------------------------------
void
CxLogFile::setMaxSize( int size )
{
    _maxFileSize = size;
}

//-------------------------------------------------------------------------
// CxLogFile::path
//
//-------------------------------------------------------------------------
CxString
CxLogFile::path(void)
{
	return( _impl->path( ) );
}


//-------------------------------------------------------------------------
// CxLogFile::open
//
//-------------------------------------------------------------------------
int
CxLogFile::open( CxString path )
{
	int s = _impl->open( path, CxString("w") );

	_impl->setFlushMode( CxFileImpl::ALWAYS_FLUSH );

	return( s );
}

//-------------------------------------------------------------------------
// CxLogFile::isOpen
//
//-------------------------------------------------------------------------
int
CxLogFile::isOpen( void )
{
    return(_impl->isOpen() );
}

//-------------------------------------------------------------------------
// CxLogFile::open
//
//-------------------------------------------------------------------------
int
CxLogFile::open( CxString path, CxString mode )
{
	int s = _impl->open( path, mode );

	_impl->setFlushMode( CxFileImpl::ALWAYS_FLUSH );

	return( s );
}


//-------------------------------------------------------------------------
// CxLogFile::close
//
//-------------------------------------------------------------------------
void
CxLogFile::close( void )
{
	_impl->close();
}



//-------------------------------------------------------------------------
// CxLogFile::
//
//-------------------------------------------------------------------------
void
CxLogFile::printf( logType type, const char *file, int line, const char *format, ... )
{
	va_list marker;
    va_start (marker, format );

	//---------------------------------------------------------------------
    // see if we're logging this type of data
	//---------------------------------------------------------------------
    if (!logFlags[type]) return;

	//---------------------------------------------------------------------
	// print the type of log entry
	//---------------------------------------------------------------------

    switch ( type ) {

        case CxLogFile::CX_INFO: 
	        _impl->printf( "\n[  INFO" );
            break;
    case CxLogFile::CX_ERROR:
            _impl->printf( "\n[ ERROR" );
            break;
    case CxLogFile::CX_DATA:
            _impl->printf( "\n[  DATA" );
            break;
    case CxLogFile::CX_DETAIL:
            _impl->printf( "\n[DETAIL" );
            break;

    }

	//---------------------------------------------------------------------
	// include process pid
	//---------------------------------------------------------------------
  	if (_pidFlag == CxLogFile::INCLUDE_PID) {
		_impl->printf(", %3ld", ::getpid() );
	}

	//---------------------------------------------------------------------
	// include thread id
	//---------------------------------------------------------------------
	if (_threadFlag == CxLogFile::INCLUDE_THREAD_ID) {

#if !defined(_SUNOS_)
		_impl->printf(", %3ld", ::pthread_self() );
#endif
    }


	//---------------------------------------------------------------------
	// include timestamp
	//---------------------------------------------------------------------
	if (_timeStampFlag == CxLogFile::INCLUDE_TIMESTAMP) {

		char buffer[100];

		struct timeval tp;
		struct timezone tzp;
		gettimeofday( &tp, &tzp);
		CxTime t( tp.tv_sec );
        
#ifdef _OSX_
		sprintf(buffer, "%d", tp.tv_usec);
#else
        sprintf(buffer, "%ld", tp.tv_usec);
#endif



/** maybe irix 6
		struct timeb tstruct;
        ftime( &tstruct );            
 	    CxTime t( tstruct.time );
		sprintf(buffer, "%u", tstruct.millitm );
**/

		CxString smallBits = buffer;
		while (smallBits.length()<6) smallBits+="0";

		CxString timeString = t.asFormattedString("%j-%H:%M:%S");
		_impl->printf( ", %s:%s", timeString.data(), smallBits.data() );
   	}


	//---------------------------------------------------------------------
	// include file and line number
	//---------------------------------------------------------------------
	if (_lineNoFlag == CxLogFile::INCLUDE_LINE_NUMBERS) {
		_impl->printf(" ,%18s:%4i", file, line);
    }

	//---------------------------------------------------------------------
	// include user text
	//---------------------------------------------------------------------
	_impl->printf("] ");
	_impl->printf( format, marker );


    va_end(marker);
}


//-------------------------------------------------------------------------
// CxLogFile::
//
//-------------------------------------------------------------------------
void CxLogFile::setGlobalLogFile(CxLogFile * pFile) 
{
    _globalLog = pFile;
}


//-------------------------------------------------------------------------
// CxLogFile::
//
//-------------------------------------------------------------------------
CxLogFile * CxLogFile::getGlobalLogFile(void) 
{
    if (_globalLog == NULL) _globalLog = new CxLogFile();
    return _globalLog;
}
