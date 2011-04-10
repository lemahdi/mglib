#pragma once


#include <iostream>
#include <map>
#include <string>

#include "typedef.h"


class MG_ArgConv
{
public:
	typedef std::map<std::string,int>	ArgConvMap;
	typedef std::pair<std::string,int>	ArgConvPair;

public:
	MG_ArgConv(void) {}
	MG_ArgConv(const std::string& aDesc, const ArgConvPair vPairs[], const size_t& aSize);
	virtual ~MG_ArgConv(void) {}

	int operator[] (const std::string& vKey) const;

private:
	std::string			myDesc;
	ArgConvMap			myMap;
};

class MG_ArgConvReverse
{
public:
	typedef std::map<int,std::string>	ArgConvReverseMap;
	typedef std::pair<std::string,int>	ArgConvPair;
	typedef std::pair<int,std::string>	ArgConvReversePair;

public:
	MG_ArgConvReverse(void) {}
	MG_ArgConvReverse(const std::string& aDesc, const ArgConvPair vPairs[], const size_t& aSize);
	virtual ~MG_ArgConvReverse(void) {}

	std::string operator[] (const int& vKey) const;

private:
	std::string			myDesc;
	ArgConvReverseMap	myMap;
};
