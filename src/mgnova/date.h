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
 *						http://www.xmission.com/~tknarr/code/Date.html
 */


#pragma once


#include <string>
#include <map>

#include "mgnova/object.h"
#include "mgnova/argconvdef.h"


MG_NAMESPACE_BEGIN


class MG_Date : public MG_Object
{
public:
	/* Constructors / Destructor */
    MG_Date(void);
    COPY_CTOR_DECL(MG_Date)
	~MG_Date(void);

	ASSIGN_OPERATOR(MG_Date)
	CLONE_METHOD(MG_Date)
	SWAP_DECL(MG_Date)

public:
	MG_Date(const long& aJD);
	MG_Date(const int& aY, const unsigned int& aM, const unsigned int& aD);
	MG_Date(const std::string& aDate, const char& aSeparator = '/', const DATE_DISPLAY& aDD = FR_DATE);

	/* Assignment operators */
	MG_Date& operator+= (const long& aRight);
	MG_Date& operator-= (const long& aRight);
	//MG_Date& operator+= (const MG_Date& aRight);
	long operator-= (const MG_Date& aRight);

	/* Arithmetic operators */
	/* Prefix */
	MG_Date& operator++ (void);
	MG_Date& operator-- (void);
	/* Postfix */
	MG_Date operator++ (int);
	MG_Date operator-- (int);

	/* logical operators */
	bool operator==	(const MG_Date& aRight) const;
	bool operator!=	(const MG_Date& aRight) const;
	bool operator<	(const MG_Date& aRight) const;
	bool operator<=	(const MG_Date& aRight) const;
	bool operator>	(const MG_Date& aRight) const;
	bool operator>=	(const MG_Date& aRight) const;

	/* Accessors */
	inline long			GetJulianDay	(void) const { return myJulianDay; }
	inline unsigned int	GetDay			(void) const { return myDay; }
	inline unsigned int	GetMonth		(void) const { return myMonth; }
	inline int			GetYear			(void) const { return myYear; }
	inline bool			IsLeapYear		(void) const { return myIsLeapYear; }
	inline unsigned int	GetDayOfWeek	(void) const { return myDayOfWeek; }
	inline unsigned int	GetDayOfWorkWeek(void) const { return myDayOfWorkWeek; }
	inline unsigned int	GetDayOfYear	(void) const { return myDayOfYear; }

	/* String function */
	std::string ToString(const char& aSeparator, const DATE_DISPLAY& aDD = FR_DATE) const;
	std::string ToString(void) const;

	/* Calendars functions */
	MG_Date& NextBusinessDay		(size_t aDays, const CALENDAR_NAME& aCal);
	MG_Date& PreviousBusinessDay	(size_t aDays, const CALENDAR_NAME& aCal);
	MG_Date& AdjustToBusinessDay	(const CALENDAR_NAME& aCal, const ADJRULE_NAME& aAdjRule = K_MODIFIED_FOLLOWING);
	double BetweenDays		(	const MG_Date		& aDt
							,	const DAYCOUNT_NAME	& aDayCount
							,	const bool			& aIsFrac
							,	const CALENDAR_NAME	& aCal = NB_CALENDARS) const;
	MG_Date& AddDays		(	const int& aDays, const CALENDAR_NAME& aCal);
	MG_Date& AddMonths		(	const int	& aFreq
							,	const int	& aTimes
							,	const bool	& aEndOfMonth = true);
	MG_Date& AddPeriod		(	const int			& aFreq
							,	const int			& aTimes
							,	const CALENDAR_NAME	& aCal = NB_CALENDARS
							,	const ADJRULE_NAME	& aAdjRule = K_MODIFIED_FOLLOWING
							,	const bool			& aEndOfMonth = true);

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
    static void JdToYmd(const long& aJD, int& aY, unsigned int& aM, unsigned int& aD);
    static long YmdToJd(const int& aY, const unsigned int& aM, const unsigned int& aD);

    static bool IsLeapYear(const int& aY);

    static unsigned int DayOfYear		(const long& aJD, const int& aY);
    static unsigned int DayOfWeek		(const long& aJD);
    static unsigned int DayOfWorkWeek	(const long& aJD);
public:
	static long ToJulianDay(const char* aDate, char aSeparator, const DATE_DISPLAY& aDD);

public:
	/* friend functions */
	friend MG_Date operator+ (MG_Date vLeft, const long& vRight)
	{
		return vLeft += vRight;
	}
	friend MG_Date operator+ (const long& vLeft, MG_Date vRight)
	{
		return vRight += vLeft;
	}
	/*friend MG_Date operator+ (const MG_Date& vLeft, MG_Date vRight)
	{
		return vRight += vLeft;
	}*/
	friend MG_Date operator- (MG_Date vLeft, const long& vRight)
	{
		return vLeft -= vRight;
	}
	/*friend MG_Date operator- (const long& vLeft, MG_Date vRight)
	{
		return vRight -= vLeft;
	}*/
	friend long operator- (const MG_Date& vLeft, MG_Date& vRight)
	{
		return vLeft.GetJulianDay() - vRight.GetJulianDay();
	}

};


MG_NAMESPACE_END
