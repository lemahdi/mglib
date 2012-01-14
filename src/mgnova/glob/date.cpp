#include "mgnova/glob/date.h"
#include "mgnova/market/calendar.h"
#include "mgnova/utils/utils.h"

#include <time.h>
#include <assert.h>
#include <sstream>


#ifdef WIN32
//#define itoa _itoa_s
#define localtime localtime_s
#endif


using namespace std;
using namespace MG;
using namespace MG_utils;


/*
 * Default constructor
 */
MG_Date::MG_Date() : MG_Object(), myJulianDay(0L)
{
#ifdef WIN32
	time_t vSysTime;
	time(&vSysTime);
	struct tm vTm;
	errno_t vErr = localtime(&vTm, &vSysTime);vErr;

	myYear		= vTm.tm_year + 1900;
	myMonth		= vTm.tm_mon + 1;
	myDay		= vTm.tm_mday;
#else
	struct tm* vTm;
	time_t vSysTime;
	vTm = localtime(&vSysTime);

	myYear		= vTm->tm_year + 1900;
	myMonth		= vTm->tm_mon + 1;
	myDay		= vTm->tm_mday;
#endif

	myJulianDay		= MG_Date::YmdToJd(myYear, myMonth, myDay);
	Rebuild();
}

/*
 * Copy constructor
 */
MG_Date::MG_Date(	const MG_Date& aRight)
				:	MG_Object(aRight)
				,	myJulianDay		(aRight.myJulianDay)
				,	myYear			(aRight.myYear)
				,	myMonth			(aRight.myMonth)
				,	myDay			(aRight.myDay)
				,	myIsLeapYear	(aRight.myIsLeapYear)
				,	myDayOfWeek		(aRight.myDayOfWeek)
				,	myDayOfWorkWeek	(aRight.myDayOfWorkWeek)
				,	myDayOfYear		(aRight.myDayOfYear)
{}

void MG_Date::Swap(MG_Date& aRight)
{
	swap(myJulianDay,		aRight.myJulianDay);
	swap(myDay,				aRight.myDay);
	swap(myMonth,			aRight.myMonth);
	swap(myYear,			aRight.myYear);
	swap(myIsLeapYear,		aRight.myIsLeapYear);
	swap(myDayOfWeek,		aRight.myDayOfWeek);
	swap(myDayOfWorkWeek,	aRight.myDayOfWorkWeek);
	swap(myDayOfYear,		aRight.myDayOfYear);
}

/*
 * Constructor YYYY MM DD
 */
MG_Date::MG_Date(	const int& aY, const unsigned int& aM, const unsigned int& aD)
				:	MG_Object(), myYear(aY), myMonth(aM), myDay(aD)
{
	myJulianDay = MG_Date::YmdToJd(myYear, myMonth, myDay);
	Rebuild();
}

/*
 * Constructor from a julian day
 */
MG_Date::MG_Date(	const long& aJD)
				:	MG_Object()
				,	myJulianDay(aJD)
{
	MG_Date::JdToYmd(myJulianDay, myYear, myMonth, myDay);
	Rebuild();
}

/*
 * Constructor by string DD/MM/YYYY or YYYY-DD-MM or YYYY MMM DD
 */
