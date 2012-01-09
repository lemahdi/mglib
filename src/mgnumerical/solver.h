/*
 * Copyright			: 2011 by MG
 * File					: solver.h
 * Version				: 0.1.21
 * Last changed			: 13 MAR 2011
 * Purpose				: MG_Solver is a base class for solver methods
 * Author				: MM Akkouh
 * Notes				: Solver class, ..
 */


#pragma once


#include "mgnova/typedef.h"
#include "mgnova/object.h"
#include "mgnova/patterns/countedptr.hpp"
#include "mgnova/wrapper/function.h"

#include "gsl/gsl_roots.h"


MG_NAMESPACE_BEGIN


/* Solver Base Class */
class MG_Solver : public MG_XLObject
{
public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_Solver)

	ASSIGN_OPERATOR(MG_Solver)
	SWAP_DECL(MG_Solver)

	MG_Solver(const double& aEpsAbs, const double& aEpsRel, const size_t& aMaxIter, const std::string& aName = "");

	virtual ~MG_Solver(void);

	/* State */
	virtual void Load(const MG_FunctionPtr& aFunc) = 0;
	inline int		GetStatus(void) const { return myStatus; }
	inline size_t	GetNbIter(void) const { return myNbIter; }

	/* Engine */
	virtual double Solve(void) = 0;

protected:
	std::string		myName;
	MG_FunctionPtr	myFunc;

	int				myStatus;
	double			myEpsAbs;
	double			myEpsRel;
	size_t			myMaxIter;
	size_t			myNbIter;

};


/* 0 Order Solver Class */
class MG_FSolver : public MG_Solver
{
public:
	enum FSOLVER_TYPE { BISECTION, FALSEPOS, BRENT };
	static const gsl_root_fsolver_type* From_MGType_To_GSLType(const FSOLVER_TYPE& aType);

	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_FSolver)

	ASSIGN_OPERATOR(MG_FSolver)
	CLONE_METHOD(MG_FSolver)
	SWAP_DECL(MG_FSolver)

	MG_FSolver(const FSOLVER_TYPE& aType, const double& aMin, const double& aMax, const double& aEpsAbs, const double& aEpsRel, const size_t& aMaxIter);

	virtual ~MG_FSolver(void);

	/* State */
	inline void SetEpsilons	(const double& aEpsAbs, const double& aEpsRel) { myEpsAbs = aEpsAbs; myEpsRel = aEpsRel; }
	inline void SetMaxIter	(const size_t& aMaxIter) { myMaxIter = aMaxIter; }
	
	void Load	(const MG_FunctionPtr& aFunc);
	void Reload	(const double& aMin, const double& aMax);

	/* Engine */
	double Solve(void);

private:
	gsl_root_fsolver* mySolver;
	const FSOLVER_TYPE myType;
	double myMin;
	double myMax;

};


/* 1st Order Solver Class */
class MG_FDfSolver : public MG_Solver
{
public:
	enum FDFSOLVER_TYPE { NEWTON, SECANT, STEFFENSON };
	static const gsl_root_fdfsolver_type* From_MGType_To_GSLType(const FDFSOLVER_TYPE& aType);

	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_FDfSolver)

	ASSIGN_OPERATOR(MG_FDfSolver)
	CLONE_METHOD(MG_FDfSolver)
	SWAP_DECL(MG_FDfSolver)

	MG_FDfSolver(const FDFSOLVER_TYPE& aType, const double& aGuess, const double& aEpsAbs, const double& aEpsRel, const size_t& aMaxIter);

	virtual ~MG_FDfSolver(void);

	/* State */
	void Load	(const MG_FunctionPtr& aFunc);
	void Reload	(const double& aGuess);

	/* Engine */
	double Solve(void);

private:
	gsl_root_fdfsolver* mySolver;
	const FDFSOLVER_TYPE myType;
	double myGuess;

};


MG_NAMESPACE_END

