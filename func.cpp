#include "func.h"

#include <map>
#include <string>
#include <math.h>


using namespace std;


double MaxFunc::Eval(const vector<double>& aArgs)
{
	return aArgs[0]>aArgs[1] ? aArgs[0] : aArgs[1];
}


double MinFunc::Eval(const vector<double>& aArgs)
{
	return aArgs[0]<aArgs[1] ? aArgs[0] : aArgs[1];
}


double AbsFunc::Eval(const vector<double>& aArgs)
{
	return fabs(aArgs[0]);
}


double ExpFunc::Eval(const vector<double>& aArgs)
{
	return exp(aArgs[0]);
}


double LogFunc::Eval(const vector<double>& aArgs)
{
	return log(aArgs[0]);
}


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
	myFuncs.insert(NameFuncPair("MAX",FuncPtr(new MaxFunc())));
	myFuncs.insert(NameFuncPair("MIN",FuncPtr(new MinFunc())));
	myFuncs.insert(NameFuncPair("ABS",FuncPtr(new AbsFunc())));
	myFuncs.insert(NameFuncPair("EXP",FuncPtr(new ExpFunc())));
	myFuncs.insert(NameFuncPair("LOG",FuncPtr(new LogFunc())));
	myFuncs.insert(NameFuncPair("POW",FuncPtr(new PowFunc())));
}

