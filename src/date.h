/*
 * Copyright			: 2010 by MG
 * File					: date.h
 * Version				: 0.1.16
 * Last changed			: 25 DEC 2010
 * Purpose				: Tiny class for Julian dates
 * Author				: MM Akkouh
 * Notes				: The original author is Todd Knarr who put
 *						his version under the GNU General Public Licence.
 *						This file respects the purposes of the GNU General
 *						Public Licence.
 * Original notes		: Provides a MG_Date class which represents dates as
 *						Julian day numbers ( days since 1 Jan 4713 BC ).
 *						This class can handle all dates from  1 Jan 4713BC to 31 Dec 4999AD.
 *						Note: Years AD are positive, years BC are negative. There is
 *						no year 0AD, it goes from 1BC to 1AD. A year of 0 will be treated
 *						as 1BC. No attempt is made to validate ranges. Physical errors
 *						will not result from insane day-month combinations like the 87th
 *						day of the 45th month, but the results will obviously not make
 *						much sense.
 */

#pragma once

#include <iostream>
#include <time.h>
#include <string>
#include <map>

#include "typedef.h"

class MG_Date
{
public:
	/* Constructors / Destructor */
    MG_Date(void);
    MG_Date(const MG_Date& aFrom);
	~MG_Date(void) {}

	MG_Date(const long& aJD);
	MG_Date(const int& aY, const unsigned int& aM, const unsigned int& aD);
#ifndef __CYGWIN__
	MG_Date(const time_t& aSysTime);
#endif
	MG_Date(const std::string& aDate, const char& aSeparator, const DATE_DISPLAY& aDD);

	/* Assignment operators */
    MG_Date& operator= (const MG_Date& aFrom);
    MG_Date& operator+= (const long& vRight);
    MG_Date& operator-= (const long& vRight);

	/* Arithmetic operators */
    long operator+ (const MG_Date& vRight);
    long operator- (const MG_Date& vRight);
    
	MG_Date& operator++ (void);
    MG_Date& operator-- (void);

	/* logical operators */
	bool operator==	(const MG_Date& vRight) const;
	bool operator!=	(const MG_Date& vRight) const;
	bool operator<	(const MG_Date& vRight) const;
	bool operator<=	(const MG_Date& vRight) const;
	bool operator>	(const MG_Date& vRight) const;
	bool operator>=	(const MG_Date& vRight) const;

	/* Accessors */
	inline long GetJulianDay(void) const { return myJulianDay; }

	/* String function */
	std::string ToString(const char& aSeparator, const DATE_DISPLAY& aDD = FR_DATE) const;
	std::string ToString(void) const;

private:
	/* Update */
	void Rebuild(void);

private:
    long			myJulianDay;

	unsigned int	myDay;
	unsigned int	myMonth;
	int				myYear;

	bool			myIsLeapYear;

	unsigned int	myDayOfWeek;
	unsigned int	myDayOfWorkWeek;
	unsigned int	myDayOfYear;

private:
	/* static functions */
    static void JdToYmd(const long& aJD, int* aY, unsigned int* aM, unsigned int* aD);
    static long YmdToJd(const int& aY, const unsigned int& aM, const unsigned int& aD);

    static bool IsLeapYear(const int& aY);

    static unsigned int DayOfYear		(const long& aJD, const int& aY);
    static unsigned int DayOfWeek		(const long& aJD);
    static unsigned int DayOfWorkWeek	(const long& aJD);
public:
	static long ToJulianDay(const char* aDate, char aSeparator, const DATE_DISPLAY& aDD);

public:
	/* friend functions */
	friend MG_Date operator+ (const MG_Date& vLeft, const long& vRight)
	{
		long vJD = vLeft.myJulianDay + vRight;
		MG_Date vDate(vJD);
		return vDate;
	}
	friend MG_Date operator+ (const long& vLeft, const MG_Date& vRight)
	{
		long vJD = vLeft + vRight.myJulianDay;
		MG_Date vDate(vJD);
		return vDate;
	}
	friend MG_Date operator- (const MG_Date& vLeft, const long& vRight)
	{
		long vJD = vLeft.myJulianDay - vRight;
		MG_Date vDate(vJD);
		return vDate;
	}
	friend MG_Date operator- (const long& vLeft, const MG_Date& vRight)
	{
		long vJD = vLeft - vRight.myJulianDay;
		MG_Date vDate(vJD);
		return vDate;
	}

};

