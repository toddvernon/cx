//-------------------------------------------------------------------------------------------------
//
//  time.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxTime Class
//
//-------------------------------------------------------------------------------------------------

#include <cx/base/time.h>


//-------------------------------------------------------------------------
// CxTime::CxTime
//
//-------------------------------------------------------------------------
CxTime::CxTime( void )
{
    ::time(&_epochSeconds);
/*
	struct timeval tv;
	struct timezone tz;

	if (!gettimeofday( &tv, &tz)) {
		printf("timezone minutes west = %d\n", tz.tz_minuteswest);
		printf("timezone dst = %d\n", tz.tz_dsttime);
	}
*/
}


//-------------------------------------------------------------------------
// CxTime::CxTime
//
//-------------------------------------------------------------------------
CxTime::CxTime( time_t s_ )
{
    _epochSeconds = s_;
}


//-------------------------------------------------------------------------
// CxTime::CxTime
//
//-------------------------------------------------------------------------
CxTime::CxTime( int year, int month, int day, int hour, int min, int sec )
{
    struct tm theTime;
    memset(&theTime, 0, sizeof(struct tm));

    theTime.tm_sec  = sec;           // 0 - 59
    theTime.tm_min  = min;           // 0 - 59
    theTime.tm_hour = hour;          // 0 - 23
    theTime.tm_mday = day;           // 1 - 31
    theTime.tm_mon  = month - 1;     // 1 - 12
    theTime.tm_year = year  - 1900;  // 1900 - 2038
    theTime.tm_isdst = -1;           // let mktime determine DST

    _epochSeconds = mktime( &theTime );
}


//-------------------------------------------------------------------------
// CxTime::CxTime
//
//-------------------------------------------------------------------------
CxTime::CxTime( const CxTime& t_ )
{
	if ( &t_ != this ) {
		_epochSeconds = t_._epochSeconds;
    }
}


//-------------------------------------------------------------------------
// CxTime::~CxTime
//
//-------------------------------------------------------------------------
CxTime::~CxTime( void )
{
}


//-------------------------------------------------------------------------
// CxString::operator=
//
//-------------------------------------------------------------------------
CxTime&
CxTime::operator=( const CxTime& t_ )
{
	if ( &t_ != this ) {
        _epochSeconds = t_._epochSeconds;
	}
	return( *this );
}


//-------------------------------------------------------------------------
// CxTime::offsetSeconds
//
//-------------------------------------------------------------------------
void
CxTime::offsetSeconds( int s )
{
    _epochSeconds += s;
}


//-------------------------------------------------------------------------
// CxTime::offsetMinutes
//
//-------------------------------------------------------------------------
void
CxTime::offsetMinutes( int m )
{
    offsetSeconds( m * 60 );
}

    
//-------------------------------------------------------------------------
// CxTime::offsetHours
//
//-------------------------------------------------------------------------
void
CxTime::offsetHours( int h )
{
    offsetMinutes( h * 60 );
}


//-------------------------------------------------------------------------
// CxTime::offsetDays
//
//-------------------------------------------------------------------------
void
CxTime::offsetDays( int d )
{
    offsetHours( d * 24 );
}


//-------------------------------------------------------------------------
// CxTime::offsetMonths
//
//-------------------------------------------------------------------------
void 
CxTime::offsetMonths( int m )
{
    struct tm theTime, *tptr;

    tptr = localtime( &_epochSeconds );

    memcpy( &theTime, tptr, sizeof( struct tm ));

    if ( theTime.tm_mon == 11) {
        theTime.tm_mon = 0;
        theTime.tm_year ++;
    } else {
        theTime.tm_mon++;
    }

    _epochSeconds = mktime( &theTime );
}


//-------------------------------------------------------------------------
// CxTime::year
//
//-------------------------------------------------------------------------
int
CxTime::year( void )
{
    struct tm theTime, *tptr;
    tptr = localtime( &_epochSeconds );
    memcpy( &theTime, tptr, sizeof( struct tm ));
    return( theTime.tm_year + 1900 );
}


//-------------------------------------------------------------------------
// CxTime::month
//
//-------------------------------------------------------------------------
int
CxTime::month( void )
{
    struct tm theTime, *tptr;
    tptr = localtime( &_epochSeconds );
    memcpy( &theTime, tptr, sizeof( struct tm ));
    return( theTime.tm_mon + 1 );
}


//-------------------------------------------------------------------------
// CxTime::day
//
//-------------------------------------------------------------------------
int
CxTime::day( void )
{
    struct tm theTime, *tptr;
    tptr = localtime( &_epochSeconds );
    memcpy( &theTime, tptr, sizeof( struct tm ));
    return( theTime.tm_mday );
}


//-------------------------------------------------------------------------
// CxTime::hour
//
//-------------------------------------------------------------------------
int
CxTime::hour( void )
{
    struct tm theTime, *tptr;
    tptr = localtime( &_epochSeconds );
    memcpy( &theTime, tptr, sizeof( struct tm ));
    return( theTime.tm_hour );
}


//-------------------------------------------------------------------------
// CxTime::minute
//
//-------------------------------------------------------------------------
int
CxTime::minute( void )
{
    struct tm theTime, *tptr;
    tptr = localtime( &_epochSeconds );
    memcpy( &theTime, tptr, sizeof( struct tm ));
    return( theTime.tm_min );
}


