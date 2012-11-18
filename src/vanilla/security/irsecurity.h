/*
 * Copyright			: 2012 by MG
 * Version				: 0.1.23
 * Date					: 16 SEP 2012
 * Purpose				: MG_IRSecurity is a base class for creating IR securities
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "nova/market/schedule.h"
#include "vanilla/security/security.h"


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

	MG_IRSecurity	(	const NX_NAME& aNX = K_NX_NONE
					,	const MG_TermStructure& aNotional = MG_TermStructure(100.));
	MG_IRSecurity	(	const MG_Schedule& aSched
					,	const NX_NAME& aNX = K_NX_NONE
					,	const MG_TermStructure& aNotional = MG_TermStructure(100.));

public:
	/* State */
	inline const MG_Schedule& GetSchedule	(void) const { return mySchedule; }
	inline const MG_Date& GetStartDate		(void) const { return mySchedule.GetStartDate(); }
	inline const MG_Date& GetEndDate		(void) const { return mySchedule.GetEndDate(); }

	/* Engine */
	void GenerateSchedule(const MG_GenericDate& aStDt, const MG_GenericDate& aEdDt, const MG_IRIndex& aIdx);

public:
	virtual double ImpliedSpread(const MG_Model& aMdl, const size_t& aLegId, const double& aPrice = 0.) const = 0;
	virtual double ImpliedRate	(const MG_Model& aMdl, const size_t& aLegId, const double& aPrice = 0.) const = 0;
	
	void	PrePricing		(const MG_Model& aMdl);

protected:
	MG_Schedule mySchedule;
	NX_NAME myNX;

};


MG_NAMESPACE_END
