#include "func.h"

#include <map>
#include <string>
#include <math.h>


using namespace std;


MG_Func::MG_Func() {}

MG_Func::~MG_Func() {}


MG_MaxFunc::MG_MaxFunc() : MG_Func() {}

double MG_MaxFunc::Eval(const vector<double>& aArgs)
{
	return aArgs[0]>aArgs[1] ? aArgs[0] : aArgs[1];
}


MG_MinFunc::MG_MinFunc() : MG_Func() {}

double MG_MinFunc::Eval(const vector<double>& aArgs)
{
	return aArgs[0]<aArgs[1] ? aArgs[0] : aArgs[1];
}


MG_AbsFunc::MG_AbsFunc() : MG_Func() {}

double MG_AbsFunc::Eval(const vector<double>& aArgs)
{
	return fabs(aArgs[0]);
}


MG_ExpFunc::MG_ExpFunc() : MG_Func() {}

double MG_ExpFunc::Eval(const vector<double>& aArgs)
{
	return exp(aArgs[0]);
}


MG_LogFunc::MG_LogFunc() : MG_Func() {}

double MG_LogFunc::Eval(const vector<double>& aArgs)
{
	return log(aArgs[0]);
}

MG_LogPowFunc::MG_LogPowFunc() : MG_Func() {}

double MG_LogPowFunc::Eval(const vector<double>& aArgs)
{
	return pow(aArgs[0],aArgs[1]);
}

#ifndef WIN32
template <>
#endif
MG_FuncBuilder* MG_SFuncBuilder::myInstance = NULL;

MG_FuncBuilder::MG_FuncBuilder()
{
	myFuncs.insert(NameFuncPair("MAX",new MG_MaxFunc()));
	myFuncs.insert(NameFuncPair("MIN",new MG_MinFunc()));
	myFuncs.insert(NameFuncPair("ABS",new MG_AbsFunc()));
	myFuncs.insert(NameFuncPair("EXP",new MG_ExpFunc()));
	myFuncs.insert(NameFuncPair("LOG",new MG_LogFunc()));
	myFuncs.insert(NameFuncPair("POW",new MG_LogPowFunc()));
}

MG_FuncBuilder::~MG_FuncBuilder()
{
	std::map<std::string,MG_Func*>::iterator it = myFuncs.begin();
	while (it != myFuncs.end())
	{
		delete it->second;
		it++;
	}
}

MG_Func* MG_FuncBuilder::GetFunc(const std::string &aFuncName)
{
	return myFuncs[aFuncName];
}
