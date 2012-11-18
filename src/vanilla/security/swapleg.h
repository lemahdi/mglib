/*
 * Copyright			: 2012 by MG
 * Version				: 0.1.23
 * Date					: 01 JAN 2012
 * Purpose				: MG_SwapLeg is an interest rate coupon leg
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "vanilla/security/irsecurity.h"

#include <vector>


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

	MG_SwapLeg	(	const MG_GenericDate	& aSt
				,	const MG_GenericDate	& aEd
				,	const RCVPAY_NAME		& aRcvPay = K_RCV
				,	const MG_IRIndex		& aIRIndex = MG_IRIndex()
				,	const double			& aSpdOrRate = 0.
				,	const NX_NAME			& aNX = K_NX_NONE
				,	const MG_TermStructure	& aNotional = MG_TermStructure(100.));
	MG_SwapLeg	(	const MG_Schedule		& aSched
				,	const RCVPAY_NAME		& aRcvPay = K_RCV
				,	const double			& aSpdOrRate = 0.
				,	const NX_NAME			& aNX = K_NX_NONE
				,	const MG_TermStructure	& aNotional = MG_TermStructure(100.));

public:
	/* State */
	inline const MG_IRIndex& GetIRIndex(void) const { return myIRIndex; }
	inline bool IsFixed(void) const { return myIRIndex.GetIndexName() == K_FIXED; }
	inline bool IsFloat(void) const { return myIRIndex.GetIndexName() != K_FIXED; }
	inline void Spread(const double& aSpread) { mySpreadOrRate = aSpread; }

	/* Engine */
	double	Level			(const MG_Model& aMdl) const;

	double	ImpliedSpread	(const MG_Model& , const size_t& , const double& ) const { return 0.; }
	double	ImpliedRate		(const MG_Model& , const size_t& , const double& ) const { return 0.; }

	void	PrePricing		(const MG_Model& aMdl);
	double	Price			(void) const;


private:
	RCVPAY_NAME	myRcvPay;
	MG_IRIndex myIRIndex;

	double mySpreadOrRate;

	std::vector<double> myRawFwd;

};


MG_NAMESPACE_END
