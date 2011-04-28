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
	FAKE_ASSIGN_OPERATOR(MG_Cache)
public:
	MG_Cache(void);
	virtual ~MG_Cache(void);

public:
	MG_ObjectPtr GetFromCache(const size_t& aIndex) const;
	size_t SetToCache(const MG_ObjectPtr& aObjPtr);
	void SetToCache(const MG_ObjectPtr& aObjPtr, const size_t& aIndex);

private:
	std::vector<MG_ObjectPtr> myCache;
	size_t myCounter;

public:
	static CacheMap ourGlobalCache;
};
typedef Singleton<MG_Cache> MG_SCache;


MG_NAMESPACE_END
