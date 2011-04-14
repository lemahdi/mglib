/*
 * Copyright			: 2011 by MG
 * File					: solver.h
 * Version				: 0.1.21
 * Last changed			: 13 MAR 2011
 * Purpose				: MG_Solver is a base class for solver methods
 * Author				: MM Akkouh
 * Notes				: Newton Raphson, ..
 */


#pragma once


#include "typedef.h"
#include "infrafunc.h"
#include "countedptr.hpp"

#include <assert.h>


#define MY_MAX_ITER		100
#define MY_ERROR		1e-6
#define MY_TOLERANCE	1e-10
#define MY_EPSILON		3e-8


MG_NAMESPACE_BEGIN


class MG_Solver
{
private:
	FAKE_ASSIGN_OPERATOR(MG_Solver)

public:
	MG_Solver	(	const MG_UnaryFuncPtr	& aFunc
				,	const double			& aError		= MY_ERROR
				,	const double			& aTolerance	= MY_TOLERANCE
				,	const unsigned int		& aMaxIter		= MY_MAX_ITER);
	virtual ~MG_Solver(void) {}

public:
	inline unsigned int GetNbIter(void) const { return myNbIter; }

	virtual double Solve(const double& aInit, const double& aTarget, const double& aMin, const double& aMax) = 0;

protected:
	MG_UnaryFuncPtr		myFunc;
	const double		myError;
	const double		myTolerance;
	const unsigned int	myMaxIter;
	unsigned int		myNbIter;
};


class MG_NewtonRaphsonSolver : public MG_Solver
{
private:
	FAKE_ASSIGN_OPERATOR(MG_NewtonRaphsonSolver)

public:
	MG_NewtonRaphsonSolver	(	const MG_UnaryFuncPtr	& aFunc
							,	const MG_UnaryFuncPtr	& aFuncPrime
							,	const double			& aError		= MY_ERROR
							,	const double			& aTolerance	= MY_TOLERANCE
							,	const unsigned int		& aMaxIter		= MY_MAX_ITER);
	virtual ~MG_NewtonRaphsonSolver(void) {}

public:
	virtual double Solve(const double& aInit, const double& aTarget, const double& aMin, const double& aMax);

private:
	MG_UnaryFuncPtr		myFuncPrime;
};


class MG_BrentSolver : public MG_Solver
{
private:
	FAKE_ASSIGN_OPERATOR(MG_BrentSolver)

public:
	MG_BrentSolver	(	const MG_UnaryFuncPtr	& aFunc
					,	const double			& aEps			= MY_EPSILON
					,	const double			& aError		= MY_ERROR
					,	const double			& aTolerance	= MY_TOLERANCE
					,	const unsigned int		& aMaxIter		= MY_MAX_ITER);
	virtual ~MG_BrentSolver(void) {}

public:
	inline double GetBest(void) const { return myBest; }

	virtual double Solve(const double& aInit, const double& aTarget, const double& aMin, const double& aMax);

private:
	double myBest;
	double myEpsilon;
};


MG_NAMESPACE_END


#undef MY_MAX_ITER
#undef MY_ERROR
#undef MY_TOLERANCE
#undef MY_EPSILON
