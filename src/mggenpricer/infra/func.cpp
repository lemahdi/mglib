#include "mggenpricer/infra/func.h"
#include "mggenpricer/infra/arg.h"
#include "mggenpricer/genmod/irpricingmodel.h"

#include <math.h>


using namespace std;
using namespace MG;


MG_Func::MG_Func() {}

MG_Func::~MG_Func() {}

MG_Arg MG_Func::Eval(const vector<MG_Arg> &aArgs, const vector<double> &)
{
	return Eval(aArgs);
}


MG_MaxFunc::MG_MaxFunc() : MG_Func() {}

MG_Arg MG_MaxFunc::Eval(const vector<MG_Arg>& aArgs)
{
	assert(aArgs.size() == 2);
	assert(aArgs[0].Type()==MG_Arg::ARG_DOUBLE || aArgs[0].Type()==MG_Arg::ARG_V_DOUBLE);
	assert(aArgs[1].Type()==MG_Arg::ARG_DOUBLE || aArgs[1].Type()==MG_Arg::ARG_V_DOUBLE);

	const MG_Arg& vArg0 = aArgs[0];
	const MG_Arg& vArg1 = aArgs[1];

	if (vArg0.Type()==MG_Arg::ARG_DOUBLE && vArg1.Type()==MG_Arg::ARG_DOUBLE)
		return vArg0.Double()>vArg1.Double() ? vArg0 : vArg1;

	if (vArg0.Type()==MG_Arg::ARG_V_DOUBLE && vArg1.Type()==MG_Arg::ARG_DOUBLE)
	{
		const vector<double>& vArg0V = *vArg0.VDouble();
		size_t vNb = vArg0V.size();
		MG_StdVectDblPtr vValsPtr(new vector<double>(vNb));
		vector<double>& vVals = *vValsPtr;
		for(size_t i=0; i<vNb; ++i)
			vVals[i] = vArg0V[i]>vArg1.Double() ? vArg0V[i] : vArg1.Double();
		return MG_Arg(vValsPtr);
	}

	if (vArg0.Type()==MG_Arg::ARG_DOUBLE && vArg1.Type()==MG_Arg::ARG_V_DOUBLE)
	{
		const vector<double>& vArg1V = *vArg1.VDouble();
		size_t vNb = vArg1V.size();
		MG_StdVectDblPtr vValsPtr(new vector<double>(vNb));
		vector<double>& vVals = *vValsPtr;
		for(size_t i=0; i<vNb; ++i)
			vVals[i] = vArg0.Double()>vArg1V[i] ? vArg0.Double() : vArg1V[i];
		return MG_Arg(vValsPtr);
	}

	// if (vArg0.Type()==MG_Arg::ARG_V_DOUBLE && vArg1.Type()==MG_Arg::ARG_V_DOUBLE)
	const vector<double>& vArg0V = *vArg0.VDouble();
	const vector<double>& vArg1V = *vArg1.VDouble();
	size_t vNb = vArg0V.size();
	MG_StdVectDblPtr vValsPtr(new vector<double>(vNb));
	vector<double>& vVals = *vValsPtr;
	for(size_t i=0; i<vNb; ++i)
		vVals[i] = vArg0V[i]>vArg1V[i] ? vArg0V[i] : vArg1V[i];
	return MG_Arg(vValsPtr);
}


MG_MinFunc::MG_MinFunc() : MG_Func() {}

