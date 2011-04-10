#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <string>

#include "singleton.hpp"
#include "countedptr.hpp"
#include "typedef.h"

#define MG_FuncPtr CountedPtr<MG_Func>

class MG_Func
{
public:
	MG_Func(void);
	~MG_Func(void);

	virtual double Eval(const std::vector<double>& aArgs) = 0;
};

class MG_MaxFunc : public MG_Func
{
public:
	MG_MaxFunc(void);

	virtual double Eval(const std::vector<double>& aArgs);
};

class MG_MinFunc : public MG_Func
{
public:
	MG_MinFunc(void);

	virtual double Eval(const std::vector<double>& aArgs);
};

class MG_AbsFunc : public MG_Func
{
public:
	MG_AbsFunc(void);

	virtual double Eval(const std::vector<double>& aArgs);
};

class MG_ExpFunc : public MG_Func
{
public:
	MG_ExpFunc(void);

	virtual double Eval(const std::vector<double>& aArgs);
};

class MG_LogFunc : public MG_Func
{
public:
	MG_LogFunc(void);

	virtual double Eval(const std::vector<double>& aArgs);
};

class MG_PowFunc : public MG_Func
{
public:
	MG_PowFunc(void);

	virtual double Eval(const std::vector<double>& aArgs);
};

class MG_FuncBuilder
{
	template<class T> friend class Singleton;

private:
	MG_FuncBuilder(void) {}

public:
	~MG_FuncBuilder(void) {}

	void Init(void);

	MG_FuncPtr GetFunc(const std::string& aFuncName);

private:
	NameFuncMap myFuncs;
};
typedef Singleton<MG_FuncBuilder> MG_SFuncBuilder;
