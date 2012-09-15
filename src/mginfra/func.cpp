#include "mginfra/func.h"

#include <map>
#include <string>
#include <math.h>
#include <assert.h>


using namespace std;
using namespace MG;


MG_Func::MG_Func() {}

MG_Func::~MG_Func() {}


MG_MaxFunc::MG_MaxFunc() : MG_Func() {}

double MG_MaxFunc::Eval(const vector<double>& aArgs)
{
	assert(aArgs.size() == 2);
	return aArgs[0]>aArgs[1] ? aArgs[0] : aArgs[1];
}


MG_MinFunc::MG_MinFunc() : MG_Func() {}

double MG_MinFunc::Eval(const vector<double>& aArgs)
{
	assert(aArgs.size() == 2);
	return aArgs[0]<aArgs[1] ? aArgs[0] : aArgs[1];
}


MG_AbsFunc::MG_AbsFunc() : MG_Func() {}

double MG_AbsFunc::Eval(const vector<double>& aArgs)
{
	assert(aArgs.size() == 1);
	return fabs(aArgs[0]);
}


MG_ExpFunc::MG_ExpFunc() : MG_Func() {}

double MG_ExpFunc::Eval(const vector<double>& aArgs)
{
	assert(aArgs.size() == 1);
	return exp(aArgs[0]);
}


MG_LogFunc::MG_LogFunc() : MG_Func() {}

double MG_LogFunc::Eval(const vector<double>& aArgs)
{
	assert(aArgs.size() == 1);
	return log(aArgs[0]);
}

MG_PowFunc::MG_PowFunc() : MG_Func() {}

double MG_PowFunc::Eval(const vector<double>& aArgs)
{
	assert(aArgs.size() == 2);
	return pow(aArgs[0],aArgs[1]);
}

MG_IfFunc::MG_IfFunc() : MG_Func() {}

double MG_IfFunc::Eval(const vector<double>& aArgs)
{
	assert(aArgs.size() == 3);
	return aArgs[0] ? aArgs[1] : aArgs[2];
}

template <>
MG_FuncBuilderPtr MG_SFuncBuilder::myInstance = MG_FuncBuilderPtr(NULL);

void MG_FuncBuilder::Init()
{
	myFuncs.insert(NameFuncPair("MAX",MG_FuncPtr(new MG_MaxFunc())));
	myFuncs.insert(NameFuncPair("MIN",MG_FuncPtr(new MG_MinFunc())));
	myFuncs.insert(NameFuncPair("ABS",MG_FuncPtr(new MG_AbsFunc())));
	myFuncs.insert(NameFuncPair("EXP",MG_FuncPtr(new MG_ExpFunc())));
	myFuncs.insert(NameFuncPair("LOG",MG_FuncPtr(new MG_LogFunc())));
	myFuncs.insert(NameFuncPair("POW",MG_FuncPtr(new MG_PowFunc())));
	myFuncs.insert(NameFuncPair("IF",MG_FuncPtr(new MG_IfFunc())));
}

MG_FuncPtr MG_FuncBuilder::GetFunc(const std::string &aFuncName)
{
	return myFuncs[aFuncName];
}
