#include <stdio.h>
#include <sys/types.h>
#include <time.h>

#include <iostream>

#include <cx/base/string.h>
#include <cx/base/slist.h>
#include <cx/base/star.h>
#include <cx/base/time.h>

#include <cx/tz/civil_time.h>
#include <cx/tz/time_zone.h>


#ifndef _CX_DATE_TIME_
#define _CX_DATE_TIME_

class CxDateTime
{
  public:

	CxDateTime(void);
	// construct right now in current timezone
    
    CxDateTime( cctz::time_zone tz );
	// construct from time now in a specific timezone
    
    CxDateTime( time_t epochSecond );
    // construct from epochSeconds, default timezone
    
    CxDateTime( time_t epochSecond, cctz::time_zone);
	// construct from epochSeconds and timezone

    CxDateTime(std::chrono::system_clock::time_point tp );
    // construct from time_point, in default timezone
    
    CxDateTime(std::chrono::system_clock::time_point tp, cctz::time_zone tz );
	// construct from time_point and timezone

    CxDateTime( CxString timeString );
    // construct from time string in local time zone
    
	CxDateTime( CxString timeString, cctz::time_zone tz );
	// construct from time point and timezone

    CxDateTime(long year, long month, long day, long hour, long minute, long second);
    // construct from time elements in local time zone
    
	CxDateTime(long year, long month, long day, long hour, long minute, long second, cctz::time_zone tz);
	// construct from time elements and specific timezone

	CxDateTime( const CxDateTime& dt);
	// copy constructor

	CxDateTime& operator=( const CxDateTime& dt);
	// assignment operator

	int operator==( const CxDateTime& dt);
	// comparision
    
    CxString timeZoneName(void);
    // get the objects timezone Name
    
    cctz::time_zone timeZone(void) const;
	// get the timezone 

	static std::chrono::system_clock::time_point now_tp(void);
	// get a now system clock time (epoch * 10)
    
    static int now_epochSeconds(void);
    // get a new epoch seconds



	void dump(void);
	// debug dump internal data
	
	int epochSeconds(void);

	int breakdown(long *year, long *month, long *day, long *hour, long *minute, long *second );
    // breakdown the time parts using default timezone
    
	int breakdown(long *year, long *month, long *day, long *hour, long *minute, long *second, cctz::time_zone destZone);
    // breakdown the time parts using a specific timezone

	void offsetYears( long y);
	void offsetYears( long y, cctz::time_zone destZone);
    
    void offsetMonths( long m );
	void offsetMonths( long m, cctz::time_zone destZone);
    
    void offsetDays( long d );
    void offsetDays( long d, cctz::time_zone destZone);

	void offsetHours( long h );
    void offsetHours( long h, cctz::time_zone destZone);
    
    void offsetMinutes( long m );
	void offsetMinutes( long m, cctz::time_zone destZone);

    void offsetSeconds( long s );
    void offsetSeconds( long s, cctz::time_zone destZone);

    CxString format( CxString format="") const;
    // format output for default timezone
    
    CxString format( cctz::time_zone destZone, CxString format="") const;
    // format output for a specific timezone
    
	static 
	CxDateTime firstSecondOfTheDay( CxDateTime dt );
    static
    CxDateTime firstSecondOfTheDay( CxDateTime dt, cctz::time_zone);

    static
    CxDateTime lastSecondOfTheDay( CxDateTime dt );
	static 
	CxDateTime lastSecondOfTheDay( CxDateTime dt, cctz::time_zone); 
    
	static
	cctz::time_zone zoneNameToZone( CxString zoneName );

	static
	CxString formatTimeLength(CxDateTime startTime, CxDateTime endTime);


  private:
    
    void
    set(int year, int month, int day, int hour, int min, int sec );
    // set the object time using parts and objects existing timezone
    
    void
    set(int year, int month, int day, int hour, int min, int sec, cctz::time_zone destZone);
    // set the object time using parts and a new default timezone

    friend std::ostream& operator<<(std::ostream& str, const CxDateTime& dt );
    // outputs a CxDateTime to an ostream

	CxString zoneName;
	
	cctz::time_zone zone;

	std::chrono::system_clock::time_point theTimePoint;
};

#endif

