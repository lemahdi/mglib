#include "mgnova/argconv.h"


using namespace std;
using namespace MG;


MG_ArgConv::MG_ArgConv(const std::string& aDesc, const ArgConvPair vPairs[], const size_t& aSize)
	: myDesc(aDesc)
{
	for(size_t i=0; i<aSize; i++)
		myMap.insert(vPairs[i]);
}

int MG_ArgConv::operator[] (const string& vKey) const
{
	ArgConvMap::const_iterator itMap = myMap.find(vKey);
	if (itMap == myMap.end())
		//throw
		return -1;
	return itMap->second;
}


MG_ArgConvReverse::MG_ArgConvReverse(const std::string& aDesc, const ArgConvPair vPairs[], const size_t& aSize)
	: myDesc(aDesc)
{
	ArgConvReversePair vPair;
	for(size_t i=0; i<aSize; i++)
	{
		vPair = ArgConvReversePair(vPairs[i].second,vPairs[i].first);
		myMap.insert(vPair);
	}
}

string MG_ArgConvReverse::operator[] (const int& vKey) const
{
	ArgConvReverseMap::const_iterator itMap = myMap.find(vKey);
	if (itMap == myMap.end())
		//throw
		return "";
	return itMap->second;
}

