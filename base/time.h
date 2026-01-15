/***************************************************************************
 *
 *  time.h
 *
 *  CxTime Class
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


#include <sys/time.h>

//-------------------------------------------------------------------------
// standard includes
//
//-------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <time.h>


//-------------------------------------------------------------------------
// platform includes
//
//-------------------------------------------------------------------------
#if defined(_LINUX_)
#include <unistd.h>
#endif

#if defined(_NT_)
#endif


//-------------------------------------------------------------------------
// local includes
//
//-------------------------------------------------------------------------
#include <cx/base/string.h>


#ifndef _CxTime_
#define _CxTime_


#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


//-------------------------------------------------------------------------
// class CxTime
//
//-------------------------------------------------------------------------
class CxTime
{
  public:

	CxTime( void );
	// constructor

	CxTime( time_t epochSeconds_ );
	// construct with seconds from epoch

	CxTime( int year, int month, int day, int hour, int min, int sec );
	// construct from individual elements

	CxTime( const CxTime& t_ );
	// copy constructor

    ~CxTime( void );
	// destructor

	CxTime& operator=( const CxTime& t_ );
	// operator equal

	int operator==( const CxTime& t_ );
	// comparison operator    

    static CxTime now(void);
	// return an object with current time

    time_t epochSeconds( void ) const;
	// return time in seconds from epoch

    CxString asString( void ) const;
	// return current time as a string

	CxString asFormattedString( CxString format ) const;
	// return current time as a formatted string

	void offsetSeconds( int );
	// offset self by number of seconds

	void offsetMinutes( int );
	// offset self by number of minutes	

	void offsetHours( int );
	// offset self by number of hours	

	void offsetDays( int );
	// offset self by number of days	

	void offsetMonths( int );
	// offset self by number of months	

	void offsetYears( int );
	// offset self by number of years	

	static int monthNumber( CxString s );
	// based on month name return months number
	// (1-12), or -1

	int year( void );
	// return year of self (1900-2038)

 	int month( void );
	// return the month of the year of self (1-12)

 	int day( void );
	// returns the day of the month of self (1-31)

	int hour( void );
	// return hour of the day of self (0-23)

	int minute( void );
	// return minute past the hour of self (0-59)

	int second( void );
	// return second past the minute of self (0-59)


	static CxTime firstSecondOfTheDay( CxTime t);
	static CxTime lastSecondOfTheDay( CxTime t );

	static CxTime epochSecondsFromDateTimeStamp( CxString dateString, CxString timeString );

	static
	CxString formatTimeLength(CxTime startTime, CxTime endTime);

  private:

	friend std::ostream& operator<<(std::ostream& str, const CxTime& t_ );
	// outputs a CxTime to an ostream
 
	time_t _epochSeconds;
  	// seconds from Jan 1, 1970
};


#endif
