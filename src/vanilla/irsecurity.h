/*
 * Copyright			: 2012 by MG
 * Version				: 0.1.23
 * Date					: 16 SEP 2012
 * Purpose				: MG_IRSecurity is a base class for creating IR securities
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "mgnova/market/schedule.h"
#include "vanilla/security.h"


MG_NAMESPACE_BEGIN


/* Interest Rate base class security */
class MG_IRSecurity : public MG_Security
{
public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_IRSecurity)

	ASSIGN_OPERATOR(MG_IRSecurity)
	//CLONE_METHOD(MG_IRSecurity)
	SWAP_DECL(MG_IRSecurity)

	virtual ~MG_IRSecurity(void);

	MG_IRSecurity(void) {}
	MG_IRSecurity(const MG_Schedule& aSched);

public:
	/* State */
	inline const MG_Schedule& GetSchedule	(void) const { return mySchedule; }
	inline const MG_Date& GetStartDate		(void) const { return mySchedule.GetStartDate(); }
	inline const MG_Date& GetEndDate		(void) const { return mySchedule.GetEndDate(); }

protected:
	/* Engine */
	void GenerateSchedule(const MG_GenericDate& aStDt, const MG_GenericDate& aEdDt, const MG_IRIndex& aIdx);
	virtual double ImpliedSpread(void) const = 0;

protected:
	MG_Schedule mySchedule;

};

MG_NAMESPACE_END
