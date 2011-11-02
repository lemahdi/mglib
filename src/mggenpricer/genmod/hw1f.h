/*
 * Copyright			: 2011 by MG
 * File					: hw1f.h
 * Version				: 0.1.23
 * Last changed			: 01 NOV 2011
 * Purpose				: MG_HW1F is the Hull & White One Factor model
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "mggenpricer/genmod/irpricingmodel.h"


MG_NAMESPACE_BEGIN


/* Base class for models */
class MG_HW1F : public MG_IRPricingModel
{
public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_HW1F)

	ASSIGN_OPERATOR(MG_HW1F)
	SWAP_DECL(MG_HW1F)
	CLONE_METHOD(MG_HW1F)

	MG_HW1F(void);

	virtual ~MG_HW1F(void);

private:
};


MG_NAMESPACE_END
