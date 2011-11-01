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

#include "mgnova/typedef.h"


MG_NAMESPACE_BEGIN


template<typename T, class U>
class Repository
{
private:
	struct CleanMap
	{
		std::map<T,U*> myMap;
		std::map<T,U*>& Map(void) { return myMap; }
		~CleanMap(void)
		{
			std::map<T,U*>::iterator itMap = myMap.begin();
			while (itMap != myMap.end())
			{
				delete itMap->second;
				itMap->second = NULL;
				++itMap;
			}
		}
	};

	Repository(void) {}

public:
	static void Init(void)
	{
		U::Init();
	}

	static U* Instance(T aKey)
	{
		if (myInstance.Map().find(aKey) == myInstance.Map().end())
		{
			U* vSingle = new U(aKey);
			std::pair<T,U*> vPair(aKey, vSingle);
			Repository::myInstance.Map().insert(vPair);
		}

		return Repository::myInstance.Map()[aKey];
	}

private:
	static CleanMap myInstance;

};
template<typename T, class U>
typename Repository<T,U>::CleanMap Repository<T,U>::myInstance = Repository<T,U>::CleanMap();

MG_NAMESPACE_END
