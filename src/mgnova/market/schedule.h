
/*
 * Copyright			: 2011 by MG
 * Version				: 0.1.23
 * Date					: 31 DEC 2011
 * Purpose				: MG_Schedule is a class for dealing with interest and libor dates
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "mgnova/glob/typedef.h"
#include "mgnova/glob/object.h"
#include "mgnova/glob/argconvdef.h"
#include "mgnova/glob/genericdate.h"
#include "mgnova/market/irindex.h"

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

	MG_Schedule(void) {}
	MG_Schedule	(	const MG_Date		& aStDt
				,	const MG_Date		& aEdDt
				,	const MG_IRIndex	& aIRIndex
				,	const FREQUENCY_NAME& aFreq
				,	const ADJ_NAME		& aIntAdj		= K_ADJUSTED
				,	const STUBRULE_NAME	& aStubRule		= K_SHORT_START
				,	const bool			& aIsDecompound	= true
				);
	MG_Schedule	(	const MG_GenericDate& aStDt
				,	const MG_GenericDate& aEdDt
				,	const MG_IRIndex	& aIRIndex
				,	const FREQUENCY_NAME& aFreq
				,	const ADJ_NAME		& aIntAdj		= K_ADJUSTED
				,	const STUBRULE_NAME	& aStubRule		= K_SHORT_START
				,	const bool			& aIsDecompound	= true
				);

	virtual ~MG_Schedule(void);

	/* State */
	inline const MG_IRIndex					& GetIRIndex			(void) const { return myIRIndex; }
	inline const MG_Date					& GetStartDate			(void) const { return myStDt; }
	inline const MG_Date					& GetEndDate			(void) const { return myEdDt; }
	inline const std::vector<MG_Date>		& GetResetDates			(void) const { return myResetDates; }
	inline const std::vector<MG_Date>		& GetIntStartDates		(void) const { return myIntStartDates; }
	inline const std::vector<MG_Date>		& GetIntEndDates		(void) const { return myIntEndDates; }
	inline const std::vector<MG_Date>		& GetPayDates			(void) const { return myPayDates; }
	inline const std::vector<unsigned int>	& GetIntDays			(void) const { return myIntDays; }
	inline const std::vector<double>		& GetIntTerms			(void) const { return myIntTerms; }
	inline const std::vector<MG_Date>		& GetFwdRateStartDates	(void) const { return myFwdRateStartDates; }
	inline const std::vector<MG_Date>		& GetFwdRateEndDates	(void) const { return myFwdRateEndDates; }

	/* Engine */
	void InterpretDates(const MG_Date& aAsOf);
	void GenerateDates(void);

private:
	MG_GenericDate	myGenStDt;
	MG_GenericDate	myGenEdDt;

	MG_Date			myStDt;
	MG_Date			myEdDt;

	MG_Date			myAdjStDt;
	MG_Date			myAdjEdDt;

	MG_IRIndex		myIRIndex;

	FREQUENCY_NAME	myFreq;
	bool			myIsDecompound;

	ADJ_NAME		myIntAdj;
	STUBRULE_NAME	myStubRule;

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

