/*
 * Copyright			: 2011 by MG
 * Version				: 0.1.23
 * Date					: 01 NOV 2011
 * Purpose				: MG_PricingModel is a base class for stochastic models
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "mgnova/glob/object.h"


MG_NAMESPACE_BEGIN


/* Base class for models */
class MG_PricingModel : public MG_XLObject
{
public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_PricingModel)

	ASSIGN_OPERATOR(MG_PricingModel)
	SWAP_DECL(MG_PricingModel)
	CLONE_METHOD(MG_PricingModel)

	MG_PricingModel(void);

	virtual ~MG_PricingModel(void);

private:
};


MG_NAMESPACE_END
