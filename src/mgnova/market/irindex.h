/*
 * Copyright			: 2012 by MG
 * File					: irindex.h
 * Version				: 0.1.23
 * Last changed			: 01 JAN 2012
 * Purpose				: MG_IRIndex is an interest rate index descriptor
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "mgnova/glob/object.h"
#include "mgnova/glob/argconvdef.h"
#include "mgnova/market/currency.h"


MG_NAMESPACE_BEGIN


/* Interest Leg Coupon Leg */
class MG_IRIndex : public MG_XLObject
{
public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_IRIndex)

	ASSIGN_OPERATOR(MG_IRIndex)
	CLONE_METHOD(MG_IRIndex)
	SWAP_DECL(MG_IRIndex)

	virtual ~MG_IRIndex(void);

	MG_IRIndex	(	const INDEX_NAME	& aIndexName	= INDEX_NAME_DEF
				,	const MG_Currency	& aCcy			= MG_Currency(CURRENCY_NAME_DEF)
				,	const DAYCOUNT_NAME	& aDayCount		= K_ACT_365
				,	const ADJRULE_NAME	& aAdjRule		= K_MODIFIED_FOLLOWING
				,	const TIMING_NAME	& aRstTiming	= K_ADVANCE
				,	const TIMING_NAME	& aPayTiming	= K_ARREARS
				,	const CALENDAR_NAME	& aRstCal		= CALENDAR_NAME_DEF
				,	const CALENDAR_NAME	& aPayCal		= CALENDAR_NAME_DEF
				,	const int			& aRstGap		= -2
				,	const int			& aPayGap		= 0);

public:
	/* State */
	inline const INDEX_NAME&	GetIndexName	(void) const { return myIndexName; }
	inline const MG_Currency&	GetCurrency		(void) const { return myCcy; }
	inline const DAYCOUNT_NAME&	GetDayCount		(void) const { return myDayCount; }
	inline const ADJRULE_NAME&	GetAdjRule		(void) const { return myAdjRule; }
	inline const TIMING_NAME&	GetResetTiming	(void) const { return myRstTiming; }
	inline const TIMING_NAME&	GetPayTiming	(void) const { return myPayTiming; }
	inline const int&			GetResetGap		(void) const { return myRstGap; }
	inline const int&			GetPayGap		(void) const { return myPayGap; }
	inline const CALENDAR_NAME&	GetResetCalendar(void) const { return myRstCal; }
	inline const CALENDAR_NAME&	GetPayCalendar	(void) const { return myPayCal; }

private:
	INDEX_NAME		myIndexName;
	MG_Currency		myCcy;
	DAYCOUNT_NAME	myDayCount;
	ADJRULE_NAME	myAdjRule;
	TIMING_NAME		myRstTiming;
	TIMING_NAME		myPayTiming;
	int				myRstGap;
	int				myPayGap;
	CALENDAR_NAME	myRstCal;
	CALENDAR_NAME	myPayCal;

	FREQUENCY_NAME	myFrequency;

};

MG_NAMESPACE_END