MG_Date::MG_Date(	const string& aDate
				,	const char& aSeparator
				,	const DATE_DISPLAY& aDD)
				:	MG_Object()
{
	if (aDate.find(aSeparator) == string::npos)
	{
		ostringstream os;
		os << __FILE__ << "-" << __LINE__ << ": bad separator.";
#ifndef __CYGWIN__
		throw exception(os.str().c_str());
#endif
	}

	size_t vPos1 = aDate.find(aSeparator, 0);
	size_t vPos2 = aDate.find(aSeparator, vPos1);

	string vTok1 = aDate.substr(0, vPos1-1);
	string vTok2 = aDate.substr(vPos1+1, vPos2-1);
	string vTok3 = aDate.substr(vPos2+1);

	switch(aDD)
	{
	case ENG_DATE:			// 2010/12/25
	{
		myYear	= atoi(vTok1.c_str());
		myMonth	= atoi(vTok2.c_str());
		myDay	= atoi(vTok3.c_str());
		break;
	}
	case FR_DATE:			// 25/12/2010
	{
		myYear	= atoi(vTok3.c_str());
		myMonth	= atoi(vTok2.c_str());
		myDay	= atoi(vTok1.c_str());
		break;
	}
	case ENG_M3L_DATE:		// 2010-Dec-25
	{
		myYear	= atoi(vTok1.c_str());
		vTok2	= ToUpper(vTok2);
		myMonth	= (unsigned int)MonthsConvertor[vTok2] + 1;
		myDay	= atoi(vTok3.c_str());
		break;
	}
	case FR_M3L_DATE:		// 25-Dec-2010
	{
		myYear	= atoi(vTok3.c_str());
		vTok2	= ToUpper(vTok2);
		myMonth	= (unsigned int)MonthsConvertor[vTok2] + 1;
		myDay	= atoi(vTok1.c_str());
		break;
	}
	default: assert("Unrecognized display mode for date.");
	}

	myJulianDay = MG_Date::YmdToJd(myYear, myMonth, myDay);
	Rebuild();
}

MG_Date::~MG_Date()
{}

/*
 * Assignment operators
 */
MG_Date& MG_Date::operator+= (const long& aRight)
{
	myJulianDay += aRight;
	MG_Date::JdToYmd(myJulianDay, myYear, myMonth, myDay);
	Rebuild();
	return *this;
}
MG_Date& MG_Date::operator-= (const long& aRight)
{
	myJulianDay -= aRight;
	MG_Date::JdToYmd(myJulianDay, myYear, myMonth, myDay);
	Rebuild();
	return *this;
}
/*MG_Date& MG_Date::operator+= (const MG_Date& aRight)
{
	myJulianDay += aRight.myJulianDay;
	MG_Date::JdToYmd(myJulianDay, myYear, myMonth, myDay);
	Rebuild();
	return *this;
}*/
long MG_Date::operator-= (const MG_Date& aRight)
{
	/*myJulianDay -= aRight.myJulianDay;
	MG_Date::JdToYmd(myJulianDay, myYear, myMonth, myDay);
	Rebuild();
	return *this;*/
	return myJulianDay - aRight.myJulianDay;
}

/*
 * Arithmetic operators
 */
MG_Date& MG_Date::operator++ ()
{
	myJulianDay++;
	MG_Date::JdToYmd(myJulianDay, myYear, myMonth, myDay);
	Rebuild();
	return *this;
}
MG_Date& MG_Date::operator-- ()
{
	myJulianDay--;
	MG_Date::JdToYmd(myJulianDay, myYear, myMonth, myDay);
	Rebuild();
	return *this;
}
MG_Date MG_Date::operator++ (int)
{
	MG_Date vOld(*this);
	++*this;
	return vOld;
}
MG_Date MG_Date::operator-- (int)
{
	MG_Date vOld(*this);
	--*this;
	return vOld;
}

/*
 * Logical operators
 */
bool MG_Date::operator== (const MG_Date& aRight) const
{
	return myJulianDay == aRight.myJulianDay;
}
bool MG_Date::operator!= (const MG_Date& aRight) const
{
	return myJulianDay != aRight.myJulianDay;
}
bool MG_Date::operator< (const MG_Date& aRight) const
{
	return myJulianDay < aRight.myJulianDay;
}
bool MG_Date::operator<= (const MG_Date& aRight) const
{
	return myJulianDay <= aRight.myJulianDay;
}
bool MG_Date::operator> (const MG_Date& aRight) const
{
	return myJulianDay > aRight.myJulianDay;
}
bool MG_Date::operator>= (const MG_Date& aRight) const
{
	return myJulianDay >= aRight.myJulianDay;
}

/*
 * Updating
 */
