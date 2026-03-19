/*
 * Copyright			: 2010 by MG
 * Version				: 0.1.20
 * Date					: 02 JAN 2011
 * Purpose				: MG_Object is a base class for almost all classes
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include <iostream>
#include <assert.h>

#include "nova/glob/typedef.h"


MG_NAMESPACE_BEGIN


class MG_Object
{
public:
	MG_Object() {}
	MG_Object(const MG_Object&) = default;
	// self-generated copy-constructor is enough
	virtual ~MG_Object() {}

	MG_Object& operator= (const MG_Object& /*aRight*/) { return *this; }
	virtual MG_Object* Clone() const = 0;

};

class MG_XLObject : public MG_Object
{
	friend class MG_Cache;

public:
	MG_XLObject();
	COPY_CTOR_DECL(MG_XLObject)

	SWAP_DECL(MG_XLObject)

protected:
	std::string	myXLName;
	int			myXLId;
};


MG_NAMESPACE_END

