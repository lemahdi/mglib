/*
 * Copyright			: 2010 by MG
 * Version				: 0.1.22
 * Date					: 24 APR 2011
 * Purpose				: MG_Cached is a class for managing the persistence
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include <vector>
#include <map>

#include "nova/glob/object.h"
#include "nova/glob/typedef.h"
#include "nova/patterns/singleton.hpp"


MG_NAMESPACE_BEGIN


/* Cache */
class MG_Cache
{
	template<class T> friend class Singleton;

private:
	FAKE_ASSIGN_OPERATOR(MG_Cache)

	MG_Cache(void);

public:
	virtual ~MG_Cache(void);

public:
	bool PersistentInsert		(MG_XLObjectPtr& aXLObj, std::string& aRefObj, std::string& aError);
	bool PersistentGet			(const std::string& aRefObj, MG_XLObjectPtr& aXLObj, std::string& aError);
	bool IsMGObjectDescriptor	(const std::string& aDescriptor);

private:
	size_t myCounter;

public:
	static CacheMap		ourGlobalCache;
	static UnCacheMap	ourGlobalUnCache;
};
typedef Singleton<MG_Cache> MG_SCache;


MG_NAMESPACE_END
