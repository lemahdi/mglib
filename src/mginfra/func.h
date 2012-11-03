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


#include <iostream>
#include <map>
#include <vector>
#include <string>

#include "mgnova/patterns/singleton.hpp"
#include "mgnova/patterns/countedptr.hpp"
#include "mgnova/glob/date.h"


MG_NAMESPACE_BEGIN


class MG_PricingModel;
class MG_IRPricingModel;


class MG_Arg
{
public:
	enum TYPE { ARG_ERR, ARG_DOUBLE, ARG_STRING, ARG_DATE, ARG_V_DOUBLE, ARG_BOOL, ARG_V_BOOL };

	MG_Arg(void) : myType(ARG_ERR) {}
	MG_Arg(const double& aDbl) : myType(ARG_DOUBLE), myDouble(aDbl) {}
	MG_Arg(const std::string& aStr) : myType(ARG_STRING), myString(aStr) {}
	MG_Arg(const MG_Date& aDt) : myType(ARG_DATE), myDate(aDt), myDouble(aDt.GetJulianDay()) {}
	MG_Arg(const std::vector<double>& aVDbl) : myType(ARG_V_DOUBLE), myVDouble(aVDbl) {}
	MG_Arg(const bool& aBool) : myType(ARG_BOOL), myBool(aBool) {}
	MG_Arg(const std::vector<bool>& aVBool) : myType(ARG_V_BOOL), myVBool(aVBool) {}

	inline const TYPE&					Type	(void) const { return myType; }
	inline double						Double	(void) const { return myDouble; }
	inline const std::string&			String	(void) const { return myString; }
	inline const MG_Date&				Date	(void) const { return myDate; }
	inline const std::vector<double>&	VDouble	(void) const { return myVDouble; }
	inline bool							Bool	(void) const { return myBool; }
	inline const std::vector<bool>&		VBool	(void) const { return myVBool; }

	friend std::ostream& operator<< (std::ostream& aOs, const MG_Arg& aArg)
	{
		aOs << aArg.Double();
		return aOs;
	}

private:
	TYPE myType;

	double myDouble;
	std::string myString;
	MG_Date myDate;
	std::vector<double> myVDouble;
	bool myBool;
	std::vector<bool> myVBool;
};


class MG_Func
{
public:
	MG_Func(void);
	virtual ~MG_Func(void);

	virtual double Eval(const std::vector<MG_Arg>& aArgs) = 0;
	virtual std::vector<double> Eval(const std::vector<MG_Arg>& aArgs, const std::vector<double>& aStates);

	virtual void SetModel(const MG_PricingModelPtr& ) {}
};

class MG_MaxFunc : public MG_Func
{
public:
	MG_MaxFunc(void);

	double Eval(const std::vector<MG_Arg>& aArgs);
};

class MG_MinFunc : public MG_Func
{
public:
	MG_MinFunc(void);

	double Eval(const std::vector<MG_Arg>& aArgs);
};

class MG_AbsFunc : public MG_Func
{
public:
	MG_AbsFunc(void);

	double Eval(const std::vector<MG_Arg>& aArgs);
};

class MG_ExpFunc : public MG_Func
{
public:
	MG_ExpFunc(void);

	double Eval(const std::vector<MG_Arg>& aArgs);
};

class MG_LogFunc : public MG_Func
{
public:
	MG_LogFunc(void);

	double Eval(const std::vector<MG_Arg>& aArgs);
};

class MG_PowFunc : public MG_Func
{
public:
	MG_PowFunc(void);

	double Eval(const std::vector<MG_Arg>& aArgs);
};

class MG_IfFunc : public MG_Func
{
public:
	MG_IfFunc(void);

	double Eval(const std::vector<MG_Arg>& aArgs);
};

class MG_LiborFunc : public MG_Func
{
public:
	MG_LiborFunc(void);

	double Eval(const std::vector<MG_Arg>& aArgs);
	std::vector<double> Eval(const std::vector<MG_Arg>& aArgs, const std::vector<double>& aStates);

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
