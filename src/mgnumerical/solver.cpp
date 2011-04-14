#include "solver.h"
#include "exception.h"

#include <math.h>


using namespace std;
using namespace MG;


/*
 * Constructor for base class
 */
MG_Solver::MG_Solver(	const MG_UnaryFuncPtr	& aFunc
					,	const double			& aError
					,	const double			& aTolerance
					,	const unsigned int		& aMaxIter)
					:	myFunc(aFunc), myError(aError), myTolerance(aTolerance), myMaxIter(aMaxIter), myNbIter(0)
{
	assert(aError > 0);
}


/* Newton Raphson
 * Constructor for Newton Raphson solving method
 */
MG_NewtonRaphsonSolver::MG_NewtonRaphsonSolver	(	const MG_UnaryFuncPtr	& aFunc
												,	const MG_UnaryFuncPtr	& aFuncPrime
												,	const double			& aError
												,	const double			& aTolerance
												,	const unsigned int		& aMaxIter)
												:	MG_Solver(aFunc, aError, aTolerance, aMaxIter)
												,	myFuncPrime(aFuncPrime)
{}

/*
 * Solving algorithm
 */
double MG_NewtonRaphsonSolver::Solve(const double& aInit, const double& aTarget, const double& aMin, const double& aMax)
{
	assert(aMin<aMax && aInit>=aMin && aInit<=aMax);
	double vX0(aInit), vX1, vError, vA(aMin), vB(aMax), vTmp;
	unsigned int vNbTouched = 0;
	myNbIter = 0;
	while (myNbIter != myMaxIter)
	{
		vTmp = myFuncPrime->operator()(vX0);
		if (vTmp == 0)
		{
			vX0 = vX0 + (vB-vA)/10; // just a trick to move to a non optimal point
			vTmp = myFuncPrime->operator()(vX0);
			myNbIter++;
			continue;
		}
		vX1 = vX0 - myFunc->operator()(vX0) / vTmp;
		vError = fabs(myFunc->operator()(vX1) - aTarget);
		if (vError < myError)
			return vX1;
		if (vX1 < vA)
		{
			vX0 = vA;
			vNbTouched++;
		}
		else if (vX1 > vB)
		{
			vX0 = vB;
			vNbTouched++;
		}
		else
			vX0 = vX1;
		if (vNbTouched > 2)
			MG_THROW("MG_NewtonRaphsonSolver::Solve, solver cannot find the root.");
		myNbIter++;
	}
	MG_THROW("MG_NewtonRaphsonSolver::Solve, solver cannot find the root.");
}


/* Brent
 * Constructor for Brent solving method
 */
MG_BrentSolver::MG_BrentSolver	(	const MG_UnaryFuncPtr	& aFunc
								,	const double			& aEps
								,	const double			& aError
								,	const double			& aTolerance
								,	const unsigned int		& aMaxIter)
								:	MG_Solver(aFunc, aError, aTolerance, aMaxIter)
								,	myEpsilon(aEps), myBest(0.)
{}

/*
 * Solving algorithm
 */
double MG_BrentSolver::Solve(const double& , const double& aTarget, const double& aMin, const double& aMax)
{
	assert(aMin<aMax);

	double vA(aMin), vB(aMax), vC(aMax), vD(0.), vE(0.);
	double vMin1, vMin2;
	double vFA = myFunc->operator()(vA);
	double vFB = myFunc->operator()(vB);
	double vFC, vP, vQ, vR, vS, vXm, vTolerance;

	if (fabs(vFA-aTarget) < myTolerance)
		return vA;

	if (fabs(vFB-aTarget) < myTolerance)
		return vB;

	if (vFA*vFB >= 0.)
	{
		myBest = fabs(vFA)>fabs(vFB) ? vB : vA;
		MG_THROW("MG_BrentSolver::Solve, solver cannot find the root.");
	}

	vFC = vFB;
	for(myNbIter=0; myNbIter<myMaxIter; myNbIter++)
	{
		if (vFB*vFC > 0.)
		{
			vC	= vA;
			vFC	= vFA;
			vE	= vD = vB-vA;
		}
		if (fabs(vFC) < fabs(vFB))
		{
			vA	= vB;
			vB	= vC;
			vC	= vA;
			vFA	= vFB;
			vFB	= vFC;
			vFC	= vFA;
		}
		vTolerance = 2.*myEpsilon*fabs(vB) + 0.5*myTolerance;
		vXm = 0.5*(vC-vB);
		if (fabs(vXm)<=vTolerance || vFB==0.)
			return vB;
		if (fabs(vE)>=vTolerance && fabs(vFA)>fabs(vFB))
		{
			vS = vFB/vFA;
			if (vA == vC)
			{
				vP = 2.*vXm*vS;
				vQ = 1. - vS;
			}
			else
			{
				vQ = vFA/vFC;
				vR = vFB/vFC;
				vP = vS*(2.*vXm*vQ*(vQ-vR) - (vB-vA)*(vR-1.));
				vQ = (vQ - 1.)*(vR - 1.)*(vS - 1.);
			}
			if (vP > 0.)
				vQ = -vQ;
			vP = fabs(vP);
			vMin1 = 3.0*vXm*vQ - fabs(vTolerance*vQ);
			vMin2 = fabs(vE*vQ);
			if (2.*vP < min(vMin1, vMin2))
			{
				vE = vD;
				vD = vP/vQ;
			}
			else
			{
				vD = vXm;
				vE = vD;
			}
		}
		else
		{
			vD = vXm;
			vE = vD;
		}
		vA	= vB;
		vFA	= vFB;
		if (fabs(vD) > vTolerance)
			vB += vD;
		else
			vB += vTolerance>=0. ? fabs(vXm) : -fabs(vXm);
		vFB = myFunc->operator()(vB) - aTarget;

		if (fabs(vFB) < myTolerance)
			return vB;
	}

	MG_THROW("MG_BrentSolver::Solve, solver cannot find the root.");
}
