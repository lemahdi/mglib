#include "mgnova/schedule.h"
#include "mgnova/utils/utils.h"

#include <math.h>


using namespace std;
using namespace MG;


/*
 * Schedule Class
 */
MG_Schedule::MG_Schedule(	const MG_Schedule& aRight)
						:	MG_XLObject(aRight)
						,	myStDt(aRight.myStDt), myEdDt(aRight.myEdDt)
						,	myAdjStDt(aRight.myAdjStDt), myAdjEdDt(aRight.myAdjEdDt)
						,	myCcy(aRight.myCcy), myIndexName(aRight.myIndexName)
						,	myRstCal(aRight.myRstCal), myPayCal(aRight.myPayCal)
						,	myFreq(aRight.myFreq), myDayCount(aRight.myDayCount), myIsDecompound(aRight.myIsDecompound)
						,	myIntAdj(aRight.myIntAdj), myAdjRule(aRight.myAdjRule), myStubRule(aRight.myStubRule)
						,	myRstTiming(aRight.myRstTiming), myPayTiming(aRight.myPayTiming)
						,	myRstGap(aRight.myRstGap), myPayGap(aRight.myPayGap)
						,	myNbOfFlows(aRight.myNbOfFlows)
						,	myResetDates(aRight.myResetDates)
						,	myIntStartDates(aRight.myIntStartDates), myIntEndDates(aRight.myIntEndDates)
						,	myPayDates(aRight.myPayDates)
						,	myIntDays(aRight.myIntDays), myIntTerms(aRight.myIntTerms)
						,	myFwdRateStartDates(aRight.myFwdRateStartDates), myFwdRateEndDates(aRight.myFwdRateEndDates)
{}

void MG_Schedule::Swap(MG_Schedule& aRight)
{
	MG_XLObject::Swap(aRight);

	myStDt.Swap(aRight.myStDt);
	myEdDt.Swap(aRight.myEdDt);
	myAdjStDt.Swap(aRight.myAdjStDt);
	myAdjEdDt.Swap(aRight.myAdjEdDt);

	myCcy.Swap(aRight.myCcy);
	swap(myIndexName, aRight.myIndexName);
	swap(myRstCal, aRight.myRstCal);
	swap(myPayCal, aRight.myPayCal);

	swap(myFreq, aRight.myFreq);
	swap(myDayCount, aRight.myDayCount);
	swap(myIsDecompound, aRight.myIsDecompound);

	swap(myIntAdj, aRight.myIntAdj);
	swap(myAdjRule, aRight.myAdjRule);
	swap(myStubRule, aRight.myStubRule);
	swap(myRstTiming, aRight.myRstTiming);
	swap(myPayTiming, aRight.myPayTiming);
	swap(myRstGap, aRight.myRstGap);
	swap(myPayGap, aRight.myPayGap);

	swap(myNbOfFlows, aRight.myNbOfFlows);
	myResetDates.swap(aRight.myResetDates);
	myIntStartDates.swap(aRight.myIntStartDates);
	myIntEndDates.swap(aRight.myIntEndDates);
	myPayDates.swap(aRight.myPayDates);
	myIntDays.swap(aRight.myIntDays);
	myIntTerms.swap(aRight.myIntTerms);
	myFwdRateStartDates.swap(aRight.myFwdRateStartDates);
	myFwdRateEndDates.swap(aRight.myFwdRateEndDates);
}

MG_Schedule::MG_Schedule(	const MG_Date		& aStDt
						,	const MG_Date		& aEdDt
						,	const MG_Currency	& aCcy
						,	const FREQUENCY_NAME& aFreq
						,	const INDEX_NAME	& aIdxNm
						,	const DAYCOUNT_NAME	& aDayCount
						,	const ADJ_NAME		& aIntAdj
						,	const ADJRULE_NAME	& aAdjRule
						,	const STUBRULE_NAME	& aStubRule
						,	const TIMING_NAME	& aRstTiming
						,	const TIMING_NAME	& aPayTiming
						,	const CALENDAR_NAME	& aRstCal
						,	const CALENDAR_NAME	& aPayCal
						,	const int			& aRstGap
						,	const int			& aPayGap
						,	const bool			& aIsDecompound)
						:	myStDt(aStDt), myEdDt(aEdDt)
						,	myFreq(aFreq), myDayCount(aDayCount), myIsDecompound(aIsDecompound)
						,	myCcy(aCcy), myIndexName(aIdxNm)
						,	myIntAdj(aIntAdj), myAdjRule(aAdjRule), myStubRule(aStubRule)
						,	myRstTiming(aRstTiming), myPayTiming(aPayTiming)
						,	myRstCal(aRstCal), myPayCal(aPayCal)
						,	myRstGap(aRstGap), myPayGap(aPayGap)
{
	myXLName = MG_SCHED_XL_NAME;

	GenerateDates();
}

