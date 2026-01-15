/***************************************************************************
 *
 *  date_time.h
 *
 *  CxTime Class
 *
 ***************************************************************************
 *
 * (c) Copyright 2017 T.Vernon
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
#include <sys/types.h>
#include <time.h>
#include <iostream>


#include <cx/base/string.h>


//-------------------------------------------------------------------------
// local includes
//
//-------------------------------------------------------------------------
#include <cx/tz/civil_time.h>
#include <cx/tz/time_zone.h>
#include <cx/tz/date_time.h>



//-------------------------------------------------------------------------
// <constructor>
//
// Constructs a datetime object with the current time in the current
// system timezone
//
//-------------------------------------------------------------------------
CxDateTime::CxDateTime()
{
	zone = cctz::local_time_zone();
	theTimePoint  = std::chrono::system_clock::now();
}

//-------------------------------------------------------------------------
// <constructor>
//
// Constructs a datetime object with the epochSeconds time in the 
// local timezone
//
//-------------------------------------------------------------------------
CxDateTime::CxDateTime(cctz::time_zone tz )
{
	zone = tz;
	theTimePoint  = std::chrono::system_clock::now();
}

//-------------------------------------------------------------------------
// <constructor>
//
// Constructs a datetime object with the epochSeconds time in the 
// local timezone
//
//-------------------------------------------------------------------------

CxDateTime::CxDateTime(time_t epochSecond )
{
    zone = cctz::local_time_zone();
    theTimePoint = std::chrono::system_clock::from_time_t( epochSecond );
}

CxDateTime::CxDateTime(time_t epochSecond, cctz::time_zone tz )
{
	zone = tz;
	theTimePoint = std::chrono::system_clock::from_time_t( epochSecond );
}

//-------------------------------------------------------------------------
// <constructor>
//
// Constructs a datetime object with the epochSeconds time in the 
// local timezone
//
//-------------------------------------------------------------------------

CxDateTime::CxDateTime(std::chrono::system_clock::time_point tp )
{
    zone = cctz::local_time_zone();
    theTimePoint = tp;
}

CxDateTime::CxDateTime(std::chrono::system_clock::time_point tp, cctz::time_zone tz )
{
	zone = tz;
	theTimePoint = tp;
}


//-------------------------------------------------------------------------
// <constructor>
//
// Constructs a datetime object from a time string in the format of
// "2018-01-01 09:01:01" and the specified timezone string in the format of
// "America/New_York".  The zoneName is an optional argument.  If not 
// passed then the local system timezone is used.
//
// see: https://en.wikipedia.org/wiki/List_of_tz_database_time_zones
//
//-------------------------------------------------------------------------

CxDateTime::CxDateTime(CxString timeString)
{
    zone = cctz::local_time_zone();
    
    std::string civil_string = timeString.data();
    std::chrono::system_clock::time_point tp;
    
    const bool ok = cctz::parse("%Y-%m-%d %H:%M:%S", civil_string, zone, &theTimePoint);
    if (!ok) {
        printf("ERROR: can't parse time");
    }
}


CxDateTime::CxDateTime(CxString timeString, cctz::time_zone tz)
{
	zone = tz;

	std::string civil_string = timeString.data();
  	std::chrono::system_clock::time_point tp;

	const bool ok = cctz::parse("%Y-%m-%d %H:%M:%S", civil_string, zone, &theTimePoint);
	if (!ok) {
		printf("ERROR: can't parse time");
	}	
}


//-------------------------------------------------------------------------
// <constructor>
//
// Constructs a datetime object from time parts and the specified timezone 
// The zoneName is an optional argument.  If not passed then the local 
// system timezone is used.
//
// see: https://en.wikipedia.org/wiki/List_of_tz_database_time_zones
//
//-------------------------------------------------------------------------

CxDateTime::CxDateTime(long year, long month, long day, long hour, long minute, long second )
{
    zone = cctz::local_time_zone();
    
    // Converts the input civil time to absolute time
    theTimePoint = cctz::convert(cctz::civil_second(year, month, day, hour, minute, second), zone);
}


CxDateTime::CxDateTime(long year, long month, long day, long hour, long minute, long second, cctz::time_zone destZone)
{
	zone = destZone;

	// Converts the input civil time to absolute time
	theTimePoint = cctz::convert(cctz::civil_second(year, month, day, hour, minute, second), zone);
}


//-------------------------------------------------------------------------
// <copy constructor>
//
// create a new object from an existing object
//
//-------------------------------------------------------------------------
CxDateTime::CxDateTime( const CxDateTime& dt )
{
	if (&dt != this) {
		zone = dt.zone;
        zoneName = dt.zoneName;
		theTimePoint = dt.theTimePoint;
	}
}

    
//-------------------------------------------------------------------------
// operator=
//
// assignment operator
//
//-------------------------------------------------------------------------
CxDateTime& 
CxDateTime::operator=( const CxDateTime& dt )
{
	if (&dt != this) {
		zone = dt.zone;
        zoneName = dt.zoneName;
		theTimePoint = dt.theTimePoint;
	}
	return( *this );
}

//-------------------------------------------------------------------------
// operator==
//
// returns 1 if both times and timezones are the same
//
//-------------------------------------------------------------------------

int
CxDateTime::operator==( const CxDateTime& dt ) 
{
    if (( zone == dt.zone) && (theTimePoint==dt.theTimePoint) && (zoneName == dt.zoneName)) {
		return(1);
	}
	return(0);
}
    

//-------------------------------------------------------------------------
// CxDateTime tzName
//
// returns the objects timezone name.  Note for local default it will
// return /etc/localtime
//
//-------------------------------------------------------------------------
CxString  
CxDateTime::timeZoneName(void)
{
   	std::string str = zone.name();
    CxString ts = str.c_str();
	return(ts);
}

//-------------------------------------------------------------------------
// zoneNametoZone
//
// static member that returns a timezone based on name
//
//-------------------------------------------------------------------------
/*static*/
cctz::time_zone
CxDateTime::zoneNameToZone( CxString zoneName )
{
	cctz::time_zone theZone = cctz::local_time_zone();

	if (zoneName.length()!=0) {
		cctz::load_time_zone(zoneName.data(), &theZone);
	}

	return(theZone);
}


