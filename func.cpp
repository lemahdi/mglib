#include "func.h"

#include <map>
#include <string>
#include <math.h>


using namespace std;


Func::Func() {}

Func::~Func() {}


MaxFunc::MaxFunc() : Func() {}

double MaxFunc::Eval(const vector<double>& aArgs)
{
	return aArgs[0]>aArgs[1] ? aArgs[0] : aArgs[1];
}


MinFunc::MinFunc() : Func() {}

double MinFunc::Eval(const vector<double>& aArgs)
{
	return aArgs[0]<aArgs[1] ? aArgs[0] : aArgs[1];
}


AbsFunc::AbsFunc() : Func() {}

double AbsFunc::Eval(const vector<double>& aArgs)
{
	return fabs(aArgs[0]);
}


ExpFunc::ExpFunc() : Func() {}

double ExpFunc::Eval(const vector<double>& aArgs)
{
	return exp(aArgs[0]);
}


LogFunc::LogFunc() : Func() {}

double LogFunc::Eval(const vector<double>& aArgs)
{
	return log(aArgs[0]);
}

PowFunc::PowFunc() : Func() {}

double PowFunc::Eval(const vector<double>& aArgs)
{
	return pow(aArgs[0],aArgs[1]);
}

#ifndef WIN32
template <>
#endif
FuncBuilder* SFuncBuilder::myInstance = NULL;

FuncBuilder::FuncBuilder()
{
	myFuncs.insert(NameFuncPair("MAX",new MaxFunc()));
	myFuncs.insert(NameFuncPair("MIN",new MinFunc()));
	myFuncs.insert(NameFuncPair("ABS",new AbsFunc()));
	myFuncs.insert(NameFuncPair("EXP",new ExpFunc()));
	myFuncs.insert(NameFuncPair("LOG",new LogFunc()));
	myFuncs.insert(NameFuncPair("POW",new PowFunc()));
}

FuncBuilder::~FuncBuilder()
{
	std::map<std::string,Func*>::iterator it = myFuncs.begin();
	while (it != myFuncs.end())
	{
		delete it->second;
		it++;
	}
}

Func* FuncBuilder::GetFunc(const std::string &aFuncName)
{
	return myFuncs[aFuncName];
}
