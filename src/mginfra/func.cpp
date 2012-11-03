#include "mginfra/func.h"
#include "mgnova/glob/date.h"
#include "mggenpricer/genmod/irpricingmodel.h"

#include <map>
#include <string>
#include <math.h>
#include <assert.h>


using namespace std;
using namespace MG;


MG_Func::MG_Func() {}

MG_Func::~MG_Func() {}

vector<double> MG_Func::Eval(const vector<MG_Arg> &aArgs, const vector<double> &)
{
	return vector<double>(1, Eval(aArgs));
}


MG_MaxFunc::MG_MaxFunc() : MG_Func() {}

double MG_MaxFunc::Eval(const vector<MG_Arg>& aArgs)
{
	assert(aArgs.size() == 2);
	assert(aArgs[0].Type() == MG_Arg::ARG_DOUBLE);
	assert(aArgs[1].Type() == MG_Arg::ARG_DOUBLE);

	return aArgs[0].Double()>aArgs[1].Double() ? aArgs[0].Double() : aArgs[1].Double();
}


MG_MinFunc::MG_MinFunc() : MG_Func() {}

double MG_MinFunc::Eval(const vector<MG_Arg>& aArgs)
{
	assert(aArgs.size() == 2);
	assert(aArgs[0].Type() == MG_Arg::ARG_DOUBLE);
	assert(aArgs[1].Type() == MG_Arg::ARG_DOUBLE);

	return aArgs[0].Double()<aArgs[1].Double() ? aArgs[0].Double() : aArgs[1].Double();
}


MG_AbsFunc::MG_AbsFunc() : MG_Func() {}

double MG_AbsFunc::Eval(const vector<MG_Arg>& aArgs)
{
	assert(aArgs.size() == 1);
	assert(aArgs[0].Type() == MG_Arg::ARG_DOUBLE);

	return fabs(aArgs[0].Double());
}


MG_ExpFunc::MG_ExpFunc() : MG_Func() {}

double MG_ExpFunc::Eval(const vector<MG_Arg>& aArgs)
{
	assert(aArgs.size() == 1);
	assert(aArgs[0].Type() == MG_Arg::ARG_DOUBLE);

	return exp(aArgs[0].Double());
}


MG_LogFunc::MG_LogFunc() : MG_Func() {}

double MG_LogFunc::Eval(const vector<MG_Arg>& aArgs)
{
	assert(aArgs.size() == 1);
	assert(aArgs[0].Type() == MG_Arg::ARG_DOUBLE);

	return log(aArgs[0].Double());
}

MG_PowFunc::MG_PowFunc() : MG_Func() {}

double MG_PowFunc::Eval(const vector<MG_Arg>& aArgs)
{
	assert(aArgs.size() == 2);
	assert(aArgs[0].Type() == MG_Arg::ARG_DOUBLE);
	assert(aArgs[1].Type() == MG_Arg::ARG_DOUBLE);

	return pow(aArgs[0].Double(), aArgs[1].Double());
}

MG_IfFunc::MG_IfFunc() : MG_Func() {}

double MG_IfFunc::Eval(const vector<MG_Arg>& aArgs)
{
	assert(aArgs.size() == 3);
	assert(aArgs[0].Type() == MG_Arg::ARG_BOOL);
	assert(aArgs[1].Type() == MG_Arg::ARG_DOUBLE);
	assert(aArgs[2].Type() == MG_Arg::ARG_DOUBLE);

	return aArgs[0].Bool() ? aArgs[1].Double() : aArgs[2].Double();
}

MG_LiborFunc::MG_LiborFunc() : MG_Func() {}

double MG_LiborFunc::Eval(const vector<MG_Arg>& aArgs)
{
	return Eval(aArgs, vector<double>(1,0.))[0];
}

vector<double> MG_LiborFunc::Eval(const vector<MG_Arg>& aArgs, const vector<double>& aStates)
{
	assert(aArgs.size() == 7);
	assert(aArgs[0].Type() == MG_Arg::ARG_DATE);
	assert(aArgs[1].Type() == MG_Arg::ARG_DATE);
	assert(aArgs[2].Type() == MG_Arg::ARG_DATE);
	assert(aArgs[3].Type() == MG_Arg::ARG_DATE);
	assert(aArgs[4].Type() == MG_Arg::ARG_DOUBLE);
	assert(aArgs[5].Type() == MG_Arg::ARG_DOUBLE);
	assert(aArgs[6].Type() == MG_Arg::ARG_DOUBLE);

	return myModel->Libor(aArgs[0].Date(), aArgs[1].Date()
						, aArgs[2].Date(), aArgs[3].Date()
						, aArgs[4].Double(), aArgs[5].Double(), aArgs[6].Double(), aStates);
}

void MG_LiborFunc::SetModel(const MG_PricingModelPtr& aMdl)
{
	myModel = aMdl;
}

template <>
MG_FuncBuilderPtr MG_SFuncBuilder::myInstance = MG_FuncBuilderPtr(NULL);

void MG_FuncBuilder::Init()
{
	// Basic functions
	myFuncs.insert(NameFuncPair("MAX",MG_FuncPtr(new MG_MaxFunc())));
	myFuncs.insert(NameFuncPair("MIN",MG_FuncPtr(new MG_MinFunc())));
	myFuncs.insert(NameFuncPair("ABS",MG_FuncPtr(new MG_AbsFunc())));
	myFuncs.insert(NameFuncPair("EXP",MG_FuncPtr(new MG_ExpFunc())));
	myFuncs.insert(NameFuncPair("LOG",MG_FuncPtr(new MG_LogFunc())));
	myFuncs.insert(NameFuncPair("POW",MG_FuncPtr(new MG_PowFunc())));
	myFuncs.insert(NameFuncPair("IF",MG_FuncPtr(new MG_IfFunc())));
	
	// Numeric functions
	myFuncs.insert(NameFuncPair("LIBOR",MG_FuncPtr(new MG_LiborFunc())));
}

MG_FuncPtr MG_FuncBuilder::GetFunc(const std::string &aFuncName)
{
	NameFuncMap::const_iterator vIt = myFuncs.find(aFuncName);
	if (vIt != myFuncs.end())
		return vIt->second;
	return MG_FuncPtr(NULL);
}
