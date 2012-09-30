/*
 * Copyright			: 2011 by MG
 * Version				: 0.1.21
 * Date					: 17 MAR 2011
 * Purpose				: some structures to define simple fucntions
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include <functional>
#include <math.h>


MG_NAMESPACE_BEGIN


struct MG_UnaryFunc : public std::unary_function<double,double>
{
	virtual double operator() (const double& aArg) = 0;
};
/*
struct MG_TestFunc : public MG_UnaryFunc
{
	virtual double operator() (const double& aArg) { return exp(aArg)-aArg*aArg; }
};
struct MG_TestFuncPrime : public MG_UnaryFunc
{
	virtual double operator() (const double& aArg) { return exp(aArg)-2*aArg; }
};*/


MG_NAMESPACE_END

