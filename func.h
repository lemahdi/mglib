#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <assert.h>

#include "typedef.h"
#include "singleton.hpp"


class Func
{
public:
	Func(void) {}
	virtual ~Func(void) {}

	virtual double Eval(const std::vector<double>& aArgs) = 0;
};

class MaxFunc : public Func
{
public:
	MaxFunc(void) {}
	virtual ~MaxFunc(void) {}

	virtual double Eval(const std::vector<double>& aArgs);
};

class MinFunc : public Func
{
public:
	MinFunc(void) : Func() {}
	virtual ~MinFunc(void) {}

	virtual double Eval(const std::vector<double>& aArgs);
};

class AbsFunc : public Func
{
public:
	AbsFunc(void) : Func() {}
	virtual ~AbsFunc(void) {}

	virtual double Eval(const std::vector<double>& aArgs);
};

class ExpFunc : public Func
{
public:
	ExpFunc(void) : Func() {}
	virtual ~ExpFunc(void) {}

	virtual double Eval(const std::vector<double>& aArgs);
};

class LogFunc : public Func
{
public:
	LogFunc(void) : Func() {}
	virtual ~LogFunc(void) {}

	virtual double Eval(const std::vector<double>& aArgs);
};

class PowFunc : public Func
{
public:
	PowFunc(void) : Func() {}
	virtual ~PowFunc(void) {}

	virtual double Eval(const std::vector<double>& aArgs);
};

class FuncBuilder
{
	template<class T> friend class Singleton;

private:
	FuncBuilder(void);

public:
	~FuncBuilder(void) {}

	inline FuncPtr GetFunc(const std::string& aFuncName) { assert(myFuncs.find(aFuncName)==myFuncs.end()); return myFuncs[aFuncName]; }

private:
	NameFuncMap myFuncs;
};
typedef Singleton<FuncBuilder> SFuncBuilder;
