/*
 * Copyright			: 2012 by MG
 * Version				: 0.1.23
 * Date					: 07 OCT 2012
 * Purpose				: MG_MonteCarloMethod is a Monte Carlo numerical class
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "genpricer/numerics/nummethod.h"
#include "mgnova/patterns/countedptr.hpp"
#include "mgnova/wrapper/matrix.h"


MG_NAMESPACE_BEGIN


class MG_RandDist;


/* Base class for models */
class MG_MonteCarloMethod : public MG_NumericalMethod
{
public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_MonteCarloMethod)

	ASSIGN_OPERATOR(MG_MonteCarloMethod)
	SWAP_DECL(MG_MonteCarloMethod)
	CLONE_METHOD(MG_MonteCarloMethod)

	MG_MonteCarloMethod	(	const size_t		& aTimeSteps
						,	const size_t		& aSimulNb
						,	const MG_RandDistPtr& aDist);

	virtual ~MG_MonteCarloMethod(void);

	/* accessors */
	inline size_t TimeSteps(void) const { return myTimeSteps; }
	inline size_t SimulNb(void) const { return mySimulNb; }
	inline const MG_Matrix Simulations(void) const { return mySimulations; }

	/* engine */
	void Simulate(void);

private:
	size_t myTimeSteps;
	size_t mySimulNb;
	MG_RandDistPtr myDist;

	MG_Matrix mySimulations;
};


MG_NAMESPACE_END