//-------------------------------------------------------------------------
// breakdown
//
// returns the individual date time components given the destTimeZone
// destTimeZone is an option argument.  if not passed then the default
// time zone is used in the constructor is used.
//
//-------------------------------------------------------------------------

int
CxDateTime::breakdown(long *year, long *month, long *day, long *hour, long *minute, long *second )
{
    cctz::civil_second cs = cctz::convert( theTimePoint, zone);
    
    *second   = cs.second();
    *minute   = cs.minute();
    *hour  = cs.hour();
    *day   = cs.day();
    *month = cs.month();
    *year  = cs.year();
    
    return( 1 );
}


int
CxDateTime::breakdown(long *year, long *month, long *day, long *hour, long *minute, long *second, cctz::time_zone destZone)
{
	cctz::civil_second cs = cctz::convert( theTimePoint, destZone);

	*second   = cs.second();
	*minute   = cs.minute();
	*hour  = cs.hour();
	*day   = cs.day();
	*month = cs.month();
	*year  = cs.year();

	return( 1 );
}


//-------------------------------------------------------------------------
// set
//
// sets a new value for the date time object
//
//-------------------------------------------------------------------------
void
CxDateTime::set(int year, int month, int day, int hour, int min, int sec )
{
	// Converts the input civil time in NYC to an absolute time.
	theTimePoint = cctz::convert(cctz::civil_second(year, month, day, hour, min, sec), zone);
}

void
CxDateTime::set(int year, int month, int day, int hour, int min, int sec, cctz::time_zone tz)
{
    zone = tz;
    
    // Converts the input civil time in NYC to an absolute time.
    theTimePoint = cctz::convert(cctz::civil_second(year, month, day, hour, min, sec), zone);
}