void MG_Date::Rebuild()
{
	myIsLeapYear	= MG_Date::IsLeapYear(myYear);
	myDayOfWeek		= MG_Date::DayOfWeek(myJulianDay);
	myDayOfWorkWeek	= MG_Date::DayOfWorkWeek(myJulianDay);
	myDayOfYear		= MG_Date::DayOfYear(myJulianDay, myYear);
}

/*
 * String function
 */
std::string MG_Date::ToString(const char& aSeparator, const DATE_DISPLAY& aDD) const
{
	string vDate;
	char vBuffer[10];
	vBuffer[0] = aSeparator;
	vBuffer[1] = '\0';
	string vSeparator = string(vBuffer);
	string vDay = itoa(myDay);
	string vMonth = itoa(myMonth);
	string vYear = itoa(myYear);

	switch(aDD)
	{
	case ENG_DATE:
		vDate = vYear + vSeparator + vMonth + vSeparator + vDay;
		break;
	case FR_DATE:
		vDate = vDay + vSeparator + vMonth + vSeparator + vYear;
		break;
	case ENG_M3L_DATE:
		vDate = vYear + vSeparator + MonthsRevertor[myMonth-1] + vSeparator + vDay;
		break;
	case FR_M3L_DATE:
		vDate = vDay + vSeparator + MonthsRevertor[myMonth-1] + vSeparator + vYear;
		break;
	default: assert("Bad displaying date parameter.");
	}

	return vDate;
}

std::string MG_Date::ToString() const
{
	char vBuffer[12];

    long vJD = myJulianDay;
    if (vJD < 0L)
        vBuffer[0] = '-';
    else
        vBuffer[0] = '+';

    vBuffer[11] = '\0';
    for (unsigned int i=10; i>0; i--)
    {
        vBuffer[i] = (char)(vJD % 10L) + '0';
        vJD /= 10;
    }

    return string(vBuffer);
}

/*
 * Static function
 * Retrieve the year, month and day from the julian day
 */
void MG_Date::JdToYmd(const long& aJD, int& aY, unsigned int& aM, unsigned int& aD)
{
    long t1, t2, yr, mo;

    t1 = aJD + 68569L;
    t2 = 4L * t1 / 146097L;
    t1 = t1 - ( 146097L * t2 + 3L ) / 4L;
    yr = 4000L * ( t1 + 1L ) / 1461001L;
    t1 = t1 - 1461L * yr / 4L + 31L;
    mo = 80L * t1 / 2447L;
    aD = (int) ( t1 - 2447L * mo / 80L );
    t1 = mo / 11L;
    aM = (int) ( mo + 2L - 12L * t1 );
    aY = (int) ( 100L * ( t2 - 49L ) + yr + t1 );

    // Correct for BC years
    if (aY <= 0)
        aY -= 1;

    return;
}

/*
 * Returns the julian day from the day, month and year
 */
long MG_Date::YmdToJd(const int& aY, const unsigned int& aM, const unsigned int& aD)
{
    long vJD;
    long lmonth = (long) aM, lday = (long) aD, lyear = (long) aY;

    // Adjust BC years
    if ( lyear < 0 )
        lyear++;

    vJD = lday - 32075L +
        1461L * ( lyear + 4800L + ( lmonth - 14L ) / 12L ) / 4L +
        367L * ( lmonth - 2L - ( lmonth - 14L ) / 12L * 12L ) / 12L -
        3L * ( ( lyear + 4900L + ( lmonth - 14L ) / 12L ) / 100L ) / 4L;

    return vJD;
}

/*
 * Is a leap year
 */
bool MG_Date::IsLeapYear(const int& aY)
{
	long vJd1, vJd2;
	vJd1 = MG_Date::YmdToJd(aY, 2, 28);
	vJd2 = MG_Date::YmdToJd(aY, 3, 1);
	return ((vJd2 - vJd1) > 1);
}

/*
 * Day of a year with 1 Jan being day 1
 */
