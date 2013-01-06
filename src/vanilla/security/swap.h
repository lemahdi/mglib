/*
 * Copyright			: 2012 by MG
 * Version				: 0.1.23
 * Date					: 18 NOV 2012
 * Purpose				: MG_Swap is an interest rate swap
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "vanilla/security/swapleg.h"


MG_NAMESPACE_BEGIN


/* Interest Leg Coupon Leg */
class MG_Swap : public MG_IRSecurity
{
public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_Swap)

	ASSIGN_OPERATOR(MG_Swap)
	CLONE_METHOD(MG_Swap)
	SWAP_DECL(MG_Swap)

	virtual ~MG_Swap(void);

	MG_Swap	(	const MG_SwapLeg	& aLeg1
			,	const MG_SwapLeg	& aLeg2
			,	const RCVPAY_NAME	& aRcvPay = K_RCV);

public:
	/* State */
	inline const MG_SwapLeg& Leg1(void) const { return myLeg1; }
	inline const MG_SwapLeg& Leg2(void) const { return myLeg2; }
	const MG_SwapLeg& FixedLeg(void) const;
	const MG_SwapLeg& FloatLeg(void) const;

	/* Engine */
	double SwapRate			(const MG_Model& aMdl) const;
	double ImpliedSpread	(const MG_Model& aMdl, const size_t& aLegId, const double& aPrice = 0.) const;
	double ImpliedRate		(const MG_Model& aMdl, const size_t& aLegId, const double& aPrice = 0.) const;

	void	PrePricing	(const MG_Model& aMdl);
	double	Price		(void) const;

private:
	RCVPAY_NAME	myRcvPay;
	MG_SwapLeg myLeg1;
	MG_SwapLeg myLeg2;

};


MG_NAMESPACE_END
