
/*
 * Copyright			: 2011 by MG
 * File					: schedule.h
 * Version				: 0.1.23
 * Last changed			: 31 DEC 2011
 * Purpose				: MG_Schedule is a class for dealing with interest and libor dates
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "mgnova/typedef.h"
#include "mgnova/object.h"
#include "mgnova/argconvdef.h"
#include "mgnova/date.h"
#include "mgnova/currency.h"

#include <vector>


MG_NAMESPACE_BEGIN


/* Solver Base Class */
class MG_Schedule : public MG_XLObject
{
public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_Schedule)

	ASSIGN_OPERATOR(MG_Schedule)
	CLONE_METHOD(MG_Schedule)
	SWAP_DECL(MG_Schedule)

	MG_Schedule	(	const MG_Date		& aStDt
				,	const MG_Date		& aEdDt
				,	const MG_Currency	& aCcy
				,	const FREQUENCY_NAME& aFreq
				,	const INDEX_NAME	& aIdxNm
				,	const DAYCOUNT_NAME	& aDayCount		= K_ACT_365
				,	const ADJ_NAME		& aIntAdj		= K_ADJUSTED
				,	const ADJRULE_NAME	& aAdjRule		= K_MODIFIED_FOLLOWING
				,	const STUBRULE_NAME	& aStubRule		= K_SHORT_START
				,	const TIMING_NAME	& aRstTiming	= K_ADVANCE
				,	const TIMING_NAME	& aPayTiming	= K_ARREARS
				,	const CALENDAR_NAME	& aRstCal		= NB_CALENDARS
				,	const CALENDAR_NAME	& aPayCal		= NB_CALENDARS
				,	const int			& aRstGap		= -2
				,	const int			& aPayGap		= 0
				,	const bool			& aIsDecompound	= true
				);

	virtual ~MG_Schedule(void);

	/* State */
	inline const std::vector<MG_Date>		& GetResetDates			(void) const { return myResetDates; }
	inline const std::vector<MG_Date>		& GetIntStartDates		(void) const { return myIntStartDates; }
	inline const std::vector<MG_Date>		& GetIntEndDates		(void) const { return myIntEndDates; }
	inline const std::vector<MG_Date>		& GetPayDates			(void) const { return myPayDates; }
	inline const std::vector<unsigned int>	& GetIntDays			(void) const { return myIntDays; }
	inline const std::vector<double>		& GetIntTerms			(void) const { return myIntTerms; }
	inline const std::vector<MG_Date>		& GetFwdRateStartDates	(void) const { return myFwdRateStartDates; }
	inline const std::vector<MG_Date>		& GetFwdRateEndDates	(void) const { return myFwdRateEndDates; }

	/* Engine */
	void GenerateDates(void);

private:
	MG_Date			myStDt;
	MG_Date			myEdDt;

	MG_Date			myAdjStDt;
	MG_Date			myAdjEdDt;

	INDEX_NAME		myIndexName;
	MG_Currency		myCcy;
	CALENDAR_NAME	myRstCal;
	CALENDAR_NAME	myPayCal;

	FREQUENCY_NAME	myFreq;
	DAYCOUNT_NAME	myDayCount;
	bool			myIsDecompound;

	ADJ_NAME		myIntAdj;
	ADJRULE_NAME	myAdjRule;
	STUBRULE_NAME	myStubRule;
	TIMING_NAME		myRstTiming;
	TIMING_NAME		myPayTiming;

	int				myRstGap;
	int				myPayGap;


	size_t myNbOfFlows;
	std::vector<MG_Date> myResetDates;
	std::vector<MG_Date> myIntStartDates;
	std::vector<MG_Date> myIntEndDates;
	std::vector<MG_Date> myPayDates;

	std::vector<unsigned int> myIntDays;
	std::vector<double> myIntTerms;

	std::vector<MG_Date> myFwdRateStartDates;
	std::vector<MG_Date> myFwdRateEndDates;
};


MG_NAMESPACE_END