//-------------------------------------------------------------------------
// timeZone
//
// returns the object timezone
//
//-------------------------------------------------------------------------
cctz::time_zone
CxDateTime::timeZone(void) const
{
	return( zone );
}


//-------------------------------------------------------------------------
// CxDateTime::offsetYears
//
// add passed value to year +-
//
//-------------------------------------------------------------------------

void
CxDateTime::offsetYears( long y )
{
    offsetYears( y, zone );
}

void
CxDateTime::offsetYears( long y, cctz::time_zone destZone )
{
	long year, month, day, hour, minute, second;
	breakdown( &year, &month, &day, &hour, &minute, &second, destZone );
	year += y;
	set( year, month, day, hour, minute, second, destZone );
}


//-------------------------------------------------------------------------
// CxDateTime::offsetMonths
//
// add passed value to month +-
//
//-------------------------------------------------------------------------
void
CxDateTime::offsetMonths( long m )
{
    offsetMonths( m, zone);
}

void
CxDateTime::offsetMonths( long m, cctz::time_zone destZone)
{
	long year, month, day, hour, minute, second;
	breakdown( &year, &month, &day, &hour, &minute, &second, destZone);
	month += m;
	set( year, month, day, hour, minute, second, destZone );
}


//-------------------------------------------------------------------------
// CxDateTime::offsetDays
//
// add passed value to day +-
//
//-------------------------------------------------------------------------
void
CxDateTime::offsetDays( long d )
{
    offsetDays( d, zone );
}

void
CxDateTime::offsetDays( long d, cctz::time_zone destZone)
{
	long year, month, day, hour, minute, second;
	breakdown( &year, &month, &day, &hour, &minute, &second, destZone);
	day += d;
	set( year, month, day, hour, minute, second, destZone );
}

//-------------------------------------------------------------------------
// CxDateTime::offsetHours
//
// add passed value to hour +-
//
//-------------------------------------------------------------------------
void
CxDateTime::offsetHours( long h )
{
    offsetHours( h, zone);
}

void
CxDateTime::offsetHours( long h, cctz::time_zone destZone)
{
	long year, month, day, hour, minute, second;
	breakdown( &year, &month, &day, &hour, &minute, &second, destZone);
	hour += h;
	set( year, month, day, hour, minute, second, destZone );
}

//-------------------------------------------------------------------------
// CxDateTime::offsetMinutes
//
// add passed value to minute +-
//
//-------------------------------------------------------------------------
void
CxDateTime::offsetMinutes( long m )
{
    offsetMinutes( m, zone);
}

void
CxDateTime::offsetMinutes( long m, cctz::time_zone destZone)
{
	long year, month, day, hour, minute, second;
	breakdown( &year, &month, &day, &hour, &minute, &second, destZone);
	minute += m;
	set( year, month, day, hour, minute, second, destZone );
}


//-------------------------------------------------------------------------
// CxDateTime::offsetSeconds
//
// add passed value to second +-
//
//-------------------------------------------------------------------------

void
CxDateTime::offsetSeconds( long s )
{
    offsetSeconds( s, zone );
}

void
CxDateTime::offsetSeconds( long s, cctz::time_zone destZone)
{
	long year, month, day, hour, minute, second;
	breakdown( &year, &month, &day, &hour, &minute, &second, destZone);
	second += s;
	set( year, month, day, hour, minute, second, destZone );
}

//-------------------------------------------------------------------------
// CxTime::now_tp()
//
// returns a chrono time_point value representing now
//
//-------------------------------------------------------------------------
/*static*/
std::chrono::system_clock::time_point 
CxDateTime::now_tp( void  )
{
	return( std::chrono::system_clock::now() );
}

//-------------------------------------------------------------------------
// CxDateTime::now_epochSeconds
//
// Get epochSeconds from 1970 UTC
//
//-------------------------------------------------------------------------
/*static*/
int
CxDateTime::now_epochSeconds(void)
{
    std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(tp);
    return( now_c );
}


