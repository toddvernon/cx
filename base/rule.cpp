//-------------------------------------------------------------------------------------------------
//
//  rule.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxRuleEntry, CxRuleList Class
//
//-------------------------------------------------------------------------------------------------

#include <cx/base/rule.h>


//-------------------------------------------------------------------------
// CxRuleEntry::CxRuleEntry
//
//-------------------------------------------------------------------------
CxRuleEntry::CxRuleEntry()
{
}

//-------------------------------------------------------------------------
// CxRuleEntry::CxRuleEntry
//
//-------------------------------------------------------------------------
CxRuleEntry::CxRuleEntry( int year_, int bmonth_, int emonth_, int bday_, int eday_, int dow_, int bhour_, int ehour_, int bminute_, int eminute_, CxString message_ )
{
	_year    = year_;
	_bmonth  = bmonth_;
	_emonth  = emonth_;
	_bday    = bday_;
	_eday    = eday_;
	_dow     = dow_;
	_bhour   = bhour_;
	_ehour   = ehour_;
	_bminute = bminute_;
	_eminute = eminute_;
	
	_message = message_;

}

//-------------------------------------------------------------------------
// CxRuleEntry::CxRuleEntry
//
//-------------------------------------------------------------------------
CxRuleEntry::CxRuleEntry(const CxRuleEntry& re)
{
	if (this != &re) {

		_year    = re._year;
		_bmonth  = re._bmonth;
		_emonth  = re._emonth;
		_bday    = re._bday;
		_eday    = re._eday;
		_dow     = re._dow;
		_bhour   = re._bhour;
		_ehour   = re._ehour;
		_bminute = re._bminute;
		_eminute = re._eminute;
	
		_message = re._message;

	}
}

//-------------------------------------------------------------------------
// CxRuleEntry::operator=
//
//-------------------------------------------------------------------------
CxRuleEntry&
CxRuleEntry::operator=(const CxRuleEntry& re)
{
	if (this != &re) {
		_year    = re._year;
		_bmonth  = re._bmonth;
		_emonth  = re._emonth;
		_bday    = re._bday;
		_eday    = re._eday;
		_dow     = re._dow;
		_bhour   = re._bhour;
		_ehour   = re._ehour;
		_bminute = re._bminute;
		_eminute = re._eminute;
	
		_message = re._message;
	}

	return(*this);
}

//-------------------------------------------------------------------------
// CxRuleEntry::operator==
//
//-------------------------------------------------------------------------
int
CxRuleEntry::operator==(const CxRuleEntry& re) const
{
    	if ( 	( _year    == re._year  	) && 
		( _bmonth  == re._bmonth 	) && 
		( _emonth  == re._emonth 	) && 
		( _bday    == re._bday		) && 
		( _eday    == re._eday 		) && 
		( _dow     == re._dow           ) &&
		( _bhour   == re._bhour 	) && 
		( _ehour   == re._ehour 	) && 
		( _bminute == re._bminute 	) && 
		( _eminute == re._eminute 	) && 
		( _message == re._message       ) ) 
	{
		return (1);
	}

	return( 0 );
}
     

int
CxRuleEntry::isMatch( int now_year, int now_month, int now_day, int now_dow, int now_hour, int now_minute )
{
	// check to make sure now is within the bounds of the rule

	if ( !(( now_month  >= _bmonth)    && ( now_month  <= _emonth  )) )  	return( 0 );
	if ( !(( now_day    >= _bday)      && ( now_day    <= _eday    )) )  	return( 0 );
	if ( !(( now_hour   >= _bhour)     && ( now_hour   <= _ehour   )) ) 	return( 0 );
	if ( !(( now_minute >= _bminute)   && ( now_minute <= _eminute )) ) 	return( 0 );


	// if the rule specifies a specific year, then test for it now
	if ( _year != -1) {
		if ( now_year != _year  ) return( 0 );
	}

	// if the rule specifies a specific day of the week, then test if now is it.
	if ( _dow != -1) {  
		if ( now_dow != _dow ) return( 0 );
	}

	return( 1 );
}


CxString
CxRuleEntry::message( void )
{
	return( _message );
}


int
CxRuleEntry::dayname_to_number(CxString dayName)
{
	if (dayName=="Sunday")    return(1);
	if (dayName=="Monday")    return(2);
	if (dayName=="Tuesday")   return(3);
	if (dayName=="Wednesday") return(4);
	if (dayName=="Thursday")  return(5);
	if (dayName=="Friday")    return(6);
	if (dayName=="Saturday")  return(7);
	return( 0 );
}


//-------------------------------------------------------------------------
// CxRuleList::CxRuleList
//
//-------------------------------------------------------------------------
CxRuleList::CxRuleList(void)
{
}

//-------------------------------------------------------------------------
// CxRuleList::~CxRuleList
//
//-------------------------------------------------------------------------
CxRuleList::~CxRuleList(void)
{
}

