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
#include <memory>

#include "nova/glob/typedef.h"


MG_NAMESPACE_BEGIN


template<class T>
class Singleton
{
private:
	Singleton() {}

public:
	inline static std::shared_ptr<T> Instance()
	{
		if (!myInstance)
			myInstance = std::shared_ptr<T>(new T());

		return myInstance;
	}

private:
	static std::shared_ptr<T> myInstance;
};


MG_NAMESPACE_END
