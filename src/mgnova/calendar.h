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
#include "mgnova/argconvdef.h"
#include "mgnova/patterns/repository.hpp"

#include <vector>


MG_NAMESPACE_BEGIN


class MG_Calendar
{
	template<typename T,class U> friend class Repository;

private:
	MG_Calendar(void);
	MG_Calendar(const CURRENCY_NAME& aCcyName);

public:
	~MG_Calendar(void) {}

public:
	void AddPeriod(const unsigned int& aNbDays, MG_Date& aDate) const;

private:
	std::vector<unsigned int> myHolidays;

private:
	static CURRENCY_NAME										ourDefaultCcy;
	static std::map<CURRENCY_NAME,std::vector<unsigned int> >	ourCalendars;

	static void Init(void);
};
typedef Repository<CURRENCY_NAME,MG_Calendar> MG_RCalendar;


MG_NAMESPACE_END