unsigned int MG_Date::DayOfYear(const long& aJD, const int& aY)
{
    long vJD = MG_Date::YmdToJd(aY, 1, 1);
    return (unsigned int)(aJD - vJD + 1);
}

/*
 * Day of the week from 0 through 6, matching localtime()'s convention ( 0 = Sunday, 6 = Saturday )
 */
unsigned int MG_Date::DayOfWeek(const long& aJD)
{
	return ((((unsigned int)(aJD % 7L)) + 1) % 7);
}

/*
 * Day of the week from 0 through 6, starting the week on Monday ( 0 = Monday, 6 = Sunday )
 */
unsigned int MG_Date::DayOfWorkWeek(const long& aJD)
{
	return ((unsigned int)(aJD % 7L));
}

/*
 * From a character string to a julian day
 */
long MG_Date::ToJulianDay(const char* aDate, char aSeparator, const DATE_DISPLAY& aDD)
{
	string vDate(aDate);
	if (vDate.find(aSeparator) == string::npos)
	{
		ostringstream os;
		os << __FILE__ << "-" << __LINE__ << ": bad separator.";
#ifndef __CYGWIN__
		throw exception(os.str().c_str());
#endif
	}

	size_t vPos1 = vDate.find(aSeparator, 0);
	size_t vPos2 = vDate.find(aSeparator, vPos1+1);

	string vTok1 = vDate.substr(0, vPos1);
	string vTok2 = vDate.substr(vPos1+1, vPos2-vPos1-1);
	string vTok3 = vDate.substr(vPos2+1, vDate.size()-vPos2-1);

	int				vYear	= 0;
	unsigned int	vMonth	= 0;
	unsigned int	vDay	= 0;

	switch(aDD)
	{
	case ENG_DATE:			// 2010/12/25
	{
		vYear	= atoi(vTok1.c_str());
		vMonth	= atoi(vTok2.c_str());
		vDay	= atoi(vTok3.c_str());
		break;
	}
	case FR_DATE:			// 25/12/2010
	{
		vYear	= atoi(vTok3.c_str());
		vMonth	= atoi(vTok2.c_str());
		vDay	= atoi(vTok1.c_str());
		break;
	}
	case ENG_M3L_DATE:		// 2010-Dec-25
	{
		vYear	= atoi(vTok1.c_str());
		vTok2	= ToUpper(vTok2);
		vMonth	= (unsigned int)MonthsConvertor[vTok2] + 1;
		vDay	= atoi(vTok3.c_str());
		break;
	}
	case FR_M3L_DATE:		// 25-Dec-2010
	{
		vYear	= atoi(vTok3.c_str());
		vTok2	= ToUpper(vTok2);
		vMonth	= (unsigned int)MonthsConvertor[vTok2] + 1;
		vDay	= atoi(vTok1.c_str());
		break;
	}
	default: assert("Unrecognized display mode for date.");
	}

	long vJulianDay = MG_Date::YmdToJd(vYear, vMonth, vDay);
	return vJulianDay;
}

/*
 * Calendars functions
 */
MG_Date& MG_Date::NextBusinessDay(size_t aDays, const CALENDAR_NAME& aCal)
{
	while (aDays)
	{
		++*this;
		if (MG_Calendar::IsBusinessDay(aCal, *this))
			--aDays;
	}
	return *this;
}

MG_Date& MG_Date::PreviousBusinessDay(size_t aDays, const CALENDAR_NAME& aCal)
{
	while (aDays)
	{
		--*this;
		if (MG_Calendar::IsBusinessDay(aCal, *this))
			--aDays;
	}
	return *this;
}

MG_Date& MG_Date::AdjustToBusinessDay(const CALENDAR_NAME& aCal, const ADJRULE_NAME& aAdjRule)
{
	return AddPeriod(K_DAILY, 0, aCal, aAdjRule, false);
}

MG_Date& MG_Date::AddDays(const int& aDays, const CALENDAR_NAME& aCal)
{
	if (aDays >= 0)
		return NextBusinessDay(size_t(aDays), aCal);
	else
		return PreviousBusinessDay(size_t(-aDays), aCal);
}

