#include "mgnova/numerical/solver.h"

#include "gsl/gsl_errno.h"
#include "mgnova/glob/exception.h"


using namespace std;
using namespace MG;


/*
 * Solver Base Class
 */
MG_Solver::MG_Solver(	const MG_Solver& aRight)
					:	MG_XLObject(aRight)
					,	myName(aRight.myName), myFunc(NULL)
					,	myStatus(aRight.myStatus)
					,	myEpsAbs(aRight.myEpsAbs), myEpsRel(aRight.myEpsRel)
					,	myMaxIter(aRight.myMaxIter), myNbIter(aRight.myNbIter)
{}

void MG_Solver::Swap(MG_Solver& aRight)
{
	MG_XLObject::Swap(aRight);
	myName.swap(aRight.myName);
	myFunc->Swap(*aRight.myFunc);
	swap(myStatus, aRight.myStatus);
	swap(myEpsAbs, aRight.myEpsAbs);
	swap(myEpsRel, aRight.myEpsRel);
	swap(myMaxIter, aRight.myMaxIter);
	swap(myNbIter, aRight.myNbIter);
}

MG_Solver::MG_Solver(	const double& aEpsAbs
					,	const double& aEpsRel
					,	const size_t& aMaxIter
					,	const string& aName)
					:	MG_XLObject()
					,	myEpsAbs(aEpsAbs), myEpsRel(aEpsRel)
					,	myMaxIter(aMaxIter), myNbIter(0)
					,	myName(aName), myFunc(NULL), myStatus(GSL_EOF)
{}

MG_Solver::~MG_Solver()
{}


/*
 * 0 Order Solver Class
 */
const gsl_root_fsolver_type* MG_FSolver::From_MGType_To_GSLType(const FSOLVER_TYPE& aType)
{
	switch(aType)
	{
	case BISECTION:
		return gsl_root_fsolver_bisection;
		
	case FALSEPOS:
		return gsl_root_fsolver_falsepos;
		
	case BRENT:
		return gsl_root_fsolver_brent;

	default: return NULL;
	}
}

MG_FSolver::MG_FSolver	(	const MG_FSolver& aRight)
						:	MG_Solver(aRight)
						,	mySolver(NULL), myType(aRight.myType)
						,	myMin(aRight.myMin), myMax(aRight.myMax)
{
	if (aRight.mySolver)
	{
		const gsl_root_fsolver_type* vType = From_MGType_To_GSLType(myType);
		mySolver = gsl_root_fsolver_alloc(vType);
	}
}

void MG_FSolver::Swap(MG_FSolver& aRight)
{
	MG_Solver::Swap(aRight);
	swap(mySolver, aRight.mySolver);
	swap(const_cast<FSOLVER_TYPE&>(myType), const_cast<FSOLVER_TYPE&>(aRight.myType));
	swap(myMin, aRight.myMin);
	swap(myMax, aRight.myMax);
}

MG_FSolver::MG_FSolver	(	const FSOLVER_TYPE& aType
						,	const double& aMin
						,	const double& aMax
						,	const double& aEpsAbs
						,	const double& aEpsRel
						,	const size_t& aMaxIter)
						:	MG_Solver(aEpsAbs, aEpsRel, aMaxIter)
						,	mySolver(NULL), myType(aType)
						,	myMin(aMin), myMax(aMax)
{
	myXLName = MG_FSOLVER_XL_NAME;

	const gsl_root_fsolver_type* vType = From_MGType_To_GSLType(myType);
	if (vType)
	{
		mySolver = gsl_root_fsolver_alloc(vType);
		myName = gsl_root_fsolver_name(mySolver);
	}
}

MG_FSolver::~MG_FSolver()
{
	 gsl_root_fsolver_free(mySolver);
}

void MG_FSolver::Load(const MG_FunctionPtr& aFunc)
{
	myFunc = aFunc;
	gsl_root_fsolver_set(mySolver, dynamic_cast<MG_FFunction*>(&*myFunc)->GetFunc(), myMin, myMax);
}

void MG_FSolver::Reload(const double& aMin, const double& aMax)
{
	myMin = aMin;
	myMax = aMax;
	gsl_root_fsolver_set(mySolver, dynamic_cast<MG_FFunction*>(&*myFunc)->GetFunc(), myMin, myMax);
}

