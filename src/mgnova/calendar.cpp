#include "mgnova/calendar.h"
#include "mgnumerical/random.h"

#include <algorithm>
#include <math.h>
#include <list>


using namespace std;
using namespace MG;


/*
 * Static initializations
 */
template<>
map<CURRENCY_NAME,MG_Calendar*> MG_RCalendar::myInstance = map<CURRENCY_NAME,MG_Calendar*>();

map<CURRENCY_NAME,vector<unsigned int> > MG_Calendar::ourCalendars = map<CURRENCY_NAME,vector<unsigned int> >();

CURRENCY_NAME MG_Calendar::ourDefaultCcy = EUR;

void MG_Calendar::Init(void)
{
	unsigned int vNotBusinessDays = 30;
	MG_RandomPtr vRand = MG_RandomPtr(new MG_Random(MT19937));
	vector<double> vRandomUniforms(vNotBusinessDays);
	for(size_t i=0; i<vNotBusinessDays; ++i)
		vRandomUniforms[i] = vRand->DrawUniformInt(366);
	pair<CURRENCY_NAME,vector<unsigned int> > vPairs;
	for(unsigned int i=0; i<NB_CURRENCIES; i++)
	{
		list<unsigned int> vRandomDaysL;
		vRandomDaysL.resize(vNotBusinessDays);
		transform(vRandomUniforms.begin(), vRandomUniforms.end(), vRandomDaysL.begin(), ::floorf);
		vRandomDaysL.sort();
		vRandomDaysL.unique();
		vector<unsigned int> vRandomDaysV;
		vRandomDaysV.resize(vRandomDaysL.size());
		copy(vRandomDaysL.begin(), vRandomDaysL.end(), vRandomDaysV.begin());
		vPairs = pair<CURRENCY_NAME,vector<unsigned int> >((CURRENCY_NAME)i,vRandomDaysV);
		ourCalendars.insert(vPairs);
	}

	for(int i=0; i<NB_CURRENCIES; i++)
		MG_RCalendar::Instance((CURRENCY_NAME)i);
}


/*
 * Default constructor
 */
MG_Calendar::MG_Calendar(void) : myHolidays(ourCalendars[ourDefaultCcy])
{}

/*
 * Constructor by currency
 */
MG_Calendar::MG_Calendar(	const CURRENCY_NAME& aCcyName)
						:	myHolidays(ourCalendars[aCcyName])
{}

void MG_Calendar::AddPeriod(const unsigned int& aNbDays, MG_Date& aDate) const
{
	unsigned int vDayOfYear = aDate.GetDayOfYear() + aNbDays;
	unsigned int vAddedDays = 0;
	vector<unsigned int>::const_iterator itVect = find(myHolidays.begin(), myHolidays.end(), vDayOfYear);
	while (itVect != myHolidays.end())
	{
		vAddedDays++;
		itVect = find(myHolidays.begin(), myHolidays.end(), vDayOfYear+vAddedDays);
	}
	aDate.operator ++(aNbDays+vAddedDays);
}