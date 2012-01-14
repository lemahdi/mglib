/*
 * Copyright			: 2010 by MG
 * File					: singleton.hpp
 * Version				: 0.1.11
 * Last changed			: 18 DEC 2010
 * Purpose				: Singleton is an implementation of the singleton
 *						design pattern
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include <iostream>

#include "mgnova/glob/typedef.h"
#include "mgnova/patterns/countedptr.hpp"


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
