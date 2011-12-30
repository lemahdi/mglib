#include "mgnova/calendar.h"

#include <list>
#include <algorithm>
#include <math.h>


using namespace std;
using namespace MG;


#define MIN_YEAR 2000
#define MAX_YEAR 2100

#define calendarSunday		0x0001
#define calendarMonday		0x0002
#define calendarTuesday		0x0004
#define calendarWednesday	0x0008
#define calendarThursday	0x0010
#define calendarFriday		0x0020
#define calendarSaturday	0x0040
#define calendarMaxBits		4
#define calendarMask		0x0001


/*
 * Calendar Class
 */
MG_Calendar::mapCcyYearJul MG_Calendar::ourCalendars = MG_Calendar::mapCcyYearJul();
map< CALENDAR_NAME , MG_Calendar::CalPpty > MG_Calendar::ourCalPpty = map< CALENDAR_NAME , MG_Calendar::CalPpty >();

bool MG_Calendar::LoadCalendars()
{
	mapYearJulian vYearHolidays;
	CalPpty vCalDesc;
	CALENDAR_NAME vCalNm = CALENDAR_NAME_DEF;
	CURRENCY_NAME vCcyNm = CURRENCY_NAME_DEF;

	// US calendar
	vYearHolidays.clear();
	vCalNm = K_USD_CAL;
	vCcyNm = K_USD;
	for(int vYear=MIN_YEAR; vYear<=MAX_YEAR; ++vYear)
	{
		list<long> vDaysHolidays;
		vDaysHolidays.push_back(MG_Date(vYear, 1, 1).GetJulianDay());
		vDaysHolidays.push_back(MG_Date(vYear, 1, 18).GetJulianDay());
		vDaysHolidays.push_back(MG_Date(vYear, 2, 15).GetJulianDay());
		vDaysHolidays.push_back(MG_Date(vYear, 5, 31).GetJulianDay());
		vDaysHolidays.push_back(MG_Date(vYear, 7, 5).GetJulianDay());
		vDaysHolidays.push_back(MG_Date(vYear, 9, 6).GetJulianDay());
		vDaysHolidays.push_back(MG_Date(vYear, 10, 11).GetJulianDay());
		vDaysHolidays.push_back(MG_Date(vYear, 11, 11).GetJulianDay());
		vDaysHolidays.push_back(MG_Date(vYear, 11, 25).GetJulianDay());
		vDaysHolidays.push_back(MG_Date(vYear, 12, 25).GetJulianDay());
		vYearHolidays[vYear] = vDaysHolidays;
	}
	ourCalendars[vCalNm] = vYearHolidays;
	vCalDesc.myName = CurrenciesNameRevertor[vCcyNm];
	vCalDesc.myWeekEnds = calendarSunday | calendarSaturday;
	ourCalPpty[vCalNm] = vCalDesc;
	
	// EURO calendar
	vYearHolidays.clear();
	vCalNm = K_EUR_CAL;
	vCcyNm = K_EUR;
	for(int vYear=MIN_YEAR; vYear<=MAX_YEAR; ++vYear)
	{
		list<long> vDaysHolidays;
		vDaysHolidays.push_back(MG_Date(vYear, 1, 1).GetJulianDay());
		vDaysHolidays.push_back(MG_Date(vYear, 4, 2).GetJulianDay());
		vDaysHolidays.push_back(MG_Date(vYear, 4, 5).GetJulianDay());
		vDaysHolidays.push_back(MG_Date(vYear, 5, 1).GetJulianDay());
		vDaysHolidays.push_back(MG_Date(vYear, 12, 25).GetJulianDay());
		vDaysHolidays.push_back(MG_Date(vYear, 12, 26).GetJulianDay());
		vYearHolidays[vYear] = vDaysHolidays;
	}
	ourCalendars[vCalNm] = vYearHolidays;
	vCalDesc.myName = CurrenciesNameRevertor[vCcyNm];
	vCalDesc.myWeekEnds = calendarSunday | calendarSaturday;
	ourCalPpty[vCalNm] = vCalDesc;
	
	// Japan calendar
	vYearHolidays.clear();
	vCalNm = K_JPY_CAL;
	vCcyNm = K_JPY;
	for(int vYear=MIN_YEAR; vYear<=MAX_YEAR; ++vYear)
	{
		list<long> vDaysHolidays;
		vDaysHolidays.push_back(MG_Date(vYear, 1, 1).GetJulianDay());
		vDaysHolidays.push_back(MG_Date(vYear, 1, 2).GetJulianDay());
		vDaysHolidays.push_back(MG_Date(vYear, 1, 3).GetJulianDay());
		vDaysHolidays.push_back(MG_Date(vYear, 1, 11).GetJulianDay());
		vDaysHolidays.push_back(MG_Date(vYear, 2, 11).GetJulianDay());
		vDaysHolidays.push_back(MG_Date(vYear, 3, 22).GetJulianDay());
		vDaysHolidays.push_back(MG_Date(vYear, 4, 29).GetJulianDay());
		vDaysHolidays.push_back(MG_Date(vYear, 5, 3).GetJulianDay());
		vDaysHolidays.push_back(MG_Date(vYear, 5, 4).GetJulianDay());
		vDaysHolidays.push_back(MG_Date(vYear, 5, 5).GetJulianDay());
		vDaysHolidays.push_back(MG_Date(vYear, 7, 19).GetJulianDay());
		vDaysHolidays.push_back(MG_Date(vYear, 9, 20).GetJulianDay());
		vDaysHolidays.push_back(MG_Date(vYear, 9, 23).GetJulianDay());
		vDaysHolidays.push_back(MG_Date(vYear, 10, 11).GetJulianDay());
		vDaysHolidays.push_back(MG_Date(vYear, 11, 3).GetJulianDay());
		vDaysHolidays.push_back(MG_Date(vYear, 11, 23).GetJulianDay());
		vDaysHolidays.push_back(MG_Date(vYear, 12, 23).GetJulianDay());
		vDaysHolidays.push_back(MG_Date(vYear, 12, 31).GetJulianDay());
		vYearHolidays[vYear] = vDaysHolidays;
	}
	ourCalendars[vCalNm] = vYearHolidays;
	vCalDesc.myName = CurrenciesNameRevertor[vCcyNm];
	vCalDesc.myWeekEnds = calendarSunday | calendarSaturday;
	ourCalPpty[vCalNm] = vCalDesc;
	
	// Brit calendar
	vYearHolidays.clear();
	vCalNm = K_GBP_CAL;
	vCcyNm = K_GBP;
	for(int vYear=MIN_YEAR; vYear<=MAX_YEAR; ++vYear)
	{
		list<long> vDaysHolidays;
		vDaysHolidays.push_back(MG_Date(vYear, 1, 1).GetJulianDay());
		vDaysHolidays.push_back(MG_Date(vYear, 4, 2).GetJulianDay());
		vDaysHolidays.push_back(MG_Date(vYear, 4, 5).GetJulianDay());
		vDaysHolidays.push_back(MG_Date(vYear, 5, 3).GetJulianDay());
		vDaysHolidays.push_back(MG_Date(vYear, 5, 31).GetJulianDay());
		vDaysHolidays.push_back(MG_Date(vYear, 8, 30).GetJulianDay());
		vDaysHolidays.push_back(MG_Date(vYear, 12, 27).GetJulianDay());
		vDaysHolidays.push_back(MG_Date(vYear, 12, 28).GetJulianDay());
		vYearHolidays[vYear] = vDaysHolidays;
	}
	ourCalendars[vCalNm] = vYearHolidays;
	vCalDesc.myName = CurrenciesNameRevertor[vCcyNm];
	vCalDesc.myWeekEnds = calendarSunday | calendarSaturday;
	ourCalPpty[vCalNm] = vCalDesc;

	return true;
}

bool MG_Calendar::IsBusinessDay(const CALENDAR_NAME& aCal, const MG_Date& aDate)
{
	switch (aCal)
	{
	case NB_CALENDARS:
	default:
		if (aDate.GetDayOfWeek() >= 5)
			return false;
	};

	long vYear = aDate.GetYear();
	/*if (vYear>MAX_YEAR || vYear<MIN_YEAR)
		return false;*/

	list<long> vYearDts = ourCalendars[aCal][vYear];
	long vJulianDay = aDate.GetJulianDay();
	list<long>::iterator vItList = lower_bound(vYearDts.begin(), vYearDts.end(), vJulianDay);

	if (vItList!=vYearDts.end() && *vItList==vJulianDay)
		return false;

	return true;
}

bool MG_Calendar::IsWeekEnd(const CALENDAR_NAME& aCal, const MG_Date& aDate)
{
	short vWE = ourCalPpty[aCal].myWeekEnds;
	unsigned int vDayWeek = aDate.GetDayOfWeek();
	short vWEBit = (short)pow(2., (int)vDayWeek);

	if (vWE & vWEBit)
		return true;

	return false;
}


#undef MIN_YEAR
#undef MAX_YEAR
