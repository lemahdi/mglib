/*
 * Copyright			: 2011 by MG
 * File					: irpricingmodel.h
 * Version				: 0.1.23
 * Last changed			: 01 NOV 2011
 * Purpose				: MG_IRPricingModel is a base class for interest rate models
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "mggenpricer/genmod/pricingmodel.h"


MG_NAMESPACE_BEGIN


/* Base class for models */
class MG_IRPricingModel : public MG_PricingModel
{
public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_IRPricingModel)

	ASSIGN_OPERATOR(MG_IRPricingModel)
	SWAP_DECL(MG_IRPricingModel)
	CLONE_METHOD(MG_IRPricingModel)

	MG_IRPricingModel(void);

	virtual ~MG_IRPricingModel(void);

private:
};


MG_NAMESPACE_END
