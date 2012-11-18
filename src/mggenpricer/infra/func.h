/*
 * Copyright			: 2010 by MG
 * Version				: 0.1.14
 * Date					: 21 DEC 2010
 * Purpose				: MG_Func is a base class for creating defined functions
 *						  MG_FuncBuilder is a singleton class for loading defined functions
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "mgnova/patterns/singleton.hpp"
#include "mgnova/patterns/countedptr.hpp"
#include "mgnova/glob/date.h"

#include <vector>


MG_NAMESPACE_BEGIN


class MG_Arg;
class MG_PricingModel;
class MG_IRPricingModel;


class MG_Func
{
public:
	MG_Func(void);
	virtual ~MG_Func(void);

	virtual MG_Arg Eval(const std::vector<MG_Arg>& aArgs) = 0;
	virtual MG_Arg Eval(const std::vector<MG_Arg>& aArgs, const std::vector<double>& aStates);

	virtual void SetModel(const MG_PricingModelPtr& ) {}
};

class MG_MaxFunc : public MG_Func
{
public:
	MG_MaxFunc(void);

	MG_Arg Eval(const std::vector<MG_Arg>& aArgs);
};

class MG_MinFunc : public MG_Func
{
public:
	MG_MinFunc(void);

	MG_Arg Eval(const std::vector<MG_Arg>& aArgs);
};

class MG_AbsFunc : public MG_Func
{
public:
	MG_AbsFunc(void);

	MG_Arg Eval(const std::vector<MG_Arg>& aArgs);
};

class MG_ExpFunc : public MG_Func
{
public:
	MG_ExpFunc(void);

	MG_Arg Eval(const std::vector<MG_Arg>& aArgs);
};

class MG_LogFunc : public MG_Func
{
public:
	MG_LogFunc(void);

	MG_Arg Eval(const std::vector<MG_Arg>& aArgs);
};

class MG_PowFunc : public MG_Func
{
public:
	MG_PowFunc(void);

	MG_Arg Eval(const std::vector<MG_Arg>& aArgs);
};

class MG_IfFunc : public MG_Func
{
public:
	MG_IfFunc(void);

	MG_Arg Eval(const std::vector<MG_Arg>& aArgs);
};

class MG_LiborFunc : public MG_Func
{
public:
	MG_LiborFunc(void);

	MG_Arg Eval(const std::vector<MG_Arg>& aArgs);
	MG_Arg Eval(const std::vector<MG_Arg>& aArgs, const std::vector<double>& aStates);

	void SetModel(const MG_PricingModelPtr& aMdl);

private:
	MG_IRPricingModelPtr myModel;
};

class MG_FuncBuilder
{
	template<class T> friend class Singleton;

private:
	MG_FuncBuilder(void) {}

public:
	virtual ~MG_FuncBuilder(void) {}

public:
	void Init(void);

	MG_FuncPtr GetFunc(const std::string& aFuncName);

private:
	NameFuncMap myFuncs;
};
typedef Singleton<MG_FuncBuilder> MG_SFuncBuilder;


MG_NAMESPACE_END