double MG_FSolver::Solve()
{
	myNbIter = 0;
	double vRoot, vLowX, vHighX;
	do
	{
		++myNbIter;
		myStatus = gsl_root_fsolver_iterate(mySolver);
		if (myStatus==GSL_EBADFUNC || myStatus==GSL_EZERODIV)
			MG_THROW("Error while iterating in solver.");

		vRoot		= gsl_root_fsolver_root(mySolver);
		vLowX		= gsl_root_fsolver_x_lower(mySolver);
		vHighX		= gsl_root_fsolver_x_upper(mySolver);
		myStatus	= gsl_root_test_interval(vLowX, vHighX, myEpsAbs, myEpsRel);
	} while (myStatus==GSL_CONTINUE && myNbIter<myMaxIter);

	return vRoot;
}


/*
 * 1st Order Solver Class
 */
const gsl_root_fdfsolver_type* MG_FDfSolver::From_MGType_To_GSLType(const FDFSOLVER_TYPE& aType)
{
	switch(aType)
	{
	case NEWTON:
		return gsl_root_fdfsolver_newton;
		
	case SECANT:
		return gsl_root_fdfsolver_secant;
		
	case STEFFENSON:
		return gsl_root_fdfsolver_steffenson;

	default: return NULL;
	}
}

MG_FDfSolver::MG_FDfSolver	(	const MG_FDfSolver& aRight)
							:	MG_Solver(aRight)
							,	mySolver(NULL), myType(aRight.myType)
							,	myGuess(aRight.myGuess)
{
	if (aRight.mySolver)
	{
		const gsl_root_fdfsolver_type* vType = From_MGType_To_GSLType(myType);
		mySolver = gsl_root_fdfsolver_alloc(vType);
	}
}

void MG_FDfSolver::Swap(MG_FDfSolver& aRight)
{
	MG_Solver::Swap(aRight);
	swap(mySolver, aRight.mySolver);
	swap(const_cast<FDFSOLVER_TYPE&>(myType), const_cast<FDFSOLVER_TYPE&>(aRight.myType));
	swap(myGuess, aRight.myGuess);
}

MG_FDfSolver::MG_FDfSolver	(	const FDFSOLVER_TYPE& aType
							,	const double& aGuess
							,	const double& aEpsAbs
							,	const double& aEpsRel
							,	const size_t& aMaxIter)
							:	MG_Solver(aEpsAbs, aEpsRel, aMaxIter)
							,	mySolver(NULL), myType(aType)
							,	myGuess	(aGuess)
{
	myXLName = MG_FDFSOLVER_XL_NAME;

	const gsl_root_fdfsolver_type* vType = From_MGType_To_GSLType(myType);
	if (vType)
	{
		mySolver = gsl_root_fdfsolver_alloc(vType);
		myName = gsl_root_fdfsolver_name(mySolver);
	}
}

MG_FDfSolver::~MG_FDfSolver()
{
	gsl_root_fdfsolver_free(mySolver);
}

void MG_FDfSolver::Load(const MG_FunctionPtr& aFunc)
{
	myFunc = aFunc;
	gsl_root_fdfsolver_set(mySolver, dynamic_cast<MG_FDfFunction*>(&*myFunc)->GetFunc(), myGuess);
}

void MG_FDfSolver::Reload(const double& aGuess)
{
	myGuess = aGuess;
	gsl_root_fdfsolver_set(mySolver, dynamic_cast<MG_FDfFunction*>(&*myFunc)->GetFunc(), myGuess);
}

double MG_FDfSolver::Solve()
{
	myNbIter = 0;
	double vRoot(myGuess), vPrevious;
	do
	{
		++myNbIter;
		myStatus = gsl_root_fdfsolver_iterate(mySolver);
		if (myStatus==GSL_EBADFUNC || myStatus==GSL_EZERODIV)
			MG_THROW("Error while iterating in solver.");

		vPrevious	= vRoot;
		vRoot		= gsl_root_fdfsolver_root(mySolver);
		myStatus	= gsl_root_test_delta(vRoot, vPrevious, myEpsAbs, myEpsRel);
	} while (myStatus==GSL_CONTINUE && myNbIter<myMaxIter);

	return vRoot;
}
