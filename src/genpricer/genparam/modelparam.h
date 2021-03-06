/*
 * Copyright			: 2011 by MG
 * Version				: 0.1.23
 * Date					: 01 NOV 2011
 * Purpose				: MG_ModelParam is a base class for pricing models parameters
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "nova/glob/object.h"
#include "nova/glob/argconvdef.h"


MG_NAMESPACE_BEGIN


/* Base class for models */
class MG_ModelParam : public MG_XLObject
{
public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_ModelParam)

	ASSIGN_OPERATOR(MG_ModelParam)
	SWAP_DECL(MG_ModelParam)
	CLONE_METHOD(MG_ModelParam)

	MG_ModelParam(void);

	virtual ~MG_ModelParam(void);

private:
	MODELPARAM_TYPE myType;
};


MG_NAMESPACE_END
