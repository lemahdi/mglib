/*
 * Copyright			: 2012 by MG
 * Version				: 0.1.23
 * Date					: 30 SEP 2012
 * Purpose				: MG_Swaption is a Swaption option
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "vanilla/security/option.h"
#include "vanilla/security/swap.h"
#include "nova/glob/genericdate.h"


MG_NAMESPACE_BEGIN


/* Cap / Floor */
class MG_Swaption : public MG_VanillaOption
{
public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_Swaption)

	ASSIGN_OPERATOR(MG_Swaption)
	CLONE_METHOD(MG_Swaption)
	SWAP_DECL(MG_Swaption)

	virtual ~MG_Swaption(void);

	MG_Swaption	(	const MG_GenericDate& aMatDt
				,	const MG_SwapPtr	& aUnderlying
				,	const CALLPUT_NAME	& aCallPut
				,	const double		& aStrike);

public:
	/* State */
	MG_SwapPtr Swap(void) { return myUnderlying; }
	/* Engine */
	void	PrePricing	(const MG_Model& aMdl);
	double	Price		(void) const;

private:
	MG_GenericDate myMatDt;
	CALLPUT_NAME myCallPut;

};

MG_NAMESPACE_END
