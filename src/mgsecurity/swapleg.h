/*
 * Copyright			: 2012 by MG
 * File					: swapleg.h
 * Version				: 0.1.23
 * Last changed			: 01 JAN 2012
 * Purpose				: MG_SwapLeg is an interest rate coupon leg
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "mgsecurity/security.h"
#include "mgnova/wrapper/vector.h"


MG_NAMESPACE_BEGIN


/* Interest Leg Coupon Leg */
class MG_SwapLeg : public MG_IRSecurity
{
public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_SwapLeg)

	ASSIGN_OPERATOR(MG_SwapLeg)
	CLONE_METHOD(MG_SwapLeg)
	SWAP_DECL(MG_SwapLeg)

	virtual ~MG_SwapLeg(void);

	MG_SwapLeg	(	const MG_Date		& aStDt
				,	const MG_Date		& aEdDt
				,	const RCVPAY_NAME	& aRcvPay = K_RCV
				,	const MG_IRIndex	& aIRIndex = MG_IRIndex());
	MG_SwapLeg	(	const MG_GenericDate& aSt
				,	const MG_GenericDate& aEd
				,	const RCVPAY_NAME	& aRcvPay = K_RCV
				,	const MG_IRIndex	& aIRIndex = MG_IRIndex());
	MG_SwapLeg	(	const MG_Schedule& aSched
				,	const RCVPAY_NAME& aRcvPay = K_RCV);

public:
	/* State */
	inline const MG_IRIndex& GetIRIndex(void) const { return mySchedule.GetIRIndex(); }

	/* Engine */
	void	PrePricing	(const MG_Model& aMdl);
	double	Price		(void) const;

private:
	RCVPAY_NAME	myRcvPay;
	MG_Vector myRawFwd;
	MG_Vector myDfs;

};

MG_NAMESPACE_END
