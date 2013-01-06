/*
 * Copyright			: 2012 by MG
 * Version				: 0.1.23
 * Date					: 23 OCT 2012
 * Purpose				: MG_GenPricer is the main class for generic pricing
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "nova/glob/object.h"
#include "nova/patterns/countedptr.hpp"


MG_NAMESPACE_BEGIN


class MG_GenSecurity;
class MG_PricingModel;


/* Base class for models */
class MG_GenPricer : public MG_XLObject
{
public:
	/* Constructors / Destructor */
	virtual ~MG_GenPricer(void);

	ASSIGN_OPERATOR(MG_GenPricer)
	CLONE_METHOD(MG_GenPricer)
	SWAP_DECL(MG_GenPricer)

	MG_GenPricer(const MG_GenSecurityPtr& aGenSec, const MG_PricingModelPtr& aPricingMdl);

	/* Engine */
	void Price(void) const;

private:
	MG_GenSecurityPtr	myGenSec;
	MG_PricingModelPtr	myPricingModel;
};


MG_NAMESPACE_END
