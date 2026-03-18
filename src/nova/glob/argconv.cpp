#include "nova/glob/argconv.h"
#include "nova/glob/exception.h"

#include <sstream>


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
	{
		std::ostringstream vMsg;
		vMsg << "ArgConv[" << myDesc << "]: key '" << vKey << "' not found.";
		MG_THROW(vMsg.str());
	}
	return itMap->second;
}

bool MG_ArgConv::Exist(const string& vKey) const
{
	ArgConvMap::const_iterator itMap = myMap.find(vKey);
	if (itMap == myMap.end())
		return false;
	return true;
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
	{
		std::ostringstream vMsg;
		vMsg << "ArgConvReverse[" << myDesc << "]: key " << vKey << " not found.";
		MG_THROW(vMsg.str());
	}
	return itMap->second;
}

bool MG_ArgConvReverse::Exist(const int& vKey) const
{
	ArgConvReverseMap::const_iterator itMap = myMap.find(vKey);
	if (itMap == myMap.end())
		return false;
	return true;
}