double MG_Date::BetweenDays	(	const MG_Date		& aDt
							,	const DAYCOUNT_NAME	& aDayCount
							,	const bool			& aIsFrac
							,	const CALENDAR_NAME	& aCal) const
{
	MG_Date vDt1(*this), vDt2(aDt);
	int vSign(1);
	if (vDt1 > vDt2)
	{
		MG_Date vTmp(vDt2);
		vDt2 = vDt1;
		vDt1 = vTmp;
		vSign = -1;
	}

	int vDay1 = vDt1.GetDay();
	int vDay2 = vDt2.GetDay();
	int vMth1 = vDt1.GetMonth();
	int vMth2 = vDt2.GetMonth();
	int vYr1 = vDt1.GetYear();
	int vYr2 = vDt2.GetYear();

	double vLag(0.);
	double vFrac = 1.;
	switch(aDayCount)
	{
	case K_ACT:
		{
			if (!aIsFrac)
			{
				vLag = vSign * (vDt1 - vDt2);
				break;
			}

			MG_Date vBeginYr1(vYr1, 1, 1);
			MG_Date vBeginYr2(vYr2, 1, 1);
			double vDays = 365.;
			if (vBeginYr1.IsLeapYear())
				vDays = 366.;
			double vLag1 = (vDt1 - vBeginYr1) / vDays;
			vDays = 365.;
			if (vBeginYr2.IsLeapYear())
				vDays = 366.;
			double vLag2 = -(vDt2 - vBeginYr2) / vDays;
			vLag = vLag1 + vLag2 - (vYr2 - vYr1);
			vFrac = 1.;
		}
		break;
	case K_ACT_360:
		{
			vLag = vDt1 - vDt2;
			vFrac = 360.;
		}
		break;
	case K_ACT_365:
		{
			vLag = vDt1 - vDt2;
			vFrac = 365.;
		}
		break;
	case K_U_30_360:
		{
			if (vDay1 == 31) vDay1 = 30;
			if (vDay1==30 && vDay2==31) vDay2 = 30;
			vLag = -(360 * (vYr2 - vYr1) + 30 * (vMth2 - vMth1) + (vDay2 - vDay1));
			vFrac = 360.;
		}
		break;
	case K_E_30_360:
		{
			if (vDay1 == 31) vDay1 = 30;
			if (vDay1<=30 && vDay2==31) vDay2 = 30;
			vLag = -(360 * (vYr2 - vYr1) + 30 * (vMth2 - vMth1) + (vDay2 - vDay1));
			vFrac = 360.;
		}
		break;
	case K_BUS_252:
		{
			while (vDt1 < vDt2)
			{
				if(MG_Calendar::IsWeekEnd(aCal, vDt1++))
					continue;
				--vLag;
			}
			vFrac = 252.;
		}
		break;
	};

	if (!aIsFrac)
		vFrac = 1.;

	return vSign * vLag / vFrac;
}

MG_Date& MG_Date::AddMonths	(	const int	& aFreq
							,	const int	& aTimes
							,	const bool	& aEndOfMonth)
{
	int vMonths = aFreq * aTimes;
	int vSign = vMonths>=0 ? 1 : -1;
	MG_Date vNextDt;
	if (vSign >= 0)
	{
		int vYrs = (myMonth+vMonths-1) / 12;
		int vNextMth = (myMonth+vMonths) % 12;
		if (vNextMth == 0) vNextMth = 12;
		vNextDt = MG_Date(myYear+vYrs, vNextMth, myDay);
	}
	else
	{
		int vYrs = -(-(int)myMonth-vMonths+12) / 12;
		int vNextMth = (myMonth+vMonths-12*vYrs) % 12;
		if (vNextMth == 0) vNextMth = 12;
		vNextDt = MG_Date(myYear+vYrs, vNextMth, myDay);
	}
	if (aEndOfMonth)
	{
		MG_Date vOneDayAfter = *this + 1;
		unsigned int vNewMth = vNextDt.GetMonth();
		if (vOneDayAfter.GetMonth() != myMonth)
		{
			do {
				++vNextDt;
			} while(vNextDt.GetMonth() == vNewMth);
			--vNextDt;
		}
	}
	*this = vNextDt;

	return *this;
}

