/*
 * Copyright			: 2012 by MG
 * Version				: 0.1.23
 * Date					: 16 JAN 2012
 * Purpose				: MG_Fra is a Forward Rate Agreement
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "mgsecurity/irsecurity.h"


MG_NAMESPACE_BEGIN


/* Forward Rate Agreement */
class MG_Fra : public MG_IRSecurity
{
public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_Fra)

	ASSIGN_OPERATOR(MG_Fra)
	CLONE_METHOD(MG_Fra)
	SWAP_DECL(MG_Fra)

	virtual ~MG_Fra(void);

	MG_Fra	(	const MG_GenericDate& aSt
			,	const MG_GenericDate& aEd
			,	const RCVPAY_NAME	& aRcvPay = K_RCV
			,	const MG_IRIndex	& aIRIndex = MG_IRIndex()
			,	const double		& aFixedRate = 0.);

public:
	/* State */
	inline const MG_IRIndex& GetIRIndex(void) const { return mySchedule.GetIRIndex(); }

	/* Engine */
	double	ImpliedSpread(void) const { return 0.; }
	void	PrePricing	(const MG_Model& aMdl);
	double	Price		(void) const;

private:
	RCVPAY_NAME	myRcvPay;
	MG_IRIndex myIRIndex;
	double myFixedRate;

	double myRawFwd;

};

MG_NAMESPACE_END
