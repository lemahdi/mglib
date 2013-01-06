/*
 * Copyright			: 2010 by MG
 * Version				: 0.1.11
 * Date					: 18 DEC 2010
 * Purpose				: Singleton is an implementation of the singleton
 *						design pattern
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include <iostream>

#include "nova/glob/typedef.h"
#include "nova/patterns/countedptr.hpp"


MG_NAMESPACE_BEGIN


template<class T>
class Singleton
{
private:
	Singleton(void) {}

public:
	inline static CountedPtr< T > Instance(void)
	{
		if (!myInstance)
			Singleton::myInstance = CountedPtr< T >(new T);

		return Singleton::myInstance;
	}

private:
	static CountedPtr< T > myInstance;
};


MG_NAMESPACE_END
