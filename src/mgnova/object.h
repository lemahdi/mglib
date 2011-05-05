/*
 * Copyright			: 2010 by MG
 * File					: object.h
 * Version				: 0.1.20
 * Last changed			: 02 JAN 2011
 * Purpose				: MG_Object is a base class for almost all classes
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include <iostream>
#include <assert.h>

#include "mgnova/typedef.h"
#include "mgnova/patterns/countedptr.hpp"


MG_NAMESPACE_BEGIN


class MG_Object
{
public:
	MG_Object(void) {}
	// self-generated copy-constructor is enough
	virtual ~MG_Object(void) {}

	MG_Object& operator= (const MG_Object& /*aRight*/) {}
	virtual MG_Object* Clone(void) = 0;

};

class MG_XLObject : public MG_Object
{
	friend class MG_Cache;

public:
	MG_XLObject(void);
	COPY_CTOR_DECL(MG_XLObject)

	ASSIGN_OPERATOR(MG_XLObject)
	SWAP_DECL(MG_XLObject)

protected:
	std::string	myXLName;
	int			myXLId;
};

class MG_Call : public MG_XLObject
{
public:
	MG_Call(const double& aK, const double& aMat, const double& aFwd)
		: myStrike(aK), myMaturity(aMat), myFwd(aFwd) { myXLName = "CALL"; }
	virtual MG_Object* Clone(void) { return NULL; }

public:
	double myStrike;
	double myMaturity;
	double myFwd;
};


MG_NAMESPACE_END

