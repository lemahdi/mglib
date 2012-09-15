/*
 * Copyright			: 2010 by MG
 * File					: func.h
 * Version				: 0.1.14
 * Last changed			: 21 DEC 2010
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


MG_NAMESPACE_BEGIN


class MG_Func
{
public:
	MG_Func(void);
	virtual ~MG_Func(void);

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

class MG_IfFunc : public MG_Func
{
public:
	MG_IfFunc(void);

	virtual double Eval(const std::vector<double>& aArgs);
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
