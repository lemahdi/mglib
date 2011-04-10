#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <string>

#include "singleton.hpp"
#include "typedef.h"


class Func
{
public:
	Func(void);
	~Func(void);

	virtual double Eval(const std::vector<double>& aArgs) = 0;
};

class MaxFunc : public Func
{
public:
	MaxFunc(void);

	virtual double Eval(const std::vector<double>& aArgs);
};

class MinFunc : public Func
{
public:
	MinFunc(void);

	virtual double Eval(const std::vector<double>& aArgs);
};

class AbsFunc : public Func
{
public:
	AbsFunc(void);

	virtual double Eval(const std::vector<double>& aArgs);
};

class ExpFunc : public Func
{
public:
	ExpFunc(void);

	virtual double Eval(const std::vector<double>& aArgs);
};

class LogFunc : public Func
{
public:
	LogFunc(void);

	virtual double Eval(const std::vector<double>& aArgs);
};

class PowFunc : public Func
{
public:
	PowFunc(void);

	virtual double Eval(const std::vector<double>& aArgs);
};

class FuncBuilder
{
	template<class T> friend class Singleton;

private:
	FuncBuilder(void);

public:
	~FuncBuilder(void);

	Func* GetFunc(const std::string& aFuncName);

private:
	NameFuncMap myFuncs;
};
typedef Singleton<FuncBuilder> SFuncBuilder;
