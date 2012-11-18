/*
 * Copyright			: 2012 by MG
 * Version				: 0.1.23
 * Date					: 07 OCT 2012
 * Purpose				: MG_NumericalMethod is a base class for numerical methods
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "nova/glob/object.h"


MG_NAMESPACE_BEGIN


/* Base class for models */
class MG_NumericalMethod : public MG_XLObject
{
public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_NumericalMethod)

	ASSIGN_OPERATOR(MG_NumericalMethod)
	SWAP_DECL(MG_NumericalMethod)
	CLONE_METHOD(MG_NumericalMethod)

	MG_NumericalMethod(void);

	virtual ~MG_NumericalMethod(void);
};


MG_NAMESPACE_END
