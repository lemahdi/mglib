/*
 * Copyright			: 2011 by MG
 * File					: calendar.h
 * Version				: 0.1.21
 * Last changed			: 12 MAR 2011
 * Purpose				: MG_Calendar is intended to manage currency business days
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "mgnova/date.h"

#include <map>
#include <list>


MG_NAMESPACE_BEGIN


struct MG_Calendar
{
	struct CalPpty
	{
		short myWeekEnds;
		std::string myName;
	};

	static std::map< CALENDAR_NAME , CalPpty > ourCalPpty;

	static bool LoadCalendars(void);
	static bool IsBusinessDay(const CALENDAR_NAME& aCal, const MG_Date& aDate);
	static bool IsWeekEnd(const CALENDAR_NAME& aCal, const MG_Date& aDate);

	typedef std::map< size_t , std::list<long> > mapYearJulian;
	typedef std::map< CALENDAR_NAME , mapYearJulian > mapCcyYearJul;
	static mapCcyYearJul ourCalendars;
};


MG_NAMESPACE_END