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

#include "typedef.h"


MG_NAMESPACE_BEGIN


template<class T>
class Singleton
{
private:
	Singleton(void) {}

public:
	static T* Instance(void)
	{
		if (!myInstance)
			Singleton::myInstance = new T;

		return Singleton::myInstance;
	}

	static void Release(void)
	{
		delete Singleton::myInstance;
		Singleton::myInstance = NULL;
	}

private:
	static T* myInstance;
};


MG_NAMESPACE_END
