/*
 * Copyright			: 2011 by MG
 * File					: function.h
 * Version				: 0.1.24
 * Last changed			: 24 AUG 2011
 * Purpose				: MG_Function is a wrapper class for GSL
 * Author				: MM Akkouh
 * Notes				:
 */


#pragma once


#include "mgnova/glob/typedef.h"

#include "gsl/gsl_math.h"


MG_NAMESPACE_BEGIN


/* Generic Parameter Framework */
//==> Generic If
template<bool b>
struct If_Equal {};

template<>
struct If_Equal<true> { typedef int CHECK; };

//==> Parameter Class
struct UnknownParam
{
	enum { NB = 0 };
};

struct OneParam
{
	enum { NB = 1 };
	typedef UnknownParam PREV;
	
	double a;
};

struct TwoParam
{
	enum { NB = 2 };
	typedef OneParam PREV;
	
	double a;
	double b;
};

struct ThreeParam
{
	enum { NB = 3 };
	typedef TwoParam PREV;
	
	double a;
	double b;
	double c;
};

//==> Generic Until
template<unsigned int n, typename P, bool b>
struct Until {};
 
template<unsigned int n, typename P>
struct Until<n,P,true>
{
	typedef P PARAM;
};
 
template<unsigned int n,typename P>
struct Until<n,P,false>
{
	typedef typename Until<n,typename P::PREV,P::PREV::NB==n>::PARAM PARAM;
};

//==> Generic Parameter Class
// use "typename Param<n>::P myParam;" as meber parameter
template<unsigned int n>
struct Param
{
	typedef ThreeParam MaxParam;
	typedef typename If_Equal<n <= MaxParam::NB>::CHECK CHECK;
	typedef typename Until<n,MaxParam,MaxParam::NB==n>::PARAM P;
};


/* Base Function Class */
class MG_Function
{
public:
	/* Constructors / Destructor */
	SWAP_DECL(MG_Function)

	MG_Function(void) {}

	virtual ~MG_Function(void) {}

public:
	/* Engine */
	virtual double	F_Func	(const double& aX) = 0;
	virtual double	Df_Func	(const double& aX) = 0;
	virtual void	FDf_Func(const double& aX, double& aF, double& aDf) = 0;

	static double	F	(double aX, void* aEgine);
	static double	Df	(double aX, void* aEgine);
	static void		FDf	(double aX, void* aEgine, double* aF, double* aDf);

};


/* 0 Order Function Class */
class MG_FFunction : public MG_Function
{
public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_FFunction)

	ASSIGN_OPERATOR(MG_FFunction)
	SWAP_DECL(MG_FFunction)

	MG_FFunction(void);

	virtual ~MG_FFunction(void);

	/* Accessors */
	inline gsl_function* GetFunc(void) { return &myFunc; }

private:
	gsl_function myFunc;
};


/* 1st Order Function Class */
class MG_FDfFunction : public MG_Function
{
public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_FDfFunction)

	ASSIGN_OPERATOR(MG_FDfFunction)
	SWAP_DECL(MG_FDfFunction)

	MG_FDfFunction(void);

	virtual ~MG_FDfFunction(void);

	/* Accessors */
	inline gsl_function_fdf* GetFunc(void) { return &myFunc; }

private:
	gsl_function_fdf myFunc;
};


MG_NAMESPACE_END