MG_Date& MG_Date::AddPeriod	(	const int			& aFreq
							,	const int			& aTimes
							,	const CALENDAR_NAME	& aCal
							,	const ADJRULE_NAME	& aAdjRule
							,	const bool			& aEndOfMonth)
{
	unsigned int vDays(0), vMonths(0);
	switch (aFreq)
	{
	case K_DAILY: vDays = 1;
	case K_WEEKLY: vDays = vDays==0?7:vDays;
		{
			switch (aAdjRule)
			{
			case K_FIXED_RULE:
				{
					*this += vDays*aTimes;
				}
				break;

			case K_FOLLOWING_PAY:
			case K_FOLLOWING:
				{
					NextBusinessDay(vDays*aTimes, aCal);
				}
				break;

			case K_MODIFIED_FOLLOWING:
			case K_MODIFIED_FOLLOWING_NOREDEMP:
				{
					*this += vDays*aTimes;
					unsigned int vMth = myMonth;
					if (!MG_Calendar::IsBusinessDay(aCal, *this))
						NextBusinessDay(1, aCal);
					if (vMth != myMonth)
						PreviousBusinessDay(1, aCal);
				}
				break;

			case K_PREVIOUS_PAY:
			case K_PREVIOUS:
				{
					*this += vDays*aTimes;
					if (!MG_Calendar::IsBusinessDay(aCal, *this))
						PreviousBusinessDay(1, aCal);
				}
				break;

			case K_MODIFIED_PREVIOUS:
			case K_MODIFIED_PREVIOUS_NOREDEMP:
				{
					*this += vDays*aTimes;
					unsigned int vMth = myMonth;
					if (!MG_Calendar::IsBusinessDay(aCal, *this))
						PreviousBusinessDay(1, aCal);
					if (vMth != myMonth)
						NextBusinessDay(1, aCal);
				}
				break;
			}
			break;
		}

	case K_MONTHLY: vMonths = 1;
	case K_BIMONTHLY: vMonths = vMonths==0?2:vMonths;
	case K_QUARTERLY: vMonths = vMonths==0?3:vMonths;
	case K_SEMESTERLY: vMonths = vMonths==0?6:vMonths;
	case K_YEARLY: vMonths = vMonths==0?12:vMonths;
		{
			AddMonths(vMonths, aTimes, aEndOfMonth);
			switch (aAdjRule)
			{
			case K_FIXED_RULE: break;

			case K_FOLLOWING_PAY:
			case K_FOLLOWING:
				{
					if (!MG_Calendar::IsBusinessDay(aCal, *this))
						NextBusinessDay(1, aCal);
				}
				break;

			case K_MODIFIED_FOLLOWING:
			case K_MODIFIED_FOLLOWING_NOREDEMP:
				{
					if (!MG_Calendar::IsBusinessDay(aCal, *this))
					{
						unsigned int vMth = myMonth;
						NextBusinessDay(1, aCal);
						if (vMth != myMonth)
							PreviousBusinessDay(1, aCal);
					}
				}
				break;

			case K_PREVIOUS_PAY:
			case K_PREVIOUS:
				{
					if (!MG_Calendar::IsBusinessDay(aCal, *this))
						PreviousBusinessDay(1, aCal);
				}
				break;

			case K_MODIFIED_PREVIOUS:
			case K_MODIFIED_PREVIOUS_NOREDEMP:
				{
					if (!MG_Calendar::IsBusinessDay(aCal, *this))
					{
						unsigned int vMth = myMonth;
						PreviousBusinessDay(1, aCal);
						if (vMth != myMonth)
							NextBusinessDay(1, aCal);
					}
				}
				break;
			}
		}
		break;
	};

	return *this;
}