//-------------------------------------------------------------------------
// CxTime::second
//
//-------------------------------------------------------------------------
int
CxTime::second( void )
{
    struct tm theTime, *tptr;
    tptr = localtime( &_epochSeconds );
    memcpy( &theTime, tptr, sizeof( struct tm ));
    return( theTime.tm_sec );
}


//-------------------------------------------------------------------------
// CxTime::offsetYears
//
//-------------------------------------------------------------------------
void
CxTime::offsetYears( int years )
{
    struct tm theTime, *tptr;
    tptr = localtime( &_epochSeconds );
    memcpy( &theTime, tptr, sizeof( struct tm ));
    theTime.tm_year++;
    _epochSeconds = mktime( &theTime );
}    


//-------------------------------------------------------------------------
// CxTime::now
//
//-------------------------------------------------------------------------
CxTime
CxTime::now( void )
{
    // get seconds from 1/1/1970
    time_t eSeconds;
    CxTime tt( ::time(&eSeconds) );
    return(tt);
}

/*static*/
CxTime
CxTime::firstSecondOfTheDay( CxTime t)
{
	CxTime tt = CxTime( t.year(), t.month(), t.day(), 0,0,0);
	return(tt);
}

/*static*/
CxTime
CxTime::lastSecondOfTheDay( CxTime t)
{
	CxTime tt = CxTime( t.year(), t.month(), t.day(), 23, 59, 59);
	return(tt);
}


//-------------------------------------------------------------------------
// CxTime::epochSeconds
//
//-------------------------------------------------------------------------
time_t
CxTime::epochSeconds( void ) const
{
    return( _epochSeconds );
}


//-------------------------------------------------------------------------
// CxTime::asString
//
//-------------------------------------------------------------------------
CxString
CxTime::asString( void ) const
{
    struct tm *t = localtime( &_epochSeconds );
    
    CxString st = ctime( &_epochSeconds );

    st = CxString::netNormalize( st );
    
    return( st ); 
}


//-------------------------------------------------------------------------
// CxTime::asFormattedString
//
//-------------------------------------------------------------------------
CxString
CxTime::asFormattedString( CxString format ) const
{
    char buff[2048];
    struct tm t, *tptr;
    tptr = localtime( &_epochSeconds );
    memcpy( &t, tptr, sizeof( struct tm ));
    strftime( buff, 2048, format.data(), &t );
    CxString ts = buff;
    return(ts);
}


//-------------------------------------------------------------------------
// CxTime::monthNumber
//
//-------------------------------------------------------------------------
int
CxTime::monthNumber( CxString monthStr )
{
    CxString t = CxString::toLower( monthStr );

    int i = -1;
 
    if ( t == "jan" ) i = 1;
    if ( t == "feb" ) i = 2;
    if ( t == "mar" ) i = 3;
    if ( t == "apr" ) i = 4;
    if ( t == "may" ) i = 5;
    if ( t == "jun" ) i = 6;
    if ( t == "jul" ) i = 7;
    if ( t == "aug" ) i = 8;
    if ( t == "sep" ) i = 9;
    if ( t == "oct" ) i = 10;
    if ( t == "nov" ) i = 11;
    if ( t == "dec" ) i = 12;
 
   return( i );
}

//-------------------------------------------------------------------------
// static CxTime::epochSecondsFromDateTimeStamp
//
//-------------------------------------------------------------------------
CxTime
epochSecondsFromDateTimeStamp( CxString dateString, CxString timeString )
{

	// dateString=2017-11-11  timeString=00:00:03 

    CxString yearString  = dateString.nextToken(" -");
    CxString monthString = dateString.nextToken("-"); 
    CxString dayString   = dateString.nextToken(" -");
    
    CxString hourString  = timeString.nextToken(" :");
    CxString minString   = timeString.nextToken(" :");
    CxString secString   = timeString.nextToken(" :");
    
    CxTime t(
        yearString.toInt(),
        monthString.toInt(),
        dayString.toInt(),  
        hourString.toInt(), 
        minString.toInt(), 
        secString.toInt());
        
    return( t );
}


/*static*/
CxString
CxTime::formatTimeLength(CxTime startTime, CxTime endTime)
{
    char buffer[1000];
    time_t sec  = endTime.epochSeconds() - startTime.epochSeconds();

    unsigned long hr, min, t;

    hr = sec/3600;
    t = sec%3600;
    min = t/60;
    sec = t%60;

    if ((hr==0) && (min==0) && (sec==0)) {
    sprintf(buffer, "0 seconds");
        return(buffer);
    }

    if ((hr==0) && (min==0)) {
    sprintf(buffer, "%ld seconds", sec);
        return(buffer);
    }

    if (hr==0) {
    sprintf(buffer, "%ld minutes, %ld seconds", min, sec);
    return(buffer);
    }

    sprintf(buffer, "%ld hours, %ld minutes, %ld seconds", hr, min, sec);
    return(buffer);
}


//-------------------------------------------------------------------------
// CxTime::operator==
//
//-------------------------------------------------------------------------
int
CxTime::operator==( const CxTime& t_ )
{
    if ( _epochSeconds == t_._epochSeconds ) return( TRUE );
    return( FALSE );
}


//-------------------------------------------------------------------------
// CxTime::operator<<
//
//-------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& str, const CxTime& cxstring_ )
{
    str << cxstring_.asString();
    return(str);
}