MG_Schedule::~MG_Schedule()
{}

void MG_Schedule::GenerateDates()
{
	myResetDates.clear();
	myIntStartDates.clear();
	myIntEndDates.clear();
	myPayDates.clear();
	myIntDays.clear();
	myIntTerms.clear();
	myFwdRateStartDates.clear();
	myFwdRateEndDates.clear();

	myAdjStDt = myStDt;
	myAdjStDt.AddPeriod(K_DAILY, 0, myCcy.myCalendar, myAdjRule);

	myAdjEdDt = myEdDt;
	myAdjEdDt.AddPeriod(K_DAILY, 0, myCcy.myCalendar, myAdjRule);

	MG_Date v1stStDt = myAdjStDt;
	if (myIntAdj == K_UNADJUSTED)
		v1stStDt = myStDt;

	myNbOfFlows = size_t((myEdDt-myStDt)/365. * myFreq);
	myResetDates.resize(myNbOfFlows);
	myIntStartDates.resize(myNbOfFlows);
	myIntEndDates.resize(myNbOfFlows);
	myPayDates.resize(myNbOfFlows);
	myIntDays.resize(myNbOfFlows);
	myIntTerms.resize(myNbOfFlows);
	myFwdRateStartDates.resize(myNbOfFlows);
	myFwdRateEndDates.resize(myNbOfFlows);

	MG_Date vNonAdjEdDt(myStDt);
	MG_Date vEdDt, vStDt, vRstDt, vPayDt;
	unsigned int vIntDay;
	double vIntTerm;
	MG_Date vFwdStDt, vFwdEdDt;
	int vPeriod(0);
	unsigned int vTimes(0);
	FREQUENCY_NAME vIdxFreq(NB_FREQUENCIES);
	MG_utils::GetFromIndexName(myIndexName, vPeriod, vTimes, vIdxFreq);
	vNonAdjEdDt.AddPeriod(myFreq, myNbOfFlows, myCcy.myCalendar, K_FIXED_RULE, true);
	for(int i=myNbOfFlows-1; i>=0; --i)
	{
		vEdDt = vNonAdjEdDt;
		if (myIntAdj == K_ADJUSTED)
			vEdDt.AdjustToBusinessDay(myCcy.myCalendar, myAdjRule);

		vStDt = vNonAdjEdDt;
		if (myIntAdj == K_ADJUSTED)
			vStDt.AdjustToBusinessDay(myCcy.myCalendar, myAdjRule);
		vStDt.AddPeriod(myFreq, -1, myCcy.myCalendar, myAdjRule, true);

		vRstDt = vStDt;
		if (myRstTiming == K_ARREARS)
			vRstDt = vEdDt;
		vRstDt.AddDays(myRstGap, myRstCal);

		vPayDt = vEdDt;
		if (myPayTiming == K_ADVANCE)
			vPayDt = vStDt;
		if (myIntAdj == K_ADJUSTED)
			vPayDt.AdjustToBusinessDay(myPayCal, myAdjRule);

		vIntDay = (unsigned int)vEdDt.BetweenDays(vStDt, myDayCount, false);
		vIntTerm = vEdDt.BetweenDays(vStDt, myDayCount, true);

		myResetDates[i]		= vRstDt;
		myIntStartDates[i]	= vStDt;
		myIntEndDates[i]	= vEdDt;
		myPayDates[i]		= vPayDt;
		myIntDays[i]		= vIntDay;
		myIntTerms[i]		= vIntTerm;

		if (myRstTiming == K_ARREARS)
		{
			vFwdStDt = vRstDt;
			vFwdStDt.NextBusinessDay(myCcy.mySpotDays, myCcy.myCalendar);
		}
		else
		{
			vFwdStDt = vStDt;
			vFwdStDt.NextBusinessDay(0, myCcy.myCalendar);
		}

		vFwdEdDt = vFwdStDt;
		if (myRstTiming==K_ADVANCE && myIsDecompound)
			vFwdEdDt += (vEdDt - vStDt);
		else
		{
			if (myIntAdj==K_ADJUSTED || myRstTiming==K_ADVANCE)
				vFwdEdDt.AddPeriod(vIdxFreq, vTimes*vPeriod, myCcy.myCalendar, myAdjRule, true);
			else
				vFwdEdDt.AddPeriod(vIdxFreq, vTimes*vPeriod, myCcy.myCalendar, K_FIXED_RULE, true);
		}

		myFwdRateStartDates[i] = vFwdStDt;
		myFwdRateEndDates[i] = vFwdEdDt;

		vNonAdjEdDt.AddPeriod(myFreq, -1, myCcy.myCalendar, K_FIXED_RULE, true);
	}
}

