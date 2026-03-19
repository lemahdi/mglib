/*
 * Copyright			: 2010 by MG
 * Version				: 0.1.11
 * Date					: 18 DEC 2010
 * Purpose				: Singleton is an implementation of the singleton
 *						design pattern
 * Author				: MM Akkouh
 * Notes				: Uses the Meyers' Singleton pattern (C++11 guaranteed
 *						  thread-safe local static initialization).
 *						  Returns T* to preserve existing call-site syntax.
 */


#pragma once


#include "nova/glob/typedef.h"


MG_NAMESPACE_BEGIN


template<class T>
class Singleton
{
private:
	Singleton() {}

public:
	static T* Instance()
	{
		static T instance;
		return &instance;
	}
};


MG_NAMESPACE_END