//-------------------------------------------------------------------------
// CxRuleList::CxRuleList
//
//-------------------------------------------------------------------------
CxRuleList::CxRuleList( const CxRuleList& rl )
{
	if (this != &rl) {
		_list = rl._list;
	}
}

//-------------------------------------------------------------------------
// CxRuleList::operator=
//
//-------------------------------------------------------------------------
CxRuleList&
CxRuleList::operator=( const CxRuleList& rl )
{
	if (this != &rl) {
		_list = rl._list;
	}	
	return (*this);
}

//-------------------------------------------------------------------------
// CxRuleList::operator+
//
//-------------------------------------------------------------------------
CxRuleList&
CxRuleList::operator+( CxRuleList& rl )
{
    	if ( this == &rl ) return *this;

	_list.append( rl._list );

    	return(*this);
}

//-------------------------------------------------------------------------
// CxRuleList::append
//
//-------------------------------------------------------------------------
void
CxRuleList::append( CxRuleEntry re )
{
	_list.append( re );
}



//-------------------------------------------------------------------------
// CxRuleList::has
//
//-------------------------------------------------------------------------
CxString 
CxRuleList::getMessage( int year, int month, int day, int dow, int hour, int minute )
{
	CxString message;
	CxString blank = "  ";

	for (unsigned int c=0; c<_list.entries(); c++) {

		CxRuleEntry re = _list.at(c);

		if (re.isMatch( year, month, day, dow, hour, minute)) {

			message += blank + re.message();

		}

	}

	return( message );
}


//-------------------------------------------------------------------------
// CxRuleList::entries
//
//-------------------------------------------------------------------------
int
CxRuleList::entries( void ) const
{
    return (_list.entries());
}


//-------------------------------------------------------------------------
// CxRuleList::at
//
//-------------------------------------------------------------------------
CxRuleEntry
CxRuleList::at( int i )
{
	return( _list.at(i) );
}


//-------------------------------------------------------------------------
// CxRuleList::clear
//
//-------------------------------------------------------------------------
void
CxRuleList::clear(void)
{
    _list.clear();
}




//-------------------------------------------------------------------------
// CxRuleList::dump
//
//-------------------------------------------------------------------------
void
CxRuleList::dump( void )
{
	std::cout << "ENTRIES=" << _list.entries() << std::endl;

	for (unsigned int c=0; c<_list.entries(); c++) {

		CxRuleEntry re = _list.at(c);

		std::cout << "----------------------" << std::endl;
		std::cout << "YEAR="   << re._year << std::endl;
		std::cout << "BMONTH=" << re._bmonth << "  EMONTH=" << re._emonth << std::endl;
		std::cout << "BDAY=" << re._bday << "  EDAY=" << re._eday << std::endl;
		std::cout << "DOW=" << re._dow << std::endl;
		std::cout << "BHOUR=" << re._bhour << "  EHOUR=" << re._ehour << std::endl;
		std::cout << "BMINUTE=" << re._bminute << "  EMINUTE=" << re._eminute << std::endl;

	}
}


CxRuleList 
CxRuleList::loadFromFile( CxString inFileName )
{
        CxFile inFile;
        CxString data;
        CxRuleList ruleList;

        if (!inFile.open( inFileName, "r")) {
                std::cout << "AHH can't open rules.txt" << std::endl;
        }


        while (!inFile.eof()) {

                data = inFile.getUntil('\n');

                data.stripLeading(" \t");

                if ( data.length() ) {

                        if (data.firstChar('#') != 0) {

                                CxString rules    = data.nextToken(":");
                                CxString message  = data.nextToken(":" );

                                CxString YEAR     = rules.nextToken(" \t\n" );
                                if (YEAR=="*") YEAR = "-1";

                                CxString BMONTH   = rules.nextToken(" \t\n" );
                                CxString EMONTH   = rules.nextToken(" \t\n" );
                                CxString BDAY     = rules.nextToken(" \t\n" );
                                CxString EDAY     = rules.nextToken(" \t\n" );
                                CxString DOW      = rules.nextToken(" \t\n" );
                                if (DOW =="*") DOW = "-1";

                                CxString BHOUR    = rules.nextToken(" \t\n" );
                                CxString EHOUR    = rules.nextToken(" \t\n" );
                                CxString BMIN     = rules.nextToken(" \t\n" );
                                CxString EMIN     = rules.nextToken(" \t\n" );

                                CxRuleEntry re = CxRuleEntry(
                                        YEAR.toInt(),
                                        BMONTH.toInt(),
                                        EMONTH.toInt(),
                                        BDAY.toInt(),
                                        EDAY.toInt(),
                                        DOW.toInt(),
                                        BHOUR.toInt(),
                                        EHOUR.toInt(),
                                        BMIN.toInt(),
                                        EMIN.toInt(),
                                        message );

                                ruleList.append( re );

                        }
                }

        }

	return( ruleList );
}
