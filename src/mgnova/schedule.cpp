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
						,	myIRIndex(aRight.myIRIndex)
						,	myFreq(aRight.myFreq), myIsDecompound(aRight.myIsDecompound)
						,	myIntAdj(aRight.myIntAdj), myStubRule(aRight.myStubRule)
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

	myIRIndex.Swap(aRight.myIRIndex);

	swap(myFreq, aRight.myFreq);
	swap(myIsDecompound, aRight.myIsDecompound);
	swap(myIntAdj, aRight.myIntAdj);
	swap(myStubRule, aRight.myStubRule);

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
						,	const MG_IRIndex	& aIRIndex
						,	const FREQUENCY_NAME& aFreq
						,	const ADJ_NAME		& aIntAdj
						,	const STUBRULE_NAME	& aStubRule
						,	const bool			& aIsDecompound)
						:	myStDt(aStDt), myEdDt(aEdDt)
						,	myIRIndex(aIRIndex)
						,	myFreq(aFreq), myIsDecompound(aIsDecompound)
						,	myIntAdj(aIntAdj), myStubRule(aStubRule)
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
	myAdjStDt.AddPeriod(K_DAILY, 0, myIRIndex.GetCurrency().myCalendar, myIRIndex.GetAdjRule());

	myAdjEdDt = myEdDt;
	myAdjEdDt.AddPeriod(K_DAILY, 0, myIRIndex.GetCurrency().myCalendar, myIRIndex.GetAdjRule());

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
	MG_utils::GetFromIndexName(myIRIndex.GetIndexName(), vPeriod, vTimes, vIdxFreq);
	vNonAdjEdDt.AddPeriod(myFreq, myNbOfFlows, myIRIndex.GetCurrency().myCalendar, K_FIXED_RULE, true);
	for(int i=myNbOfFlows-1; i>=0; --i)
	{
		vEdDt = vNonAdjEdDt;
		if (myIntAdj == K_ADJUSTED)
			vEdDt.AdjustToBusinessDay(myIRIndex.GetCurrency().myCalendar, myIRIndex.GetAdjRule());

		vStDt = vNonAdjEdDt;
		if (myIntAdj == K_ADJUSTED)
			vStDt.AdjustToBusinessDay(myIRIndex.GetCurrency().myCalendar, myIRIndex.GetAdjRule());
		vStDt.AddPeriod(myFreq, -1, myIRIndex.GetCurrency().myCalendar, myIRIndex.GetAdjRule(), true);

		vRstDt = vStDt;
		if (myIRIndex.GetResetTiming() == K_ARREARS)
			vRstDt = vEdDt;
		vRstDt.AddDays(myIRIndex.GetResetGap(), myIRIndex.GetResetCalendar());

		vPayDt = vEdDt;
		if (myIRIndex.GetPayTiming() == K_ADVANCE)
			vPayDt = vStDt;
		if (myIntAdj == K_ADJUSTED)
			vPayDt.AdjustToBusinessDay(myIRIndex.GetPayCalendar(), myIRIndex.GetAdjRule());

		vIntDay = (unsigned int)vEdDt.BetweenDays(vStDt, myIRIndex.GetDayCount(), false);
		vIntTerm = vEdDt.BetweenDays(vStDt, myIRIndex.GetDayCount(), true);

		myResetDates[i]		= vRstDt;
		myIntStartDates[i]	= vStDt;
		myIntEndDates[i]	= vEdDt;
		myPayDates[i]		= vPayDt;
		myIntDays[i]		= vIntDay;
		myIntTerms[i]		= vIntTerm;

		if (myIRIndex.GetResetTiming() == K_ARREARS)
		{
			vFwdStDt = vRstDt;
			vFwdStDt.NextBusinessDay(myIRIndex.GetCurrency().mySpotDays, myIRIndex.GetCurrency().myCalendar);
		}
		else
		{
			vFwdStDt = vStDt;
			vFwdStDt.NextBusinessDay(0, myIRIndex.GetCurrency().myCalendar);
		}

		vFwdEdDt = vFwdStDt;
		if (myIRIndex.GetResetTiming()==K_ADVANCE && myIsDecompound)
			vFwdEdDt += (vEdDt - vStDt);
		else
		{
			if (myIntAdj==K_ADJUSTED || myIRIndex.GetResetTiming()==K_ADVANCE)
				vFwdEdDt.AddPeriod(vIdxFreq, vTimes*vPeriod, myIRIndex.GetCurrency().myCalendar, myIRIndex.GetAdjRule(), true);
			else
				vFwdEdDt.AddPeriod(vIdxFreq, vTimes*vPeriod, myIRIndex.GetCurrency().myCalendar, K_FIXED_RULE, true);
		}

		myFwdRateStartDates[i] = vFwdStDt;
		myFwdRateEndDates[i] = vFwdEdDt;

		vNonAdjEdDt.AddPeriod(myFreq, -1, myIRIndex.GetCurrency().myCalendar, K_FIXED_RULE, true);
	}
}

