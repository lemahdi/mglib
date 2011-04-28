#include "mgxll/cache/cached.h"
#include "mgnova/exception.h"


using namespace std;
using namespace MG;


/* Cache initialization */
CacheMap MG_Cache::ourGlobalCache = CacheMap();

/* Singleton initializations */
template<>
MG_Cache* MG_SCache::myInstance = NULL;

/* Constructors */
MG_Cache::MG_Cache() : myCounter(0)
{}

MG_Cache::~MG_Cache()
{}

/* Getter from the cache */
MG_ObjectPtr MG_Cache::GetFromCache(const size_t& aIndex) const
{
	if (aIndex >= myCounter)
		MG_THROW("Index is beyond of the cache counter.");

	return myCache[aIndex];
}

/* Setter to the cache */
size_t MG_Cache::SetToCache(const MG_ObjectPtr& aObjPtr)
{
	myCache.push_back(aObjPtr);
	return myCounter++;
}

void MG_Cache::SetToCache(const MG_ObjectPtr& aObjPtr, const size_t& aIndex)
{
	if (aIndex >= myCounter)
		MG_THROW("Index is beyond of the cache counter.");

	myCache[aIndex] = aObjPtr;
}

