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
	MG_Object(const MG_Object& /*aRight*/) {}
	MG_Object& operator= (const MG_Object& /*aRight*/) {}
	virtual MG_Object* Clone(void) = 0;//{ return new MG_Object(*this); }
	virtual ~MG_Object(void) {}

};

class MG_XLObject : public MG_Object
{
	friend class MG_Cache;

public:
	MG_XLObject(void) : myXLId(NON_PERSISTENT_XL_OBJECT) {}

protected:
	std::string	myXLName;
	int			myXLId;
};


MG_NAMESPACE_END