//-------------------------------------------------------------------------
// CxDateTime::epochSeconds
//
// Get epochSeconds from 1970 UTC
//
//-------------------------------------------------------------------------
int
CxDateTime::epochSeconds(void)
{
    std::time_t now_c = std::chrono::system_clock::to_time_t(theTimePoint);
	return( now_c );
}


//-------------------------------------------------------------------------
// CxDateTime::epochSeconds
//
// Format a datetime string according to a format and adjusted to a 
// timezone.
//
// 
//
//-------------------------------------------------------------------------

CxString
CxDateTime::format( CxString format ) const
{
    CxString ts;
    
    if (format.length()==0) {
        format = "%Y-%m-%d %H:%M:%S %Ez";
    }
    
    std::string str = cctz::format( format.data(), theTimePoint, zone);
    ts = str.c_str();
    
    return(ts);
}

CxString
CxDateTime::format( cctz::time_zone destZone, CxString format ) const 
{ 
	CxString ts;

	if (format.length()==0) {
    	format = "%Y-%m-%d %H:%M:%S %Ez";
	}

    std::string str = cctz::format( format.data(), theTimePoint, destZone);
    ts = str.c_str();

	return(ts);
}




//-------------------------------------------------------------------------
// firstSecondOfTheDay
//
// create a datetime object that respresents the first second of the day
//
//-------------------------------------------------------------------------
/*static*/
CxDateTime
CxDateTime::firstSecondOfTheDay( CxDateTime dt )
{
	long year, month, day, hour, minute, second;
	dt.breakdown( &year, &month, &day, &hour, &minute, &second, dt.timeZone());
	
	CxDateTime dt2 = CxDateTime( 
		year,
		month,
		day,
		0,
		0,
		0,
		dt.timeZone());

	return( dt2 );
}

/*static*/
CxDateTime
CxDateTime::firstSecondOfTheDay( CxDateTime dt, cctz::time_zone destZone)
{
    long year, month, day, hour, minute, second;
    dt.breakdown( &year, &month, &day, &hour, &minute, &second, destZone);
    
    CxDateTime dt2 = CxDateTime(
                                year,
                                month,
                                day,
                                0,
                                0,
                                0,
                                destZone);
    
    return( dt2 );
}

//-------------------------------------------------------------------------
// lastSecondOfTheDay
//
// create a datetime object that respresents the last second of the day
//
//-------------------------------------------------------------------------
/*static*/
CxDateTime
CxDateTime::lastSecondOfTheDay( CxDateTime dt )
{
	long year, month, day, hour, minute, second;
	dt.breakdown( &year, &month, &day, &hour, &minute, &second, dt.timeZone());
	
	CxDateTime dt2 = CxDateTime(
		year,
		month,
		day,
		23,
		59,
		59,
		dt.timeZone());

	return( dt2 );
}


/*static*/
CxDateTime
CxDateTime::lastSecondOfTheDay( CxDateTime dt, cctz::time_zone destZone)
{
    long year, month, day, hour, minute, second;
    dt.breakdown( &year, &month, &day, &hour, &minute, &second, destZone);
    
    CxDateTime dt2 = CxDateTime(
                                year,
                                month,
                                day,
                                23,
                                59,
                                59,
                                destZone);
    
    return( dt2 );
    
}

/*static*/
CxString
CxDateTime::formatTimeLength(CxDateTime startTime, CxDateTime endTime)
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
// CxDateTime::operator<<
//
//
//-------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& str, const CxDateTime& dt )
{
    cctz::time_zone tz = dt.timeZone();
    str << dt.format(tz).data();
    return(str);
}


//-------------------------------------------------------------------------
// CxDateTime::dump(void)
//
// debug output
//
//-------------------------------------------------------------------------
void
CxDateTime::dump(void)
{
    printf("--CxDateTime::dump()--\n"  );
    printf("currentZoneName   = %s\n", timeZoneName().data());
    printf("theTimePoint      = %s\n", format(timeZone()).data());
    printf("epochSeconds      = %d\n",  epochSeconds());
    printf("----------------------\n"  );
}


