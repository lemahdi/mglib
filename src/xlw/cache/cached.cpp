#include "xlw/cache/cached.h"
#include "mgnova/glob/exception.h"
#include "xlw/cache/xlcache.h"

#include <sstream>
#include <time.h>


using namespace std;
using namespace MG;


/* Cache initialization */
CacheMap MG_Cache::ourGlobalCache = CacheMap();
UnCacheMap MG_Cache::ourGlobalUnCache = UnCacheMap();

/* Singleton initializations */
template<>
MG_CachePtr MG_SCache::myInstance = MG_CachePtr(NULL);

/* Constructors */
MG_Cache::MG_Cache() : myCounter(0)
{}

MG_Cache::~MG_Cache()
{}

bool MG_Cache::PersistentInsert(MG_XLObjectPtr& aXLObj, string& aRefObj, string& aError)
{
	if (aXLObj->myXLName == "")
	{
		aError = "Object not able to be seen from Excel.";
		return false;
	}

	string vSheetName;
	MG_XL_Cached::GetSheetNm(vSheetName);
	Coord vTopC, vBottomC;
	MG_XL_Cached::GetCaller(vTopC, vBottomC);

	try {
		bool vFound(false);
		pair< CacheMap::iterator,CacheMap::iterator > itMMap;
		CacheMap::iterator itMap;
		itMMap = ourGlobalCache.equal_range(vSheetName);
		for(itMap=itMMap.first; itMap!=itMMap.second; ++itMap)
		{
			if (itMap->second.first == vTopC)
			{
				vFound = true;
				break;
			}
		}

		if (vFound)
		{
			aXLObj->myXLId = itMap->second.second->myXLId;
			itMap->second.second->myXLId = NON_PERSISTENT_XL_OBJECT;
			itMap->second.second = aXLObj;
			ourGlobalUnCache[aXLObj->myXLId] = aXLObj;
		}
		else
		{
			ourGlobalUnCache.insert(make_pair(++myCounter, aXLObj));
			aXLObj->myXLId = int(myCounter);
			ourGlobalCache.insert(make_pair(vSheetName, CoordXLObject(vTopC,  aXLObj)));
		}

		aError = "";
		
		time_t time_t_T;
		struct tm tm_T;
		time(&time_t_T);
#pragma warning (push)
#pragma warning (disable : 4996)
		tm_T = *localtime(&time_t_T);
#pragma warning (pop)

		ostringstream vOSS;
		vOSS << aXLObj->myXLName << "_" << aXLObj->myXLId << "_";
		if (tm_T.tm_hour < 10)
			vOSS << "0" << tm_T.tm_hour;
		else
			vOSS << tm_T.tm_hour;
		vOSS << ":";
		if (tm_T.tm_min < 10)
			vOSS << "0" << tm_T.tm_min;
		else
			vOSS << tm_T.tm_min;
		vOSS << ":";
		if (tm_T.tm_sec < 10)
			vOSS << "0" << tm_T.tm_sec;
		else
			vOSS << tm_T.tm_sec;
		aRefObj = vOSS.str();

		return true;
	} catch (MG_Exception& vEx) {
		aError = vEx.GetMessage();
		aRefObj = "";
		return false;
	} catch (exception& vEx) {
		aError = vEx.what();
		aRefObj = "";
		return false;
	}
}

bool MG_Cache::PersistentGet(const string& aRefObj, MG_XLObjectPtr& aXLObj, string& aError)
{
	if (aRefObj == "")
	{
		aError = "Reference object is empty.";
		return false;
	}

	size_t vPos = aRefObj.find_first_of("_");
	string vRefObj = aRefObj.substr(vPos+1, string::npos);
	vPos = vRefObj.find_first_of("_");
	vRefObj = vRefObj.substr(0, vPos);
	size_t vIndex = atoi(vRefObj.c_str());
	UnCacheMap::iterator itMap = ourGlobalUnCache.find(vIndex);
	if (itMap == ourGlobalUnCache.end())
	{
		aError = aRefObj + " not found.";
		return false;
	}
	aXLObj = itMap->second;
	aError = "";

	return true;
}

bool MG_Cache::IsMGObjectDescriptor(const string &aDescriptor)
{
	if (aDescriptor.size() == 14)
		return false;

	if (aDescriptor.at(5) != '_')
		return false;

	size_t v1stUnderScore = aDescriptor.find_first_of("_");
	size_t vOffset = aDescriptor.substr(v1stUnderScore+1).find_first_of("_")+1;

	if (aDescriptor.at(5+vOffset) != '_')
		return false;

	if (aDescriptor.at(8+vOffset) != ':')
		return false;

	if (aDescriptor.at(11+vOffset) != ':')
		return false;

	string vErr;
	MG_XLObjectPtr vObj(NULL);
	PersistentGet(aDescriptor, vObj, vErr);
	if (vErr!="" || !vObj)
		return false;

	return true;
}
