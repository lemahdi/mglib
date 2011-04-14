/*
 * Copyright			: 2011 by MG
 * File					: repository.hpp
 * Version				: 0.1.21
 * Last changed			: 12 MAR 2011
 * Purpose				: Repositoy is an implementation of the repository
 *						design pattern
 * Author				: MM Akkouh
 * Notes				: This is a map of Singleton of the same class
 */


#pragma once


#include <iostream>
#include <map>

#include "typedef.h"


MG_NAMESPACE_BEGIN


template<typename T, class U>
class Repository
{
private:
	Repository(void) {}

public:
	static void Init(void)
	{
		U::Init();
	}

	static U* Instance(T aKey)
	{
		if (myInstance.find(aKey) == myInstance.end())
		{
			U* vSingle = new U(aKey);
			std::pair<T,U*> vPair(aKey, vSingle);
			Repository::myInstance.insert(vPair);
		}

		return Repository::myInstance[aKey];
	}

	static void Release(void)
	{
		/*std::map<T,U*>::iterator itMap = myInstance.begin();
		while (itMap != myInstance.end())
		{
			delete itMap->second;
			itMap->second = NULL;
			itMap++;
		}*/return;
	}

private:
	static std::map<T,U*> myInstance;
};


MG_NAMESPACE_END
