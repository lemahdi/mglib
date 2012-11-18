/*
 * Copyright			: 2011 by MG
 * Version				: 0.1.23
 * Date					: 01 NOV 2011
 * Purpose				: MG_PricingModel is a base class for stochastic models
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "nova/glob/object.h"
#include "nova/patterns/countedptr.hpp"
#include "genpricer/numerics/mcmethod.h"


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

	/* accessors */
	inline const MG_NumericalMethodPtr& NumMethod(void) const { return myNumMethod; }
	inline void NumMethod(const MG_NumericalMethodPtr& aNumMethod) { myNumMethod = aNumMethod; }

private:
	MG_NumericalMethodPtr myNumMethod;
};


MG_NAMESPACE_END
