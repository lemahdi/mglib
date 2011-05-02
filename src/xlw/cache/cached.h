/*
 * Copyright			: 2010 by MG
 * File					: cached.h
 * Version				: 0.1.22
 * Last changed			: 24 APR 2011
 * Purpose				: MG_Cached is a class for managing the persistence
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include <vector>
#include <map>

#include "mgnova/object.h"
#include "mgnova/typedef.h"
#include "mgnova/patterns/singleton.hpp"


MG_NAMESPACE_BEGIN


/* Cache */
class MG_Cache
{
	template<class T> friend class Singleton;
	FAKE_ASSIGN_OPERATOR(MG_Cache)
private:
	MG_Cache(void);
	virtual ~MG_Cache(void);

public:
	bool PersistentInsert	(MG_XLObjectPtr& aXLObj, std::string& aRefObj, std::string& aError);
	bool PersistentGet		(const std::string& aRefObj, MG_XLObjectPtr& aXLObj, std::string& aError);

private:
	size_t myCounter;

public:
	static CacheMap		ourGlobalCache;
	static UnCacheMap	ourGlobalUnCache;
};
typedef Singleton<MG_Cache> MG_SCache;


MG_NAMESPACE_END