MG_Arg MG_MinFunc::Eval(const vector<MG_Arg>& aArgs)
{
	assert(aArgs.size() == 2);
	assert(aArgs[0].Type()==MG_Arg::ARG_DOUBLE || aArgs[0].Type()==MG_Arg::ARG_V_DOUBLE);
	assert(aArgs[1].Type()==MG_Arg::ARG_DOUBLE || aArgs[1].Type()==MG_Arg::ARG_V_DOUBLE);

	const MG_Arg& vArg0 = aArgs[0];
	const MG_Arg& vArg1 = aArgs[1];

	if (vArg0.Type()==MG_Arg::ARG_DOUBLE && vArg1.Type()==MG_Arg::ARG_DOUBLE)
		return vArg0.Double()<vArg1.Double() ? vArg0 : vArg1;

	if (vArg0.Type()==MG_Arg::ARG_V_DOUBLE && vArg1.Type()==MG_Arg::ARG_DOUBLE)
	{
		const vector<double>& vArg0V = *vArg0.VDouble();
		size_t vNb = vArg0V.size();
		MG_StdVectDblPtr vValsPtr(new vector<double>(vNb));
		vector<double>& vVals = *vValsPtr;
		for(size_t i=0; i<vNb; ++i)
			vVals[i] = vArg0V[i]<vArg1.Double() ? vArg0V[i] : vArg1.Double();
		return MG_Arg(vValsPtr);
	}

	if (vArg0.Type()==MG_Arg::ARG_DOUBLE && vArg1.Type()==MG_Arg::ARG_V_DOUBLE)
	{
		const vector<double>& vArg1V = *vArg1.VDouble();
		size_t vNb = vArg1V.size();
		MG_StdVectDblPtr vValsPtr(new vector<double>(vNb));
		vector<double>& vVals = *vValsPtr;
		for(size_t i=0; i<vNb; ++i)
			vVals[i] = vArg0.Double()<vArg1V[i] ? vArg0.Double() : vArg1V[i];
		return MG_Arg(vValsPtr);
	}

	// if (vArg0.Type()==MG_Arg::ARG_V_DOUBLE && vArg1.Type()==MG_Arg::ARG_V_DOUBLE)
	const vector<double>& vArg0V = *vArg0.VDouble();
	const vector<double>& vArg1V = *vArg1.VDouble();
	size_t vNb = vArg0V.size();
	MG_StdVectDblPtr vValsPtr(new vector<double>(vNb));
	vector<double>& vVals = *vValsPtr;
	for(size_t i=0; i<vNb; ++i)
		vVals[i] = vArg0V[i]<vArg1V[i] ? vArg0V[i] : vArg1V[i];
	return MG_Arg(vValsPtr);
}


MG_AbsFunc::MG_AbsFunc() : MG_Func() {}

MG_Arg MG_AbsFunc::Eval(const vector<MG_Arg>& aArgs)
{
	assert(aArgs.size() == 1);
	assert(aArgs[0].Type()==MG_Arg::ARG_DOUBLE || aArgs[0].Type()==MG_Arg::ARG_V_DOUBLE);

	const MG_Arg& vArg0 = aArgs[0];

	if (vArg0.Type() == MG_Arg::ARG_DOUBLE)
		return MG_Arg(fabs(vArg0.Double()));

	//if (vArg0.Type() == MG_Arg::ARG_V_DOUBLE)
	const vector<double>& vArg0V = *vArg0.VDouble();
	size_t vNb = vArg0V.size();
	MG_StdVectDblPtr vValsPtr(new vector<double>(vNb));
	vector<double>& vVals = *vValsPtr;
	for(size_t i=0; i<vNb; ++i)
		vVals[i] = fabs(vArg0V[i]);
	return MG_Arg(vValsPtr);
}


MG_ExpFunc::MG_ExpFunc() : MG_Func() {}

MG_Arg MG_ExpFunc::Eval(const vector<MG_Arg>& aArgs)
{
	assert(aArgs.size() == 1);
	assert(aArgs[0].Type()==MG_Arg::ARG_DOUBLE || aArgs[0].Type()==MG_Arg::ARG_V_DOUBLE);

	const MG_Arg& vArg0 = aArgs[0];

	if (vArg0.Type() == MG_Arg::ARG_DOUBLE)
		return MG_Arg(exp(vArg0.Double()));

	//if (vArg0.Type() == MG_Arg::ARG_V_DOUBLE)
	const vector<double>& vArg0V = *vArg0.VDouble();
	size_t vNb = vArg0V.size();
	MG_StdVectDblPtr vValsPtr(new vector<double>(vNb));
	vector<double>& vVals = *vValsPtr;
	for(size_t i=0; i<vNb; ++i)
		vVals[i] = exp(vArg0V[i]);
	return MG_Arg(vValsPtr);
}


MG_LogFunc::MG_LogFunc() : MG_Func() {}

MG_Arg MG_LogFunc::Eval(const vector<MG_Arg>& aArgs)
{
	assert(aArgs.size() == 1);
	assert(aArgs[0].Type()==MG_Arg::ARG_DOUBLE || aArgs[0].Type()==MG_Arg::ARG_V_DOUBLE);

	const MG_Arg& vArg0 = aArgs[0];

	if (vArg0.Type() == MG_Arg::ARG_DOUBLE)
		return MG_Arg(log(vArg0.Double()));

	//if (vArg0.Type() == MG_Arg::ARG_V_DOUBLE)
	const vector<double>& vArg0V = *vArg0.VDouble();
	size_t vNb = vArg0V.size();
	MG_StdVectDblPtr vValsPtr(new vector<double>(vNb));
	vector<double>& vVals = *vValsPtr;
	for(size_t i=0; i<vNb; ++i)
		vVals[i] = log(vArg0V[i]);
	return MG_Arg(vValsPtr);
}

MG_PowFunc::MG_PowFunc() : MG_Func() {}

MG_Arg MG_PowFunc::Eval(const vector<MG_Arg>& aArgs)
{
	assert(aArgs.size() == 2);
	assert(aArgs[0].Type()==MG_Arg::ARG_DOUBLE || aArgs[0].Type()==MG_Arg::ARG_V_DOUBLE);
	assert(aArgs[1].Type() == MG_Arg::ARG_DOUBLE);

	const MG_Arg& vArg0 = aArgs[0];
	const MG_Arg& vArg1 = aArgs[1];

	if (vArg0.Type() == MG_Arg::ARG_DOUBLE)
		return MG_Arg(pow(vArg0.Double(), vArg1.Double()));

	//if (vArg0.Type() == MG_Arg::ARG_V_DOUBLE)
	const vector<double>& vArg0V = *vArg0.VDouble();
	size_t vNb = vArg0V.size();
	MG_StdVectDblPtr vValsPtr(new vector<double>(vNb));
	vector<double>& vVals = *vValsPtr;
	for(size_t i=0; i<vNb; ++i)
		vVals[i] = pow(vArg0V[i], vArg1.Double());
	return MG_Arg(vValsPtr);
}

MG_IfFunc::MG_IfFunc() : MG_Func() {}

MG_Arg MG_IfFunc::Eval(const vector<MG_Arg>& aArgs)
{
	assert(aArgs.size() == 3);
	assert(aArgs[0].Type()==MG_Arg::ARG_BOOL || aArgs[0].Type()==MG_Arg::ARG_V_BOOL);
	assert(aArgs[1].Type() == MG_Arg::ARG_DOUBLE);
	assert(aArgs[2].Type() == MG_Arg::ARG_DOUBLE);

	const MG_Arg& vArg0 = aArgs[0];
	const MG_Arg& vArg1 = aArgs[1];
	const MG_Arg& vArg2 = aArgs[2];

	if (vArg0.Type() == MG_Arg::ARG_BOOL)
		return vArg0.Bool() ? vArg1 : vArg2;

	//if (vArg0.Type() == MG_Arg::ARG_V_BOOL)
	const vector<bool>& vArg0V = *vArg0.VBool();
	size_t vNb = vArg0V.size();
	MG_StdVectDblPtr vValsPtr(new vector<double>(vNb));
	vector<double>& vVals = *vValsPtr;
	for(size_t i=0; i<vNb; ++i)
		vVals[i] = vArg0V[i] ? vArg1.Double() : vArg2.Double();
	return MG_Arg(vValsPtr);
}

MG_LiborFunc::MG_LiborFunc() : MG_Func() {}

MG_Arg MG_LiborFunc::Eval(const vector<MG_Arg>& aArgs)
{
	assert(aArgs.size() == 7);
	assert(aArgs[0].Type() == MG_Arg::ARG_DATE);
	assert(aArgs[1].Type() == MG_Arg::ARG_DATE);
	assert(aArgs[2].Type() == MG_Arg::ARG_DATE);
	assert(aArgs[3].Type() == MG_Arg::ARG_DATE);
	assert(aArgs[4].Type() == MG_Arg::ARG_DOUBLE);
	assert(aArgs[5].Type() == MG_Arg::ARG_DOUBLE);
	assert(aArgs[6].Type() == MG_Arg::ARG_DOUBLE);

	return myModel->Libor(aArgs[1].Date(), aArgs[2].Date(), aArgs[3].Date(), aArgs[4].Double());
}

MG_Arg MG_LiborFunc::Eval(const vector<MG_Arg>& aArgs, const vector<double>& aStates)
{
	assert(aArgs.size() == 7);
	assert(aArgs[0].Type() == MG_Arg::ARG_DATE);
	assert(aArgs[1].Type() == MG_Arg::ARG_DATE);
	assert(aArgs[2].Type() == MG_Arg::ARG_DATE);
	assert(aArgs[3].Type() == MG_Arg::ARG_DATE);
	assert(aArgs[4].Type() == MG_Arg::ARG_DOUBLE);
	assert(aArgs[5].Type() == MG_Arg::ARG_DOUBLE);
	assert(aArgs[6].Type() == MG_Arg::ARG_DOUBLE);

	MG_Arg vArg(myModel->Libor(aArgs[0].Date(), aArgs[1].Date()
							, aArgs[2].Date(), aArgs[3].Date()
							, aArgs[4].Double(), aArgs[5].Double(), aArgs[6].Double(), aStates));
	return vArg;
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
